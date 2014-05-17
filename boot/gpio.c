/*******************************************************************************/
/* gpio.c: gpio configure file for Samsung S3C2440                             */
/*******************************************************************************/


/* Define register address about gpio */

#define GPA_CON (*(volatile unsigned long*)0x56000000) // GPA control register address
#define GPB_CON (*(volatile unsigned long*)0x56000010) // GPB control register address
#define GPC_CON (*(volatile unsigned long*)0x56000020) // GPC control register address
#define GPD_CON (*(volatile unsigned long*)0x56000030) // GPD control register address
#define GPE_CON (*(volatile unsigned long*)0x56000040) // GPE control register address
#define GPF_CON (*(volatile unsigned long*)0x56000050) // GPF control register address
#define GPG_CON (*(volatile unsigned long*)0x56000060) // GPG control register address
#define GPH_CON (*(volatile unsigned long*)0x56000070) // GPH control register address
#define GPJ_CON (*(volatile unsigned long*)0x560000D0) // GPJ control register address

#define GPA_DAT (*(volatile unsigned long*)(GPA_CON + 0x04)) // GPA data register
#define GPB_DAT (*(volatile unsigned long*)(GPB_CON + 0x04)) // GPB data register
#define GPC_DAT (*(volatile unsigned long*)(GPC_CON + 0x04)) // GPC data register
#define GPD_DAT (*(volatile unsigned long*)(GPD_CON + 0x04)) // GPD data register
#define GPE_DAT (*(volatile unsigned long*)(GPE_CON + 0x04)) // GPE data register
#define GPF_DAT (*(volatile unsigned long*)(GPF_CON + 0x04)) // GPF data register
#define GPG_DAT (*(volatile unsigned long*)(GPG_CON + 0x04)) // GPG data register
#define GPH_DAT (*(volatile unsigned long*)(GPH_CON + 0x04)) // GPH data register
#define GPJ_DAT (*(volatile unsigned long*)(GPJ_CON + 0x04)) // GPJ data register

// GPA 没有上拉使能寄存器

#define GPB_UP (*(volatile unsigned long*)(GPB_CON + 0x08)) // GPB pull-up disable register
#define GPC_UP (*(volatile unsigned long*)(GPC_CON + 0x08)) // GPC pull-up disable register
#define GPD_UP (*(volatile unsigned long*)(GPD_CON + 0x08)) // GPD pull-up disable register
#define GPE_UP (*(volatile unsigned long*)(GPE_CON + 0x08)) // GPE pull-up disable register
#define GPF_UP (*(volatile unsigned long*)(GPF_CON + 0x08)) // GPF pull-up disable register
#define GPG_UP (*(volatile unsigned long*)(GPG_CON + 0x08)) // GPG pull-up disable register
#define GPH_UP (*(volatile unsigned long*)(GPH_CON + 0x08)) // GPH pull-up disable register
#define GPJ_UP (*(volatile unsigned long*)(GPJ_CON + 0x08)) // GPJ pull-up disable register


/* Values of gpio register */

#define GPACON_Val 0x000003FF // 手册P191，这里相当于将GPA0～GPA9这些引脚作为ADDR0，ADDR16～ADDR24的功能引脚，而不是作为输出引脚

#define GPBCON_Val 0x00000000
#define GPBUP_Val 0x00000000 // 清0代表使能附加上拉功能到相应端口管脚

#define GPCCON_Val 0x00000000
#define GPCUP_Val 0x00000000

#define GPDCON_Val 0x00000000
#define GPDUP_Val 0x00000000

#define GPECON_Val 0x00000000
#define GPEUP_Val 0x00000000

#define GPFCON_Val 0x00000000
#define GPFUP_Val 0x00000000

#define GPGCON_Val 0x00000000
#define GPGUP_Val 0x00000000

#define GPHCON_Val 0x00000000
#define GPHUP_Val 0x00000000

#define GPJCON_Val 0x00000000
#define GPJUP_Val 0x00000000


/* Function: Setup gpio for S3C2440 */

void GPIO_init()
{
  GPA_CON = GPACON_Val;

  GPB_CON = GPBCON_Val;
  GPB_UP = GPBUP_Val;

  GPC_CON = GPCCON_Val;
  GPC_UP = GPCUP_Val;

  GPD_CON = GPDCON_Val;
  GPD_UP = GPDUP_Val;

  GPE_CON = GPECON_Val;
  GPE_UP = GPEUP_Val;

  GPF_CON = GPFCON_Val;
  GPF_UP = GPFUP_Val;

  GPG_CON = GPGCON_Val;
  GPG_UP = GPGUP_Val;

  GPH_CON = GPHCON_Val;
  GPH_UP = GPHUP_Val;

  GPJ_CON = GPJCON_Val;
  GPJ_UP = GPJUP_Val;
}
