/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/
#ifndef __LANCOMMON_H
#define __LANCOMMON_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* LAN_TransieverStatus bits                                                  */
#define TRV_MODE_MASK                   0x07
 #define TRV_NONCONFIGURED_MODE         0x00
 #define TRV_PRX_MODE                   0x01
 #define TRV_PTX_MODE                   0x02
 #define TRV_PTX_ACKING_MODE            0x03
#define TRV_TX_DS                       0x08
#define TRV_MAX_RT                      0x10
#define TRV_RX_DR                       0x20



#define LAN_ADDR_PREFIX_LENGTH         1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/*void LAN_setTrvModeNotConf(LAN_TransceiverStatus *pstatus);
void LAN_setTrvModePrx(LAN_TransceiverStatus *pstatus);
void LAN_setTrvModePtx(LAN_TransceiverStatus *pstatus);
void LAN_setTrvModePtxAcking(LAN_TransceiverStatus *pstatus);
uint8_t LAN_getTrvMode(LAN_TransceiverStatus *pstatus);

void LAN_setTrvTxDs(LAN_TransceiverStatus *pstatus);
uint8_t LAN_getTrvTxDs(LAN_TransceiverStatus *pstatus);
void LAN_setTrvMaxRt(LAN_TransceiverStatus *pstatus);
uint8_t LAN_getTrvMaxRt(LAN_TransceiverStatus *pstatus);
void LAN_setTrvRxDr(LAN_TransceiverStatus *pstatus);
uint8_t LAN_getTrvRxDr(LAN_TransceiverStatus *pstatus);
*/
uint8_t LAN_GetPacketFromFIFO(uint8_t *Packet);
void LAN_ClearPacketFromFIFO(void);



/* External variables --------------------------------------------------------*/
extern uint8_t *PLocalLANAddr;
extern uint16_t LAN_Error;

#endif   /* __LANCOMMON_H */

/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
