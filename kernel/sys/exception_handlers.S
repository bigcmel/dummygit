/*
  该文件包含了除 Reset_Handler 以外的所有异常处理函数，
  Reset_Handler 在 startup.S 中定义了
*/

	.TEXT
	.ARM

	.GLOBL	Undef_Handler
	.GLOBL	SWI_Handler
	.GLOBL	PAbt_Handler
	.GLOBL	DAbt_Handler
	.GLOBL	IRQ_Handler
	.GLOBL	FIQ_Handler

	
/* Function: 未定义的指令 */

Undef_Handler:	


/* Function: 软件中断 */

SWI_Handler:	


/* Function: 指令预取中止 */

PAbt_Handler:	


/* Function: 数据访问中止 */

DAbt_Handler:


/* Function: 外部中断请求 */

IRQ_Handler:	


/* Function: 快速中断请求 */

FIQ_Handler:
	
