/*******************************************************************************/
/* nand_flash.c: nand_flash controller configure file for Samsung S3C2440      */
/*******************************************************************************/

#include "global.h"

/* Define register address about nand_flash controller */

#define NFCONF (*(volatile unsigned long*)0x4E000000) // 配置寄存器
#define NFCONT (*(volatile unsigned long*)0x4E000004) // 控制寄存器
#define NFCMMD (*(volatile unsigned long*)0x4E000008) // 命令寄存器
#define NFADDR (*(volatile unsigned long*)0x4E00000C) // 地址寄存器
#define NFDATA (*(volatile unsigned long*)0x4E000010) // 数据寄存器
#define NFMECCD0 (*(volatile unsigned long*)0x4E000014) // 主数据区域寄存器
#define NFMECCD1 (*(volatile unsigned long*)0x4E000018) // 主数据区域寄存器
#define NFSECCD (*(volatile unsigned long*)0x4E00001C) // 备份区域ECC寄存器
#define NFSTAT (*(volatile unsigned long*)0x4E000020) // 状态寄存器
#define NFESATA0 (*(volatile unsigned long*)0x4E000024) // ECC0 状态寄存器
#define NFESTAT1 (*(volatile unsigned long*)0x4E000028) // ECC1 状态寄存器
#define NFMECC0 (*(volatile unsigned long*)0x4E00002C) // 主数据区域 ECC0 状态寄存器
#define NFMECC1 (*(volatile unsigned long*)0x4E000030) // 主数据区域 ECC1 状态寄存器
#define NFSECC (*(volatile unsigned long*)0x4E000034) // 备份区域 ECC 状态寄存器
#define NFSBLK (*(volatile unsigned long*)0x4E000038) // 锁定块起始地址寄存器
#define NFEBLK (*(volatile unsigned long*)0x4E00003C) // 锁定块结束块地址寄存器


/* Values of Nand Flash controller register */

#define NFCONT_Val 0x0001


/* 相关参数变量的声明 */

static int TACLS; // 从 CLK/ALK 有效到 nWE/nOE 的时间
static int TWRPH0; // nWE/nOE 的有效时间
static int TWRPH1; // 从释放 CLE/ALE 到 nWE/nOE 不活动的时间

#define PAGESIZE 512 //页大小（字节单位）


/* Nand Flash Controller 的命令码 */

#define CMD_RST 0xFF // 复位
#define CMD_RD1 0x00 // 读一个页的前半部分
#define CMD_RD2 0x01 // 读一个页的后半部分
#define CMD_RDD 0x90 // 读芯片的ID号
#define CMD_PROG 0x80 // 进入写操作模式
#define CMD_PROG_END 0x10 // 写结束
#define CMD_ERASE 0x60 // 进入块擦除模式
#define CMD_ERASE_END 0xD0 // 块擦除结束
#define CMD_RDSTAT 0x70 // 读内部状态寄存器


/* 操作的函数实现  */

// 发送命令
#define NF_CMD(cmd) {NFCMMD = cmd;}

// 写入地址
#define NF_ADDR(addr) {NFADDR = addr;}

// Nand Flash 芯片选中
#define NF_nFCE_L() {NFCONT &= ~(1<<1);}

// 取消 Nand Flash 芯片选中
#define NF_nFCE_H() {NFCONT |= (1<<1);}

// 初始化 ECC
#define NF_RSTECC() {NFCONT |= (1<<4);}

// 读数据
#define NF_RDDATA() (NFDATA)

// 写数据
#define NF_WRDATA(data) {NFDATA = data;}

// 等待一段时间，直到 Nand Flash 处于准备好的状态
#define NF_WAITRB() {while(!(NFSTAT & (1 << 0)));}

// 锁住主数据区域 ECC 的值，这时 ECC 的值不会改变
#define MainECCLock() {NFCONT |= (1<<5)}

// 开锁主数据区域 ECC，允许生成新的 ECC
#define MainECCUnlock() {NFCONT &= ~(1<<5)}

// 锁住备份区域 ECC 的值，这时 ECC 的值不会改变
#define SpareECCLock() {NFCONT |= (1<<6)}

// 开锁备份区域 ECC，允许生成新的 ECC
#define SpareECCUnlock() {NFCONT &= ~(1<<6)}



/* Function: 复位 Nand Flash 外部芯片，注意不是复位 Nand Flash 控制器*/

static void NF_Reset()
{
  int i = 0;

  NF_nFCE_L(); // 片选 Nand Flash 芯片
  NF_CMD( CMD_RST ); // 复位命令
  for(i=0 ; i<10 ; i++); //等待 tWB = 100ns
  NF_WAITRB(); // wait 200~500us;
  NF_nFCE_H(); // 取消 Nand Flash 选中
}


/* Function: Setup nand_flash controller for S3C2440 */

void nand_flash_setup()
{
  //得到 TACLK，TWRPH0，TWRPH1
  TACLS = 1;
  TWRPH0 = 0;
  TWRPH1 = 0;

  NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
  NFCONT = NFCONT_Val;
  NF_Reset(); //复位 Nand Flash 外部芯片
}


/* Function: 获取 Nand Flash 芯片的ID号 */

WORD NF_CheckId()
{
  int i;
  WORD id;

  NF_nFCE_L();
  NF_CMD( CMD_RDD );
  NF_ADDR( 0x0 ); // 指定地址 0x0， 芯片手册要求
  for(i=0 ; i<10 ; i++); // 等待 tWB = 100ns
  id = NF_RDDATA() << 8; // 厂商 ID
  id |= NF_RDDATA(); // 设备 ID
  NF_nFCE_H();

  return id;
}


/* Function: 以页为单位写入 Nand Flash
   参数：block 块号；page 页号；buffer 写入内容的缓冲区 
   返回值：0 写错误；1 写成功 */

static int NF_WritePage(unsigned int block,unsigned int page,BYTE* buffer)
{
  int i;
  unsigned int blockPage = (block << 5) + page;
  BYTE* bufPt = buffer;

  NF_RSTECC();
  NF_nFCE_L();
  NF_CMD( CMD_RD1 ); // 从一个页的第一部分开始写
  NF_CMD( CMD_PROG ); // 开始进入写模式
  NF_ADDR( 0 );
  NF_ADDR( blockPage & 0xff );
  NF_ADDR( ( blockPage >> 8 ) & 0xff );
  NF_ADDR( ( blockPage >> 16 ) & 0xff );

  for(i=0 ; i<PAGESIZE ; i++)
    {
      NF_WRDATA( *bufPt++ );
    }

  NF_CMD( CMD_PROG_END );

  for(i=0 ; i<10 ; i++);
  NF_WAITRB(); //等待 Nand Flash 处于准备状态

  NF_CMD( CMD_RDSTAT );
  for(i=0 ; i<3 ; i++);

  if( NF_RDDATA() & 0x1 )
    { // 如果写有错，则标记为坏块
      NF_nFCE_H();
      NF_MarkBadBlock( block );
      return 0;
    }
  else
    { // 正常退出
      NF_nFCE_H();
      return 1;
    }
}


/* Function: 以页为单位读 Nand Flash
   参数：block 块号；page 页号；buffer 读到的缓冲区 
   返回值：0 读错误；1 读成功 */

static int NF_ReadPage(unsigned int block,unsigned int page,BYTE* buffer)
{
  
}

