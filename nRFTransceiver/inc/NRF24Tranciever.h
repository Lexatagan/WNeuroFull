/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NRF24TRANSCEIVER_H
#define __NRF24TRANSCEIVER_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
//#define LED1_EXECUTEPACKET
#ifdef STM32FF10X
//#define PIN1_TRANSMITTING
//#define PIN2_IRQPROCESSING
#endif

/** Defines the channel the radio should operate on*/
#define RF_CHANNEL                              0x28
#define RF_POWER                                HAL_NRF_0DBM
/** Defines how many retransmitts that should be performed */
#define RF_RETRANSMITS                          5        
/** Defines the retransmit delay. Should be a multiple of 250. If the 
 * RF_PAYLOAD_LENGTH is larger than 18, a higher retransmitt delay need to
 * be set. This is because both the original package and ACK payload will
 * be of this size. When the ACK payload exeedes 18 byte, it will not be able
 * to recieve the full ACK in the ordinary 250 mikroseconds, so the delay
 * will need to be increased. */
#define RF_RETRANS_DELAY                        500

#define BROADCAST_RF_ADDR                       "BRC"
#define RF_ZONE                                 "N"

#define RF_ACKING_DELAY                         5
#define RF_TX_DELAY                             2
/** Defines the time it takes for the radio to come up to operational mode */
#define RF_POWER_UP_DELAY                       50

/** Defines the payload length the radio should use */
#define RF_PAYLOAD_LENGTH                       LAN_PACKET_LENGTH 
#define RF_ADDR_LENGTH                          (LAN_ADDR_LENGTH + RF_ZONE_LENGTH)
#define RF_ZONE_LENGTH                          1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
bool NRF24_TransceiverInit(uint8_t *PLANAddr);
uint8_t NRF24_SendPacket(uint8_t *PDestLANAddr, uint8_t *PPacket);
void NRF24_IRQ_Callback(void);
void NRF24_StopAcking(void);
void NRF24_test(void);

/* External variables --------------------------------------------------------*/
//extern LAN_TransceiverModesTypeDef NRF24_Status;

#endif /* __NRF24TRANSCEIVER_H */

/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
