#include "proto.h"

void __main()
{

  wdog_setup();
  
  clock_setup();

  memory_setup();

  gpio_setup();

  nand_flash_setup();


  while(1){}

};
