#include "include/hardware_proto.h"
#include "sys/pm.h"


// 定义于 startup.S，为用户模式设置栈，并进入用户模式 
extern void __set_user_stack();

void load_user_app_to_initd();

void __main()
{

  /* 变量声明 */

  unsigned int i;


  LCD_ClearScr( 0x909090 ); // 示意进入了内核

  
  
  pm_setup(); // 进程管理器的安装与初始化

  __set_user_stack(); // 这时就相当于进入了用户模式

  load_user_app_to_initd();
  pm_scheduling(); // 开始进程管理的进程调度



}

void load_user_app_to_initd()
{}
