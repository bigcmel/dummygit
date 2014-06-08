#include "sys/pm.h"
#include "sys/syscall.h"

/* 这个函数应该是放在外部中断的处理函数里的，
   外部申请加载一个程序的时候调用，
   放在这里只为测试。*/
WORD load_user_app_to_initd( BYTE* app_name, BYTE* app_binary_base, WORD app_binary_length );

void __main()
{

  /* 变量声明 */

  WORD binary_base;
  WORD binary_length;


  // 示意程序到了这里
  LCD_ClearScr( 0x909090 );

  
  // 进程管理器的安装与初始化    
  pm_setup();


  // 载入一个应用程序，名叫 "app_0"

  binary_base = 0x00000000;
  binary_length = 1024;

  load_user_app_to_initd( "app_0", (BYTE*)binary_base, binary_length );


  while(1) // 姑且先这样写了，不停地做调度
    {
      pm_scheduling(); // 开始进程管理的进程调度
    }


}

WORD load_user_app_to_initd( BYTE* app_name, BYTE* app_binary_base, WORD app_binary_length )
{
  WORD status; // 函数执行情况
  WORD i;

  WORD app_idx;

  WORD block;
  WORD page;
  WORD page_num;
  BYTE* buffer;
  
  WORD nf_blocknum;
  WORD nf_pagepblock;
  WORD nf_mainsize;
  WORD nf_sparesize;


  /* 先注册 initd 的进程表 */

  app_binary_base = 0x00000000;
  app_binary_length = 1024;

  app_idx = initd_register_app(app_name, app_binary_base, app_binary_length);


  /* 这里要切换一下上下文，切换到 app_idx 对应的代码断去 */
  
  MMU_SwitchContext( app_idx );

  
  /* 向 sys_mm 申请需要的内存空间 */

  if( ! sys_mm_apply_sdram(app_idx, (WORD)app_binary_base, app_binary_length) )
    return RETURN_FAILED;


  /* 调用系统调用（ SWI 中断），将代码从 SDRAM 读到内存中 */
  
  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  block = 5;
  page = 0;
  page_num = ((INITD_TABLE[app_idx].binary_length - 1) / nf_mainsize) + 1;
  buffer = INITD_TABLE[app_idx].binary_base;

  for(i=0; i<page_num; i++)
    {
      status = NF_WritePage( block, page, buffer );
      page++;
      buffer += nf_mainsize;
      
      if(page == 32)
	{
	  block++;
	  page = 0;
	}
    }

  return RETURN_SUCCESS;
}

// gcc 的静态库要求链接到的函数，为空就好
void raise()
{}
