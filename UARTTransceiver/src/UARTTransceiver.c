/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "LAN.h"
#include "UARTTransceiver.h"
#include "DBG.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
uint8_t TransmittingData[UART_MSG_LENGTH];
uint8_t *PTransmittingData = TransmittingData;
uint8_t TransmittingDataOffset;

uint8_t MsgPrefix[UART_MSG_PREFIX_LENGTH];
uint8_t *PMsgPrefix = MsgPrefix;
uint8_t RecievingData[LAN_PACKET_LENGTH];
uint8_t *PRecievingData = RecievingData;
uint8_t RecievingDataOffset;

bool Synchronized = FALSE;
/* Extern variables ----------------------------------------------------------*/
LAN_TransceiverModesTypeDef UART_TransceiverStatus = NON_CONFIGURED;

/* Private function prototypes -----------------------------------------------*/
bool UART_SendNextChar(void);

bool UART_TransceiverInit(uint8_t *PLANAddr){
GPIO_InitTypeDef     GPIO_InitStructure;

  /*Enable USART clock*/
  RCC_APB2PeriphClockCmd(UART_RCC_Periph_UART, ENABLE);
#if defined STM32F0XX
  RCC_AHBPeriphClockCmd(UART_RCC_Periph_PORT, ENABLE);
#elif defined STM32F10X
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(UART_RCC_Periph_PORT, ENABLE);
#endif

  /*Configurate TX*/
  GPIO_InitStructure.GPIO_Pin =  UART_TX;
#if defined STM32F0XX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
#elif defined STM32F10X
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                              
#endif
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(UART_PORT, &GPIO_InitStructure);
  
  /*Configurate RX*/
  GPIO_InitStructure.GPIO_Pin = UART_RX;
#if defined STM32F0XX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
#elif defined STM32F10X
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                      
#endif
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(UART_PORT, &GPIO_InitStructure);
  
#if defined STM32F0XX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
#endif

USART_InitTypeDef    USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 38400;                                   //Скорость обмена 38400 бод
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;                   //Длина слова 8 бит
  USART_InitStructure.USART_StopBits = USART_StopBits_1;                        //1 стоп-бит
  USART_InitStructure.USART_Parity = USART_Parity_No ;                          //Без проверки четности
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;               //Включен передатчик UART
  USART_Init(UART, &USART_InitStructure);                                       //Заданные настройки сохраняем в регистрах UART

  USART_ClearITPendingBit(UART, USART_IT_RXNE);
  USART_ClearITPendingBit(UART, USART_IT_TC);
  //USART_ClearITPendingBit(UART, USART_IT_ORE);
  USART_ITConfig(UART, USART_IT_RXNE, ENABLE);                                  //Прерывание по приему
  //USART_ITConfig(UART, USART_IT_TC, ENABLE);                                    
  //USART_ITConfig(UART, USART_IT_ORE, ENABLE);                                   

  /* Enable UART IRQ Channel -------------------------------------------------*/
NVIC_InitTypeDef     NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = UART_IRQChannel;
#if defined STM32F0XX
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
#elif defined STM32F10X
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
#endif

  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_Cmd(UART, ENABLE); //Включаем UART
  UART_TransceiverStatus = PRX;
  return TRUE;
}


uint8_t UART_SendPacket(uint8_t *PDestLANAddr, uint8_t *PPacket)
{                                   //PDestLANAddr is not used in UART because of direct connection
  if (UART_TransceiverStatus == PRX)
  {
    LAN_CopyNBytes(PTransmittingData, UART_MSG_PREFIX, UART_MSG_PREFIX_LENGTH);
    LAN_CopyNBytes(PTransmittingData + UART_MSG_PREFIX_LENGTH, PPacket, LAN_PACKET_LENGTH);
    UART_TransceiverStatus = PTX;
    TransmittingDataOffset = UART_MSG_LENGTH;
    UART_SendNextChar();
    USART_ITConfig(UART, USART_IT_TC, ENABLE);                                    
#ifdef PIN1_TRANSMITTING
    DBG_PinSet(DBG_PIN1);
#endif
    return 1;
  }
  else
    return 0;
}

bool UART_SendNextChar(void)
{
  if (TransmittingDataOffset > 0)
  {
#if defined STM32F0XX  
    UART -> TDR = *(PTransmittingData + UART_MSG_LENGTH - TransmittingDataOffset);//Because TransmittingDataOffset is decrementing
#elif defined STM32F10X
    UART -> DR = *(PTransmittingData + UART_MSG_LENGTH - TransmittingDataOffset);//Because TransmittingDataOffset is decrementing
#endif
    TransmittingDataOffset--;
    return TRUE;
  }
  else
    return FALSE;
}

void UART_IRQ_Callback(void)
{
uint8_t Chr;
#if defined STM32F0XX
  if (UART -> ISR & USART_ISR_TC)                                               //Transmittion complete
#elif defined STM32F10X
  if (UART -> SR & USART_SR_TC)                                                 
#endif
  {
    if (!UART_SendNextChar())
    {
#ifdef PIN1_TRANSMITTING
      DBG_PinReset(DBG_PIN1);
#endif
      UART_TransceiverStatus = PRX;
      USART_ITConfig(UART, USART_IT_TC, DISABLE);                                    
    }

    USART_ClearITPendingBit(UART, USART_IT_TC);
  }
#if defined STM32F0XX                                                           //Recieve buffer is not epty
  if (UART -> ISR & USART_ISR_RXNE)                                                 
#elif defined STM32F10X
  if (UART -> SR & USART_SR_RXNE)                                               
#endif
  {
#if defined STM32F0XX
    Chr = UART -> RDR;
#elif defined STM32F10X
    Chr = UART -> DR;
#endif
    *(PMsgPrefix) = *(PMsgPrefix + 1);
    *(PMsgPrefix + 1) = Chr;

    if (Synchronized/* && (RecievingDataOffset < LAN_PACKET_LENGTH)*/)              //Recieving message
    {
      *(PRecievingData + RecievingDataOffset) = Chr;
      RecievingDataOffset++;
    }

    if (RecievingDataOffset == LAN_PACKET_LENGTH)                                      
    {
      RecievingDataOffset = 0;
      Synchronized = FALSE;
      LAN_PushPacketToStack(PRecievingData);
    }

    if (LAN_CompareNBytes(PMsgPrefix, UART_MSG_PREFIX, UART_MSG_PREFIX_LENGTH) && !Synchronized)        //Synchronize
    {
      Synchronized = TRUE;
      RecievingDataOffset = 0;
    }
    
    USART_ClearITPendingBit(UART, USART_IT_RXNE);
  }
}
