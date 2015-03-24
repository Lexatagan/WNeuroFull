/******************** (C) COPYRIGHT 2015 LX ltd. ******************************/
/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "LAN.h"
#include "hal_nrf_stm32_hw.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void hal_spi_init(void)
{
  RCC_APB2PeriphClockCmd(NRF_RCC_Periph_SPI, ENABLE);
#if defined STM32F0XX
  RCC_AHBPeriphClockCmd(NRF_RCC_Periph_GPIO, ENABLE);
#elif defined STM32F10X
  RCC_APB2PeriphClockCmd(NRF_RCC_Periph_GPIO, ENABLE);
#endif
  
  /* Configure NRF_SPI pins */

GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = NRF_SPI_CSN | NRF_GPIO_CE;                      //GPIO push-pull out
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
#if defined STM32F0XX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
#elif defined STM32F10X
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
#endif
  GPIO_Init(NRF_SPI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = NRF_SPI_SCK | NRF_SPI_MOSI;                     //AFIO push-pull out
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
#if defined STM32F0XX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
#elif defined STM32F10X
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
#endif
  GPIO_Init(NRF_SPI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = NRF_SPI_MISO;                                   //AFIO no pull in
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
#if defined STM32F0XX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
#elif defined STM32F10X
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
#endif
  GPIO_Init(NRF_SPI_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = NRF_GPIO_IRQ;                                   //GPIO no pull in
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
#if defined STM32F0XX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
#elif defined STM32F10X
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
#endif
  GPIO_Init(NRF_SPI_PORT, &GPIO_InitStructure);
  
SPI_InitTypeDef SPI_InitStructure;

  SPI_StructInit(&SPI_InitStructure);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
  SPI_Init(NRF_SPI, &SPI_InitStructure);
#if defined STM32F0XX
  SPI_RxFIFOThresholdConfig(NRF_SPI, SPI_RxFIFOThreshold_QF);
#endif
  SPI_Cmd(NRF_SPI, ENABLE);
  
  CSN_HIGH();
  
#if defined STM32F0XX
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(NRF_EXTI_PORT_SOURCE, NRF_EXTI_PIN_SOURCE);
#elif defined STM32F10X
  GPIO_EXTILineConfig(NRF_EXTI_PORT_SOURCE, NRF_EXTI_PIN_SOURCE);
#endif

EXTI_InitTypeDef EXTI_InitStructure;

  EXTI_InitStructure.EXTI_Line = NRF_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_ClearITPendingBit(NRF_EXTI_LINE);

NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable EXTI IRQ Channel -------------------------------------------------*/
  NVIC_InitStructure.NVIC_IRQChannel = NRF_NVIC_EXTI_CHANNEL;
#if defined STM32F0XX
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
#elif defined STM32F10X
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
#endif
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

uint8_t hal_nrf_rw(uint8_t value)
{
#if defined STM32F0XX
  SPI_SendData8(NRF_SPI, value);
#elif defined STM32F10X
  SPI_I2S_SendData(NRF_SPI, value);
#endif
  while (SPI_I2S_GetFlagStatus(NRF_SPI, SPI_I2S_FLAG_RXNE) == RESET);
#if defined STM32F0XX
  return SPI_ReceiveData8(NRF_SPI);
#endif
#ifdef STM32F10X
  return SPI_I2S_ReceiveData(NRF_SPI);
#endif
}

/******************* (C) COPYRIGHT 2015 LX ltd **********END OF FILE***********/
