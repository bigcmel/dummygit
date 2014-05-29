/*******************************************************************************/
/* nand_flash.c: nand_flash controller configure file for Samsung S3C2440      */
/*******************************************************************************/

/* 这里写的代码是以 K9F1208 型号的 Nand Flash 为目标的，若目标 Nand Flash 不为 K9F1208，则不应用本代码
   K9F1208参数：存储空间为64M
*/

#include "global.h"

/* Define register address about nand_flash controller */

#define NFCONF (*(volatile WORD*)0x4E000000) // 配置寄存器
#define NFCONT (*(volatile WORD*)0x4E000004) // 控制寄存器
#define NFCMMD (*(volatile WORD*)0x4E000008) // 命令寄存器
#define NFADDR (*(volatile WORD*)0x4E00000C) // 地址寄存器
#define NFDATA8 (*(volatile BYTE*)0x4E000010) // 数据寄存器,字节访问，即一次只读一个字节
#define NFMECCD0 (*(volatile WORD*)0x4E000014) // main 区域寄存器
#define NFMECCD1 (*(volatile WORD*)0x4E000018) // mian 区域寄存器
#define NFSECCD (*(volatile WORD*)0x4E00001C) // spare 区域ECC寄存器
#define NFSTAT (*(volatile WORD*)0x4E000020) // 状态寄存器
#define NFESATA0 (*(volatile WORD*)0x4E000024) // ECC0 状态寄存器
#define NFESTAT1 (*(volatile WORD*)0x4E000028) // ECC1 状态寄存器
#define NFMECC0 (*(volatile WORD*)0x4E00002C) // main 区域 ECC0 状态寄存器
#define NFMECC1 (*(volatile WORD*)0x4E000030) // main 区域 ECC1 状态寄存器
#define NFSECC (*(volatile WORD*)0x4E000034) // spare 区域 ECC 状态寄存器
#define NFSBLK (*(volatile WORD*)0x4E000038) // 锁定块起始地址寄存器
#define NFEBLK (*(volatile WORD*)0x4E00003C) // 锁定块结束块地址寄存器


/* Values of Nand Flash controller register */

#define NFCONT_Val 0x00000001 // 手册P144，这里只使能了 nand flash 控制器


/* 相关参数变量的声明 */

static int NF_TACLS; // 从 CLK/ALK 有效到 nWE/nOE 的时间
static int NF_TWRPH0; // nWE/nOE 的有效时间
static int NF_TWRPH1; // 从释放 CLE/ALE 到 nWE/nOE 不活动的时间

#define NF_BLOCKNUM 4096 // 共有4096个块
#define NF_PAGEPBLOCK 32 // 每个块中的页数
#define NF_MAINSIZE 512 // 一个页中的 main 区域大小
#define NF_SPARESIZE 16 // 一个页中的 spare 区域大小
#define NF_PAGESIZE (NF_MAINSIZE + NF_SPARESIZE) // 页大小（字节单位）


/* Nand Flash Controller 的命令码 */

#define NF_CMD_RST 0xFF // 复位
#define NF_CMD_RD1 0x00 // 读一个页的main的第一部分，本质上是将 nand flash 内部指针指向 mian 的第一部分的首部
#define NF_CMD_RD2 0x01 // 读一个页的main的第二部分，本质上是将 nand flash 内部指针指向 mian 的第二部分的首部
#define NF_CMD_RDS 0x50 // 读一个页的spare部分，本质上是将 nand flash 内部指针指向 spare 部分的首部
#define NF_CMD_RDD 0x90 // 读芯片的ID号
#define NF_CMD_PROG 0x80 // 进入写操作模式
#define NF_CMD_PROG_END 0x10 // 写结束
#define NF_CMD_ERASE 0x60 // 进入块擦除模式
#define NF_CMD_ERASE_END 0xD0 // 块擦除结束
#define NF_CMD_RDSTAT 0x70 // 读内部状态寄存器


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

// 读数据,一个字节
#define NF_RDDATA8() (NFDATA8)

// 写数据，一个字节
#define NF_WRDATA8(data) {NFDATA8 = data;}

// 等待一段时间，直到 Nand Flash 处于准备好的状态
#define NF_WAIT_RB() {while(!(NFSTAT & (1 << 0)));}

// 清除 RnB 信号
#define NF_CLEAR_RB() {NFSTAT |= (1 << 2);}

// 锁住主数据区域 ECC 的值，这时 ECC 的值不会改变
#define NF_MainECCLock() {NFCONT |= (1<<5);}

// 开锁主数据区域 ECC，允许生成新的 ECC
#define NF_MainECCUnlock() {NFCONT &= ~(1<<5);}

// 锁住备份区域 ECC 的值，这时 ECC 的值不会改变
#define NF_SpareECCLock() {NFCONT |= (1<<6);}

// 开锁备份区域 ECC，允许生成新的 ECC
#define NF_SpareECCUnlock() {NFCONT &= ~(1<<6);}



/* Function: 复位 Nand Flash 外部芯片，注意不是复位 Nand Flash 控制器*/

static void NF_Reset()
{
  NF_nFCE_L(); // 片选 Nand Flash 芯片
  NF_CLEAR_RB(); // 清除 RnB 信号
  NF_CMD( NF_CMD_RST ); // 复位命令
  NF_WAIT_RB(); // 等待 RnB 信号变高，即不忙
  NF_nFCE_H(); // 取消 Nand Flash 选中
}


/* Function: Setup nand_flash controller for S3C2440 */

void NF_init()
{
  //得到 TACLK，TWRPH0，TWRPH1
  NF_TACLS = 1;
  NF_TWRPH0 = 0;
  NF_TWRPH1 = 0;

  NFCONF = (NF_TACLS << 12) | (NF_TWRPH0 << 8) | (NF_TWRPH1 << 4) | (0 << 0); // 第0位清零，即8位IO
  NFCONT = NFCONT_Val;
  NF_RSTECC(); //复位 ECC
  NF_Reset(); //复位 Nand Flash 外部芯片
}


/* Function: 得到 nand_flash 的块数，页数，和每页的大小 */

void NF_GetBlockPageInfo(unsigned int* nf_blocknum, unsigned int* nf_pagepblock,
			 unsigned int* nf_mainsize, unsigned int* nf_sparesize)
{
  *nf_blocknum = NF_BLOCKNUM;
  *nf_pagepblock = NF_PAGEPBLOCK;
  *nf_mainsize = NF_MAINSIZE;
  *nf_sparesize = NF_SPARESIZE;
}


/* Function: 获取 Nand Flash 芯片的ID号 */

WORD NF_CheckId()
{
  WORD id;

  NF_nFCE_L();
  NF_CLEAR_RB();
  NF_CMD( NF_CMD_RDD );
  NF_ADDR( 0x0 ); // 指定地址 0x0， 芯片手册要求
  NF_WAIT_RB();
  id = NF_RDDATA8() << 8; // 厂商 ID
  id |= NF_RDDATA8(); // 设备 ID
  NF_nFCE_H();

  return id;
}


/* Function: 以页为单位读 Nand Flash
   参数：block 块号；page 页号；buffer 读到的缓冲区 
   返回值：0 读错误；1 读成功 */

unsigned int NF_ReadPage(unsigned int block,unsigned int page,BYTE* buffer)
{
  int i;
  unsigned int blockPage = (block << 5) + page;
  BYTE* bufPt = buffer;
  BYTE ECCbuf[6];

  NF_RSTECC(); // 复位 ECC
  NF_MainECCUnlock(); // 解锁本页 main 区域的 ECC 校验，允许生成 ECC 校验码
  NF_nFCE_L();
  NF_CLEAR_RB(); // 清 RnB 信号
  NF_CMD( NF_CMD_RD1 ); // 从本页的上半部分开始读
  
  NF_ADDR( 0 ); // 从本页的第一个列（字节）开始读
  NF_ADDR( blockPage & 0xff ); // 这三行代码指明页号
  NF_ADDR( ( blockPage >> 8 ) & 0xff );
  NF_ADDR( ( blockPage >> 16 ) & 0xff );

  NF_WAIT_RB();

  // 往本页的 main 区里写入 buffer 里的内容
  for(i=0 ; i<NF_MAINSIZE ; i++)
    {
      buffer[i] = NF_RDDATA8();
    }

  NF_MainECCLock(); // 锁定 main 区域的 ECC 校验码
  NF_SpareECCUnlock(); // 解锁本页 spare 区域的 ECC 校验  

  // 读 spare 区域的前4个字节，即 main 区域的 ECC 校验码部分
  for(i=0 ; i<4 ; i++)
    {
      ECCbuf[i] = NF_RDDATA8();
    }

  NF_SpareECCLock(); // 锁定 spare 区域的 ECC 校验码

  // 读 spare 区域的第5，第6个字节，即 spare 区域的 ECC 校验码部分
  for(i=4 ; i<6 ; i++)
    {
      ECCbuf[i] = NF_RDDATA8();
    }

  
  /* 下面就是验证 ECC 校验码了 */

  if(( ECCbuf[0] == (NFMECCD0 & 0xff)) && 
     ( ECCbuf[1] == ((NFMECCD0 >> 16) & 0xff)) &&
     ( ECCbuf[2] == (NFMECCD1 & 0xff)) &&
     ( ECCbuf[3] == ((NFMECCD1 >> 16) & 0xff)) &&
     ( ECCbuf[4] == (NFSECCD & 0xff)) &&
     ( ECCbuf[5] == ((NFSECCD >> 16) & 0xff)))
    { // ECC 校验全部成功
      NF_nFCE_H();
      return 1;
    }
  else
    { // ECC 校验不成功
      NF_nFCE_H();
      return 0;
    }
}


/* Function: 查看指定的块是否是坏块
   参数：block 要查看的块号
   返回值：0 不是坏块；1 是坏块 */

unsigned int NF_IsBadBlock(unsigned int block)
{
  int i;
  unsigned int blockPage = (block << 5);
  unsigned short markLen = 7;
  BYTE tmp; // 暂存一个字节

  NF_nFCE_L();
  NF_CLEAR_RB();
  NF_CMD( NF_CMD_RDS );
  
  NF_ADDR( 0 ); // 从spare的第一个列（字节）开始读
  NF_ADDR( blockPage & 0xff ); // 这三行代码指明页号
  NF_ADDR( ( blockPage >> 8 ) & 0xff );
  NF_ADDR( ( blockPage >> 16 ) & 0xff );  

  NF_WAIT_RB();

  for(i=0 ; i<markLen ; i++)
    {
      tmp = NF_RDDATA8();
      if(tmp != 0x00) // 有一个字节不为 0x00 就不是坏块
	{
	  return 0;
	}
    }
  
  // markLen 个字节都是 0x00 就是坏块
  return 1;
}
