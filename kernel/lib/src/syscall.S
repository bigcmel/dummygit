	.TEXT
	.ARM
	
	
	.GLOBAL __syscall_NF_WritePage
__syscall_NF_WritePage:

	SWI	0x1
	BX	LR

	
	.GLOBAL __syscall_NF_ReadPage
__syscall_NF_ReadPage:

	SWI	0x2
	BX	LR


	.GLOBAL __syscall_LCD_ClearScr
__syscall_LCD_ClearScr:	

	SWI	0x3
	BX	LR


	
	.END
