#ifndef __SYS_MM_H__
#define __SYS_MM_H__

#include "../include/global.h"


// 表示 mm 是否有变动过
int SYS_MM_IS_APPLY;

#define SYS_MM_APPLY 1
#define SYS_MM_UNAPPLY 0


// 改变量定义于 sys_mm.h，用于指向操作系统使用的虚拟内存映射页表
BYTE* SYS_MM_PAGETABLE;

extern void sys_mm_setup();

extern void sys_mm_run();

extern void sys_mm_setup_pagetable( BYTE* pagetable_base_addr );

#endif
