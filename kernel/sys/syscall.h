#ifndef __SYSCALL_H__
#define __SYSCALL_H__


#include "../include/global.h"


extern unsigned int __syscall_NF_WritePage(unsigned int block,unsigned int page,BYTE* buffer);

extern unsigned int __syscall_NF_ReadPage(unsigned int block,unsigned int page,BYTE* buffer);


#endif

