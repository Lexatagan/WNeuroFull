/******************** (C) COPYRIGHT 2013 LX ltd. ******************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NRF_HAL_STM32_HW_H
#define __NRF_HAL_STM32_HW_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#ifdef USE_WNEURO_MINI
  #define NRF_SPI                       SPI1
  #define NRF_SPI_PORT                  GPIOA  
  #define NRF_RCC_Periph_SPI            RCC_APB2Periph_SPI1
  #define NRF_RCC_Periph_GPIO           RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB
  #define NRF_SPI_CSN                   GPIO_Pin_10                             //GPIOA
  #define NRF_SPI_SCK                   GPIO_Pin_5                              //GPIOA
  #define NRF_SPI_MISO                  GPIO_Pin_6                              //GPIOA
  #define NRF_SPI_MOSI                  GPIO_Pin_7                              //GPIOA
  #define NRF_GPIO_CE                   GPIO_Pin_9                              //GPIOA
  #define NRF_GPIO_IRQ                  GPIO_Pin_1                              //GPIOB

  #define NRF_EXTI_PORT_SOURCE          EXTI_PortSourceGPIOB
  #define NRF_EXTI_PIN_SOURCE           EXTI_PinSource1
  #define NRF_EXTI_LINE                 EXTI_Line1
  #define NRF_NVIC_EXTI_CHANNEL         EXTI0_1_IRQn
#endif

#ifdef USE_TE_STM32F103
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
#endif

#ifdef USE_HY_STM32MINI
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
#endif

#ifdef USE_NK_STM32F103
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
#endif

#ifdef USE_SWTCH10
  #define NRF_SPI                       SPI1
  #define NRF_SPI_PORT                  GPIOA  
  #define NRF_RCC_APB2Periph_SPI        RCC_APB2Periph_SPI1
  #define NRF_RCC_APB2Periph_GPIO       RCC_APB2Periph_GPIOA
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
#endif

/* Exported macro ------------------------------------------------------------*/

// Macro that set radio's CSN line LOW.
#define CSN_LOW() GPIO_WriteBit(NRF_SPI_PORT, NRF_SPI_CSN, Bit_RESET)

// Macro that set radio's CSN line HIGH.
#define CSN_HIGH() GPIO_WriteBit(NRF_SPI_PORT, NRF_SPI_CSN, Bit_SET)

// Macro that set radio's CE line LOW.
#define CE_LOW() GPIO_WriteBit(NRF_SPI_PORT, NRF_GPIO_CE, Bit_RESET)

// Macro that set radio's CE line HIGH.
#define CE_HIGH() GPIO_WriteBit(NRF_SPI_PORT, NRF_GPIO_CE, Bit_SET)

// Macro that disables interrupts from nRF24l01.
//#define hal_IRQDisable() EXTI->IMR &= ~0x00000004
//#define hal_IRQDisable()    NVIC->ICER[0] = NVIC_ICER_CLRENA_8

// Macro that enables interrupts from nRF24l01.
//#define hal_IRQEnable() EXTI->IMR |= 0x00000004; if (!GPIO_ReadOutputDataBit(NRF_SPI_PORT, NRF_GPIO_IRQ)) EXTI_GenerateSWInterrupt(NRF_EXTI_LINE);
//#define hal_IRQEnable()    NVIC->ISER[0] = NVIC_ISER_SETENA_8

// Pulses the CE to nRF24L01 for at least 10 us
#define CE_PULSE() volatile uint16_t count; \
  count = 200; \
  CE_HIGH();  \
  while(count--) \
    ; \
  CE_LOW();

//#define RADIO_ACTIVITY() (GPIO_ReadInputDataBit(NRF_SPI_PORT, NRF_GPIO_IRQ) == Bit_SET ? FALSE: TRUE)

/* Exported functions ------------------------------------------------------- */
void hal_spi_init(void);
uint8_t hal_nrf_rw(uint8_t value);

#endif /* __NRF_HAL_STM32_HW_H */

/******************* (C) COPYRIGHT 2013 LX ltd. **********END OF FILE**********/
