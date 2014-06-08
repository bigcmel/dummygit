	.TEXT
	.ARM

	.GLOBL	_start		@ default entry point
_start:

	.EXTERN	main

		LDR	R0, =main
		BLX	R0

		BX	LR


	.END

