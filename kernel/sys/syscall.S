	.TEXT
	.ARM


	.GLOBL __syscall_enter_sve
__syscall_enter_sve:

	SWI	0x0
	BX	LR
	
	
	.GLOBAL __syscall_NF_WritePage
__syscall_NF_WritePage:

	SWI	0x1
	BX	LR

	
	.GLOBAL __syscall_NF_ReadPage
__syscall_NF_ReadPage:

	SWI	0x2
	BX	LR


	
	.END
