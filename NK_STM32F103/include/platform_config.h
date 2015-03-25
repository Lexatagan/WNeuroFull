/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Define the STM32F10x hardware depending on the used board */

//******************************************************************************
//*   LAN                                                                      *
//******************************************************************************
  #define LAN_USE_NRFTRANCIEVER
  #define LAN_USE_UARTTRANCIEVER
  #define DEFAULT_LOCAL_LAN_ADDR        "d3"

//******************************************************************************
//*   DBG                                                                      *
//******************************************************************************
  #define DBG_RCC_APB2Periph_LED        RCC_APB2Periph_GPIOC
  #define DBG_LED_PORT                  GPIOC 
  #define LED1                          GPIO_Pin_3  
  #define LED2                          GPIO_Pin_2  

//******************************************************************************
//*   nRF21l01                                                                 *
//******************************************************************************
  #define NRF_SPI                       SPI1
  #define NRF_SPI_PORT                  GPIOA  
  #define NRF_RCC_Periph_SPI            RCC_APB2Periph_SPI1
  #define NRF_RCC_Periph_GPIO           RCC_APB2Periph_GPIOA
  #define NRF_SPI_CSN                   GPIO_Pin_1  
  #define NRF_SPI_SCK                   GPIO_Pin_5  
  #define NRF_SPI_MISO                  GPIO_Pin_6  
  #define NRF_SPI_MOSI                  GPIO_Pin_7  
  #define NRF_GPIO_CE                   GPIO_Pin_3  
  #define NRF_GPIO_IRQ                  GPIO_Pin_2
  #define NRF_EXTI_PORT_SOURCE          GPIO_PortSourceGPIOA
  #define NRF_EXTI_PIN_SOURCE           GPIO_PinSource2
  #define NRF_EXTI_LINE                 EXTI_Line2
  #define NRF_NVIC_EXTI_CHANNEL         EXTI2_IRQn

//******************************************************************************
//*   USB                                                                      *
//******************************************************************************
  #define USB_DISCONNECT                      GPIOC  
  #define USB_DISCONNECT_PIN                  GPIO_Pin_9
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOC

//******************************************************************************
//*   hw_config                                                                *
//******************************************************************************
  #define                               ID1          (0x1FFFF7E8)
  #define                               ID2          (0x1FFFF7EC)
  #define                               ID3          (0x1FFFF7F0)

  /*#define RCC_APB2Periph_DIO        RCC_APB2Periph_GPIOB
  #define DIO_PORT                  GPIOB 
  #define DO0                       GPIO_Pin_12  
  #define DO1                       GPIO_Pin_13  
  #define DO2                       GPIO_Pin_14  
  #define DO3                       GPIO_Pin_15*/  
  
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
