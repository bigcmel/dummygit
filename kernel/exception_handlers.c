/*
  该文件包含了除 Reset_Handler 以外的所有异常处理函数，
  Reset_Handler 在 startup.S 中定义了
*/


/* Function: 未定义的指令 */

void Undef_Handler(){}


/* Function: 软件中断 */

void SWI_Handler(){}


/* Function: 指令预取中止 */

void PAbt_Handler(){}


/* Function: 数据访问中止 */

void DAbt_Handler(){}


/* Function: 外部中断请求 */

void IRQ_Handler(){}


/* Function: 快速中断请求 */

void FIQ_Handler(){}
