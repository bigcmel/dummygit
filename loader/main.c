#include "proto.h"

#define KERNEL_BASE_ADDR 0x30000000 // kernel.bin 加载到内存中的地址

#define KERNEL_BLOCK 1 // kernel.bin 在 nand flash 中所处的块号
#define KERNEL_PAGE 0 // kernel.bin 在 nand flash 中所处的页号
#define KERNEL_PAGE_NUM 32 // kernel.bin 所占的页数

BYTE* __main()
{

  /* 变量声明 */

  BYTE* kernel_base = (BYTE*)KERNEL_BASE_ADDR;
  BYTE* ptr = kernel_base;

  unsigned int i;

  // 获取 nand_flash 的块数，页数，页大小等等信息
  unsigned int nf_blocknum, nf_pagepblock, nf_mainsize, nf_sparesize;


  /* 将 nand_flash 的 0 号块的 8～15 号页的内容（即存储了 kernel.bin 的一段程序）
   复制到内存的 KERNEL_BASE_ADDR 处
  */

  // 获取 nand_flash 的块数，页数，页大小等等信息

  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  for(i=0 ; i<KERNEL_PAGE_NUM ; i++)
    {
      if( NF_ReadPage(KERNEL_BLOCK, KERNEL_PAGE+i, ptr) )
	{
	  ptr += nf_mainsize;
	}
      else
	{
	  LCD_ClearScr(0xFFFFFF);
	  while(1){}
	}      
    }

  
  return (BYTE*)KERNEL_BASE_ADDR;
};
