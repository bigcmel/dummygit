#include "../include/global.h"
#include "../include/hardware_proto.h"


void C_SWI_Handler( WORD number, WORD* reg )
{
  WORD* ptr_param_0 = reg + 0;
  WORD* ptr_param_1 = reg + 1;
  WORD* ptr_param_2 = reg + 2;
  WORD* ptr_param_3 = reg + 3;
  
  switch( number )
    {
    case 0: // 进入特权模式
      __asm__
	(
	 "MOV R1, %0\n"
	 "MSR CPSR_c, R1\n": :
	 "r"(Mode_SVE)
	 );
      break;
    case 1: // 从 nand flash 读
      (*ptr_param_0) = NF_ReadPage( *ptr_param_0, *ptr_param_1, (BYTE*)(*ptr_param_2) );
      break;
    case 2: // 往 nand flash 写
      (*ptr_param_0) = NF_WritePage( *ptr_param_0, *ptr_param_1, (BYTE*)(*ptr_param_2) );
      break;
    default: // 无效的 SWI 号时执行的代码
      break;
    }
}
