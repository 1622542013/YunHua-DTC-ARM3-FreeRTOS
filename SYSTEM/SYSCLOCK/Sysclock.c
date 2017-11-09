#include "Sysclock.h"

/*
f(VCO 时钟) = f(PLL 时钟输入) * (PLLN / PLLM)
f() = f(VCO 时钟) / PLLP
f(USB OTG FS, SDIO, RNG 时钟输出) = f(VCO 时钟) / PLLQ
*/

/*
STM32F4xx 内部时钟16M
VCO 时钟 必须在192M 到 432M 之间
*/

/* 以一下参数配置 ：[ pll = 16M * (336 / 16) / 1 ] == 84M */
#define PLL_usr_M         16
#define PLL_usr_N         336
#define PLL_usr_P         4
#define PLL_usr_Q         7

static uint8_t OscStatus = 0;
static uint8_t SysClockSource = 0;
static void SysClockHSI(void);

/*
  * - 0x00: HSI used as system clock
  * - 0x04: HSE used as system clock
  * - 0x08: PLL used as system clock (PLL P for STM32F446xx devices)
  * - 0x0C: PLL R used as system clock (only for STM32F446xx devices)
  */
void SysClockInit(void)
{   
    SysClockSource = RCC_GetSYSCLKSource();
    
    if(SysClockSource != Osc_Status_PLL)
    {
      SysClockHSI();
      
      OscStatus = HSI;
    }
    else if(SysClockSource == Osc_Status_PLL)
    {
      OscStatus = HSE;
    }
}

static void SysClockHSI(void)
{
    /*测试*/
    /* Disable HSE */
    RCC->CR &=~ ((uint32_t)RCC_CR_HSEON);/*测试*/
    
    RCC->CR |= RCC_CR_HSION;/*使能内部高速时钟*/
    
    while( (RCC->CR & RCC_CR_HSIRDY) == RESET);/* 等待内部时钟稳定，如果内部时钟也不行，认定MCU损坏 */
    
    /* Select regulator voltage output Scale 1 mode */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
      
    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

#if defined(STM32F401xx)
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
#endif /* STM32F401xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F469_479xx)    
    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_usr_M | (PLL_usr_N << 6) | (((PLL_usr_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSI) | (PLL_usr_Q << 24);
#endif /* STM32F40_41xxx || STM32F401xx || STM32F427_437x || STM32F429_439xx || STM32F469_479xx */

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
#if defined(STM32F40_41xxx)     
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
#endif /* STM32F40_41xxx  */

#if defined(STM32F401xx)
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_2WS;
#endif /* STM32F401xx */

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL)
    {
    }
}

uint8_t GetOscStatus(void)
{
  return OscStatus;
}
