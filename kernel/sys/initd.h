#ifndef __INITD_H__
#define __INITD_H__


#include "../include/global.h"


// initd 中进程的最大数量
#define INITD_PROC_NUM 20

// 表示 initd 中是否有应用程序进程
int INITD_IS_EMPTY;

#define INITD_NON_EMPTY 0
#define INITD_EMPTY 1


// 用于描述一个应用程序进程的结构体
typedef struct initd_node
{
  
}INITD_NODE, ptr_initd_node;

// initd 的全局进程表
INITD_NODE INITD_TABLE[INITD_PROC_NUM];



extern void initd_setup();

extern void initd_run();




#endif
