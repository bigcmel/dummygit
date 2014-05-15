/*******************************************************************************/
/* wdog.c: watchdog configure file for Samsung S3C2440                        */
/*******************************************************************************/


/* Define register address about watchdog */

#define WT_BASE (*(volatile unsigned long*)0x53000000) // watchdog timer base address
#define WT_CON (*(volatile unsigned long*)(WT_BASE + 0x00)) //watchdog timer control register address
#define WT_DAT (*(volatile unsigned long*)(WT_BASE + 0x04)) //watchdog timer data register address
#define WT_CNT (*(volatile unsigned long*)(WT_BASE + 0x08)) //watchdog timer count register address


/* Values of watchdog register */

#define WTCON_Val 0x00000000
#define WTDAT_Val 0x00008000


/* Function: Setup watchdog for S3C2440 */

void wdog_setup()
{
  WT_CNT = WTDAT_Val;
  WT_DAT = WTDAT_Val;
  WT_CON = WTCON_Val;
}
