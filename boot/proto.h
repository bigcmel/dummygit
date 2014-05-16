/*
proto.h: 该文件包含了供源文件调用的函数声明，有了这些声明，源文件才能调用这些函数
*/

#ifndef __PROTO_H__
#define __PROTO_H__

// wdog.c:
extern void wdog_setup();

// clock,c:
extern void clock_setup();

// gpio.c:
extern void gpio_setup();

// memory.c:
extern void memory_setup();

//nand_flash.c:
extern void nand_flash_setup();


#endif

