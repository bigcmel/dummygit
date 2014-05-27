#include "proto.h"

void __main()
{

  WT_init();
  
  CLK_init();

  MC_init();

  GPIO_init();

  NF_init();

  LCD_init();
  LCD_EnvidOnOff(1);
  LCD_ClearScr(0x0C0C0C);


  while(1){}

};
