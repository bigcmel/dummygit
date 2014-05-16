/*******************************************************************************/
/* memory.c: memory controller configure file for Samsung S3C2440              */
/*******************************************************************************/


/* Define register address about memory controller */

#define MC_BASE (*(volatile unsigned long*)0x48000000) // memory controller base address
#define BWSCON (*(volatile unsigned long*)(MC_BASE + 0x00)) // Bus width and wait status ctrl
#define BANKCON0 (*(volatile unsigned long*)(MC_BASE + 0x04)) // Bank 0 control register
#define BANKCON1 (*(volatile unsigned long*)(MC_BASE + 0x08)) // Bank 1 control register
#define BANKCON2 (*(volatile unsigned long*)(MC_BASE + 0x0C)) // Bank 2 control register
#define BANKCON3 (*(volatile unsigned long*)(MC_BASE + 0x10)) // Bank 3 control register
#define BANKCON4 (*(volatile unsigned long*)(MC_BASE + 0x14)) // Bank 4 control register
#define BANKCON5 (*(volatile unsigned long*)(MC_BASE + 0x18)) // Bank 5 control register
#define BANKCON6 (*(volatile unsigned long*)(MC_BASE + 0x1C)) // Bank 6 control register
#define BANKCON7 (*(volatile unsigned long*)(MC_BASE + 0x20)) // Bank 7 control register
#define REFRESH (*(volatile unsigned long*)(MC_BASE + 0x24)) // SDRAM refresh control register
#define BANKSIZE (*(volatile unsigned long*)(MC_BASE + 0x28)) // Flexible bank size register
#define MRSRB6 (*(volatile unsigned long*)(MC_BASE + 0x2C)) // Bank 6 mode register
#define MRSRB7 (*(volatile unsigned long*)(MC_BASE + 0x30)) // Bank 7 mode register


/* Values of memory register */

#define BWSCON_Val 0x22000000
#define BANKCON0_Val 0x00000700
#define BANKCON1_Val 0x00000700
#define BANKCON2_Val 0x00000700
#define BANKCON3_Val 0x00000700
#define BANKCON4_Val 0x00000700
#define BANKCON5_Val 0x00000700
#define BANKCON6_Val 0x00018005
#define BANKCON7_Val 0x00018005
#define REFRESH_Val 0x008404F3
#define BANKSIZE_Val 0x00000032
#define MRSRB6_Val 0x00000020
#define MRSRB7_Val 0x00000020


/* Function: Setup memory controller for S3C2440 */

void memory_setup()
{
  BWSCON = BWSCON_Val;
  BANKCON0 = BANKCON0_Val;
  BANKCON1 = BANKCON1_Val;
  BANKCON2 = BANKCON2_Val;
  BANKCON3 = BANKCON3_Val;
  BANKCON4 = BANKCON4_Val;
  BANKCON5 = BANKCON5_Val;
  BANKCON6 = BANKCON6_Val;
  BANKCON7 = BANKCON7_Val;
  REFRESH = REFRESH_Val;
  BANKSIZE = BANKSIZE_Val;
  MRSRB6 = MRSRB6_Val;
  MRSRB7 = MRSRB7_Val;
}
