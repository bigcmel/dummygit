/*******************************************************************************/
/* clock.c: clock configure file for Samsung S3C2440                           */
/*******************************************************************************/


/* Define register address about clock */

#define CLK_BASE (*(volatile unsigned long*)0x4C000000) // clock base address
#define LOCKTIME (*(volatile unsigned long*)(CLK_BASE + 0x00)) // PLL lock time count register
#define MPLLCON (*(volatile unsigned long*)(CLK_BASE + 0x04)) // MPLL configuration register
#define UPLLCON (*(volatile unsigned long*)(CLK_BASE + 0x08)) // UPLL configuration register
#define CLK_CON (*(volatile unsigned long*)(CLK_BASE + 0x0C)) // clock generator control register
#define CLK_SLOW (*(volatile unsigned long*)(CLK_BASE + 0x10)) // clock slow control register
#define CLK_DIVN (*(volatile unsigned long*)(CLK_BASE + 0x14)) // divider control register
#define CAM_DIVN (*(volatile unsigned long*)(CLK_BASE + 0x18)) // clock divider register


/* Values of clock register */

#define LOCKTIME_Val 0x0FFF0FFF
#define MPLLCON_Val 0x00043011
#define UPLLCON_Val 0x00038021
#define CLKCON_Val 0x001FFFF0
#define CLKSLOW_Val 0x00000004
#define CLKDIVN_Val 0x0000000F
#define CAMDIVN_Val 0x00000000


/* Function: Setup clock for S3C2440 */

void clock_setup()
{
  LOCKTIME = LOCKTIME_Val;
  CLK_DIVN = CLKDIVN_Val;
  CAM_DIVN = CAMDIVN_Val;
  MPLLCON = MPLLCON_Val;
  UPLLCON = UPLLCON_Val;
  CLK_SLOW = CLKSLOW_Val;
  CLK_CON = CLKCON_Val;
}
