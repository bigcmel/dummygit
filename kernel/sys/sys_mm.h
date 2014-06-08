#ifndef __SYS_MM_H__
#define __SYS_MM_H__

#include "../include/global.h"


// 表示 mm 是否有变动过
int SYS_MM_IS_APPLY;

#define SYS_MM_APPLY 1
#define SYS_MM_UNAPPLY 0


// ttb 表的虚拟地址
#define SYS_MM_VIR_TTB_BASE 0x3E000000
#define SYS_MM_VIR_TTB_LENGTH 4096


// sdram 的物理基地址
#define SYS_MM_PHY_SDRAM_BASE 0x30000000

// SDRAM 的大小，以兆为单位
#define SYS_MM_SDRAM_SIZE 64

// 全局变量，记录了物理地址的每一M的使用情况
HWORD SYS_MM_SDRAM_TABLE[SYS_MM_SDRAM_SIZE];


// 变量定义于 sys_mm.h，用于指向操作系统使用的虚拟内存映射页表
WORD* SYS_MM_PAGETABLE;

extern void sys_mm_setup();

extern void sys_mm_run();

extern void sys_mm_setup_pagetable( BYTE* pagetable_base_addr );

extern WORD sys_mm_apply_sdram(WORD app_idx, WORD vir_addr, WORD length );


#endif
