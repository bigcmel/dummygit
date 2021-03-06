#include "proto.h"

#define LOADER_BASE_ADDR 0x33000000 // loader.bin 加载到内存中的地址

#define LOADER_BLOCK 0 // loader.bin 在 nand flash 中所处的块号
#define LOADER_PAGE 8 // loader.bin 在 nand flash 中所处的页号
#define LOADER_PAGE_NUM 24 // loader.bin 所占的页数

BYTE* __main()
{

  /* 变量声明 */

  BYTE* loader_base = (BYTE*)LOADER_BASE_ADDR;
  BYTE* ptr = loader_base;

  WORD i;

  // 获取 nand_flash 的块数，页数，页大小等等信息
  WORD nf_blocknum, nf_pagepblock, nf_mainsize, nf_sparesize;


  /* 各种初始化 */

  WT_init();
  
  CLK_init();

  MC_init();

  GPIO_init();

  NF_init();

  LCD_init();
  LCD_EnvidOnOff(1);
  LCD_ClearScr(0x0C0C0C);


  /* 将 nand_flash 的 0 号块的 8～15 号页的内容（即存储了 loader.bin 的一段程序）
   复制到内存的 LOADER_BASE_ADDR 处
  */

  // 获取 nand_flash 的块数，页数，页大小等等信息

  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  for(i=0 ; i<LOADER_PAGE_NUM ; i++)
    {
      if( NF_ReadPage(LOADER_BLOCK, LOADER_PAGE+i, ptr) )
	{
	  ptr += nf_mainsize;
	}
      else
	{
	  LCD_ClearScr(0xFFFFFF);
	  while(1){}
	}      
    }

  
  return (BYTE*)LOADER_BASE_ADDR;
};
