	.include	"include/global.inc"


	.TEXT
	.ARM

LR_BACKUP_ADDR:
	WORD	=0x00000000
	
	.GLOBL  __APP_S_jmp_to_app
__APP_S_jmp_to_app:

		STR	LR, LR_BACKUP_ADDR
		LDR	LR, =APP_RETURN

		MSR     CPSR_c, #Mode_USR	@ 此时进入用户模式，因为要跳入应用程序了	  
	
		MOV	PC, R0 	@ 跳进 app 的代码段
APP_RETURN:

	SWI	0x0	@ 此时还是用户模式，需要使用 SWI 中断回到特权模式
	
		LDR	LR, =LR_BACKUP_ADDR
		BX	LR


	.END
