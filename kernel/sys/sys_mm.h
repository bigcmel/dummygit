#ifndef __SYS_MM_H__
#define __SYS_MM_H__

#include "../include/global.h"


// 表示 mm 是否有变动过
int SYS_MM_IS_APPLY;

#define SYS_MM_APPLY 1
#define SYS_MM_UNAPPLY 0


extern void sys_mm_setup();

extern void sys_mm_run();

#endif
