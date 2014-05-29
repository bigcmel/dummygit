/*
proto.h: 该文件包含了供源文件调用的函数声明，有了这些声明，源文件才能调用这些函数
*/

#ifndef __PROTO_H__
#define __PROTO_H__

#include "global.h"

// wdog.c:
extern void WT_init();

// clock,c:
extern void CLK_init();

// gpio.c:
extern void GPIO_init();

// memory.c:
extern void MC_init();

/* nand_flash.c:
 注意这里的函数（写，读，擦除，标志坏块）在运行之前全都不检查是否是坏块，
 而且即使操作不成功，也不会标志为坏块，
 不管是检测坏块还是标志坏块都要在函数外部使用 NF_MarkBadBlock 和 NF_IsBadBlock 函数完成
*/
extern void NF_init();
extern void NF_GetBlockPageInfo(unsigned int* nf_blocknum, unsigned int* nf_pagepblock,
				unsigned int* nf_mainsize, unsigned int* nf_sparesize);
extern WORD NF_CheckId(); // 获取厂商ID和设备ID
extern unsigned int NF_ReadPage(unsigned int block,unsigned int page,BYTE* buffer);
extern unsigned int NF_IsBadBlock(unsigned int block); // 判断一个块是不是坏块

/* lcd.c: */
extern void LCD_Init();
extern void LCD_EnvidOnOff(int onoff);
extern void LCD_PutPixel(WORD x, WORD y, WORD color);
extern void LCD_ClearScr(WORD color);


#endif
