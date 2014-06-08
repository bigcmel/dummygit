#ifndef __GLOBAL_H__
#define __GLOBAL_H__


#define Mode_USR 0x10
#define Mode_FIQ 0x11
#define Mode_IRQ 0x12
#define Mode_SVE 0x13
#define Mode_ABT 0x17
#define Mode_UND 0x1B
#define Mode_SYS 0x1F


#define RETURN_SUCCESS 1
#define RETURN_FAILED 0


typedef unsigned char BYTE;
typedef unsigned short HWORD; // 半字，两个字节
typedef unsigned long WORD; // 字，四个字节


typedef void(*PTRFUNC)(void); // PTRFUNC 是一个指向函数的指针


#endif

