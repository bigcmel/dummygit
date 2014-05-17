#include "proto.h"

void __main()
{

  WT_init();
  
  CLK_init();

  MC_init();

  GPIO_init();

  NF_init();


  while(1){}

};
