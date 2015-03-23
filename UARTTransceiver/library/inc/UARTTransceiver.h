/******************** (C) COPYRIGHT 2015 LX ltd. ******************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_TRANSCEIVER
#define __UART_TRANSCEIVER

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define UART_MSG_PREFIX                                 "->"
#define UART_MSG_PREFIX_LENGTH                          2
#define UART_MSG_LENGTH                                 UART_MSG_PREFIX_LENGTH + LAN_PACKET_LENGTH


#define UART_RCC_Periph_UART                            RCC_APB2Periph_USART1
#define UART                                            USART1
#define UART_RCC_Periph_PORT                            RCC_APB2Periph_GPIOA
#define UART_PORT                                       GPIOA
#define UART_TX                                         GPIO_Pin_9
#define UART_RX                                         GPIO_Pin_10

#define UART_IRQChannel                                 USART1_IRQn

/*Exported macro -------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
bool UART_TransceiverInit(uint8_t *PLANAddr);
uint8_t UART_SendPacket(uint8_t *Packet, uint8_t *LAN_Addr);
void UART_IRQ_Callback(void);

/* External variables --------------------------------------------------------*/

#endif   /* __UART_TRANSCEIVER */
/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
