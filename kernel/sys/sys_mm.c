#include "sys_mm.h"


static void sys_mm_optimize();


void sys_mm_setup()
{}

void sys_mm_run()
{
  // 姑且就先只做一次内存的管理，优化，有新功能以后再加
  sys_mm_optimize();
}

void sys_mm_setup_pagetable( BYTE* pagetable_base_addr )
{
  WORD ttb_base = (WORD)pagetable_base_addr;

  __asm__
    (
     "MOV R0, #0\n"
     "MCR P15, 0, R0, C7, C7, 0\n" //p205 使无效指令cache和数据cache      
     "MCR P15, 0, R0, C7, C10, 4\n" //p205 清空写缓冲区
     "MCR P15, 0, R0, C8, C7, 0\n" //p189 使无效快表内容

     "MOV R4, %0\n" // 读取页表的基地址到 R4
     "MCR P15, 0, R4, C2, C0, 0\n" // C2 中存储着页表的基地址

     "MCR P15, 0, R0, C1, C0, 0\n": /* 汇编语句部分结束 */ : /* 无输出部分 */
     "r"( ttb_base ) // 输入部分，传入一个参数，以占位符'%0'表示
     );
}

static void sys_mm_optimize()
{}

