#include "../../kernel/include/syscall.h"

void main()
{
  WORD i;

  while(1)
    {
      __syscall_LCD_ClearScr( 0x00000000 );
      for(i=0 ; i<100000 ; i++);
      __syscall_LCD_ClearScr( 0xFFFFFFFF );
    }
}
