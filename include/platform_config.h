/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Define the STM32F10x hardware depending on the used board */

#ifdef USE_HY_STM32MINI
  #define RCC_PLLMUL                RCC_PLLMul_9

#elif USE_TE_STM32F103
  #define RCC_PLLMUL                RCC_PLLMul_9

#elif USE_NK_STM32F103
  #define RCC_PLLMUL                RCC_PLLMul_9

#elif USE_SWTCH10
  #define RCC_PLLMUL                RCC_PLLMul_9

  #define RCC_APB2Periph_DIO        RCC_APB2Periph_GPIOB
  #define DIO_PORT                  GPIOB 
  #define DO0                       GPIO_Pin_12  
  #define DO1                       GPIO_Pin_13  
  #define DO2                       GPIO_Pin_14  
  #define DO3                       GPIO_Pin_15  
  
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
