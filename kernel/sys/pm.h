#ifndef __PM_H__
#define __PM_H__

#include "../include/global.h"
#include "initd.h"
#include "sys_mm.h"
#include "sys_power.h"

// 两个进程：initd，sys_mm
#define PM_PROC_NUM 2

// 第一个 sys 进程的索引值
#define PM_FIRST_SYS_IDX 1


// 在 PM 中的进程的三种状态
#define PM_PROC_STATUS_READY 0
#define PM_PROC_STATUS_RUNNING 1
#define PM_PROC_STATUS_UNREADY 2


// 表示一个 sys 进程是否申请 CPU
#define PM_SYS_APPLY 1


// 定一个结构体，用于描述 PM 进程表中的一项
typedef struct pm_node
{
  unsigned int idx; // 该进程在进程管理表中的索引
  int status; // 描述该进程所处的状态
  PTRFUNC proc_setup_func; // 该进程的初始化函数
}PM_NODE, *ptr_pm_node;

// PM 全局进程表
PM_NODE PM_TABLE[PM_PROC_NUM];

// 进程管理器的令牌，指向当前正在运行的进程
unsigned int PM_TOKEN;

// 进程表中每一项进程对应的索引
#define PM_INITD_IDX 0
#define PM_SYS_MM_IDX 1
#define PM_SLEEP 9

// 指向各个 sys 进程的全局指针
ptr_pm_node pm_ptr_initd;
ptr_pm_node pm_ptr_sys_mm;


// 记录最近发生错误的错误码
int PM_ERR_CODE;

// 各种错误的定义
#define PM_ERR_UND_TOKEN 1
#define PM_ERR_UND_SYS_IDX 2



// 为操作系统安装 pm，并将其初始化
extern void pm_setup();

// pm 的调度算法
extern void pm_scheduling();


#endif
