/*****************************************************************************/
/* S3C2440.S: Startup file kernel for Samsung S3C2440                        */
/*****************************************************************************/

/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs     */

	.EQU	Mode_USR,	0x10
	.EQU	Mode_FIQ,	0x11
	.EQU	Mode_IRQ,	0x12
	.EQU	Mode_SVC,	0x13
	.EQU	Mode_ABT,	0x17
	.EQU	Mode_UND,	0x1B
	.EQU	Mode_SYS,	0x1F

	.EQU	I_BIT,	0x80	@ when I bit is set, IRQ is disabled
	.EQU	F_BIT,	0x40	@ when F bit is set, FIQ is disabled


/*----------------------- Stack and Heap Definitions -------------------------*/

	.EQU	UND_Stack_Size,	0x00000000
	.EQU	SVC_Stack_Size,	0x00000080
	.EQU	ABT_Stack_Size,	0x00000000
	.EQU	FIQ_Stack_Size,	0x00000000
	.EQU	IRQ_Stack_Size,	0x00000080
	.EQU	USR_Stack_Size,	0x00000400

	.EQU	ISR_Stack_Size,	(UND_Stack_Size + SVC_Stack_Size + ABT_Stack_Size + FIQ_Stack_Size + IRQ_Stack_Size)

	.BSS
	.ALIGN	3

Stack_Mem:
	.SPACE   USR_Stack_Size
__initial_sp:
	.SPACE   ISR_Stack_Size
Stack_Top:


/* 	Heap Configuration	*/

	.EQU	Heap_Size,	0x00000000

	.BSS
	.ALIGN	3

__heap_base:
Heap_Mem:
	.SPACE   Heap_Size
__heap_limit:	


/*----------------------- Memory Definitions ---------------------------------*/

	.EQU	VECTORS_TABLE_BASE,	0xFFFF0000	@ 高端异常中断向量表的地址
	.EQU	KERNEL_VIR_BASE,	0x00004000	@ boot.bin 的基地址


/*----------------------- CODE -----------------------------------------------*/

/* Area Definition and Entry Point
@  Startup Code must be linked first at Address at which it expects to run.*/

	.TEXT
	.ARM

	.GLOBL	_start		@ default entry point
_start:	


@ Copy Exception Vectors to Internal RAM ---------------------------------------

                ADR     R8,  Vectors    @ Source
                LDR     R9,  =VECTORS_TABLE_BASE	@ Destination
                LDMIA   R8!, {R0-R7}    @ Load Vectors 
                STMIA   R9!, {R0-R7}    @ Store Vectors 
                LDMIA   R8!, {R0-R7}    @ Load Handler Addresses 
                STMIA   R9!, {R0-R7}    @ Store Handler Addresses

	
@ Setup Stack for each mode ----------------------------------------------------

                LDR     R0, =Stack_Top

@  Enter Undefined Instruction Mode and set its Stack Pointer
		MOV	R1,	#Mode_UND
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #UND_Stack_Size

@  Enter Abort Mode and set its Stack Pointer
                MOV	R1,	#Mode_ABT
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #ABT_Stack_Size

@  Enter FIQ Mode and set its Stack Pointer
                MOV	R1,	#Mode_FIQ
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #FIQ_Stack_Size

@  Enter IRQ Mode and set its Stack Pointer
		MOV	R1,	#Mode_IRQ
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #IRQ_Stack_Size

@  Enter Supervisor Mode and set its Stack Pointer
		MOV	R1,	#Mode_SVC
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #SVC_Stack_Size


@ Enter the C code -------------------------------------------------------------

	.EXTERN __main
                LDR     R0, =__main
                BX	R0	@ __main函数将返回 kernel.bin 的加载地址到 R0

		B	.

	
/* Exception Vectors
@  Mapped to Address 0.
@  Absolute addressing mode must be used.
@  Dummy Handlers are implemented as infinite loops which can be modified.*/

Vectors:
	LDR     PC, Reset_Addr
	LDR     PC, Undef_Addr
	LDR     PC, SWI_Addr
	LDR     PC, PAbt_Addr
	LDR     PC, DAbt_Addr
	NOP
	LDR     PC, IRQ_Addr
	LDR     PC, FIQ_Addr

	
	.EXTERN  Undef_Handler
	.EXTERN  SWI_Handler
	.EXTERN  PAbt_Handler
	.EXTERN  DAbt_Handler
	.EXTERN  IRQ_Handler
	.EXTERN  FIQ_Handler	


Reset_Addr:
	.WORD     Reset_Handler
Undef_Addr:
	.WORD     Undef_Handler
SWI_Addr:
	.WORD     SWI_Handler
PAbt_Addr:
	.WORD     PAbt_Handler
DAbt_Addr:
	.WORD     DAbt_Handler
	.WORD     0                   @ Reserved Address 
IRQ_Addr:
	.WORD     IRQ_Handler
FIQ_Addr:
	.WORD     FIQ_Handler


/* Reset Handler	*/
	.GLOBL  Reset_Handler
Reset_Handler:	

	LDR	PC, =KERNEL_VIR_BASE

	
@ Get User Initial Stack & Heap Address
	.TEXT
	.ARM

	.GLOBL  __user_initial_stackheap
__user_initial_stackheap:	

                LDR     R0, = Heap_Mem
                LDR     R1, = (Stack_Mem + USR_Stack_Size)
                LDR     R2, = (Heap_Mem + Heap_Size)
                LDR    	R3, = Stack_Mem
                BX	LR


@  Enter User Mode and set its Stack Pointer
	.TEXT
	.ARM

	.GLOBL  __set_user_stack
__set_user_stack:
	
                MSR     CPSR_c, #Mode_USR
                MOV     SP, R0
                SUB     SL, SP, #USR_Stack_Size
		BX	LR

	
	.END
