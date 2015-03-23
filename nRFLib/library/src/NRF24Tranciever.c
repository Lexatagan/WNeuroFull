/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/
/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "LAN.h"
#include "hal_nrf.h"
#include "TimeMeter.h"
#include "hal_nrf_stm32_hw.h"
#include "NRF24Tranciever.h"
#include "DBG.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LAN_TransceiverModesTypeDef NRF24_Status = NON_CONFIGURED;
uint8_t LocalRfAddr[RF_ADDR_LENGTH];
uint8_t *PLocalRfAddr = LocalRfAddr;

/* Private function prototypes -----------------------------------------------*/
void NRF24_CalculateRfAddr(uint8_t *PLANAddr, uint8_t *PRfAddr);
void NRF24_set_PTX (uint8_t *PRfAddr);
void NRF24_set_PRX (uint8_t *PRfAddr);

bool NRF24_TransceiverInit(uint8_t *PLANAddr)
{
uint8_t RealRfAddr[RF_ADDR_LENGTH];
uint8_t *PRealRfAddr = RealRfAddr;

  LAN_CopyNBytes(PLocalRfAddr, PLANAddr, LAN_ADDR_LENGTH);
  LAN_CopyNBytes(PLocalRfAddr + LAN_ADDR_LENGTH, RF_ZONE, RF_ZONE_LENGTH);

  hal_spi_init();

  hal_nrf_set_rf_channel(RF_CHANNEL);                                           // Operating on static channel 
                                                                                // Defined in radio.h. 
                                                                                // Frequenzy = 
                                                                                //        2400 + RF_CHANNEL

  hal_nrf_close_pipe(HAL_NRF_ALL);                                              // First close all radio pipes
                                                                                // Pipe 0 and 1 open by default
  hal_nrf_open_pipe(HAL_NRF_PIPE0, TRUE);                                       // Then open pipe0, w/autoack
  hal_nrf_set_rx_pload_width((uint8_t)HAL_NRF_PIPE0, RF_PAYLOAD_LENGTH);        // Pipe0 expect 
                                                                                // PAYLOAD_LENGTH byte payload
  hal_nrf_open_pipe(HAL_NRF_PIPE1, TRUE);                                       // Then open pipe1, w/autoack
    hal_nrf_set_rx_pload_width((uint8_t)HAL_NRF_PIPE1, RF_PAYLOAD_LENGTH);           // Pipe1 expect 
                                                                                // PAYLOAD_LENGTH byte payload
  
  hal_nrf_set_address_width(HAL_NRF_AW_3BYTES);                                 // 3 bytes address width
  hal_nrf_set_address(HAL_NRF_PIPE1, BROADCAST_RF_ADDR);                            // Sets recieving address on 
                                                                                // pipe1 - Broadcast address

  hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);                                      // Operates in 16bits CRC mode
  hal_nrf_set_auto_retr(RF_RETRANSMITS, RF_RETRANS_DELAY);                      // Enables auto retransmit.
                                                                                // 15 retrans with 250ms delay

  hal_nrf_set_output_power(RF_POWER);

  hal_nrf_set_power_mode(HAL_NRF_PWR_UP);                                       // Power up device

  hal_nrf_get_clear_irq_flags ();                                               //Reset previous interrupts
  hal_nrf_flush_rx();                                                           //Prevent RX FIFO blocking
  hal_nrf_flush_tx();                                                           //Prevent TX FIFO blocking

  TMet_Delay(RF_POWER_UP_DELAY);                                                // Wait for the radio to power up
  NRF24_set_PRX(PLocalRfAddr);

  hal_nrf_get_address((uint8_t)HAL_NRF_PIPE0, PRealRfAddr);

  if (LAN_CompareNBytes(PLocalRfAddr, PRealRfAddr, RF_ADDR_LENGTH))
    return TRUE;
  else
    return FALSE;
}

void NRF24_set_PTX (uint8_t *PDestRfAddr)
{
volatile uint8_t i;
  CE_LOW();                                                                     // Set Chip Enable (CE) pin low to disable reciever
  
  hal_nrf_set_address(HAL_NRF_TX, PDestRfAddr);                                 // Set reciever's addresses
  hal_nrf_set_address(HAL_NRF_PIPE0, PDestRfAddr);                              // Sets recieving address on 
                                                                                // pipe0 to get Ack 
  hal_nrf_set_operation_mode(HAL_NRF_PTX);                                      // Enter TX mode

  NRF24_Status = PTX;
}         

void NRF24_set_PRX (uint8_t *PRfAddr)
{
volatile uint8_t i;
  hal_nrf_set_address(HAL_NRF_PIPE0, PRfAddr);                                  // Sets recieving address on 
                                                                                // pipe0 - Main device address  
  
  hal_nrf_set_operation_mode(HAL_NRF_PRX);                                      // Enter RX mode
  
  hal_nrf_flush_rx();                                                           // Prevent RX FIFO blocking
  
  CE_HIGH();                                                                    // Set Chip Enable (CE) pin high to enable reciever
  
  NRF24_Status = PRX;
}         

uint8_t NRF24_SendPacket(uint8_t *PDestLANAddr, uint8_t *PPacket)
{
uint8_t DestRfAddr[RF_ADDR_LENGTH];
uint8_t *PDestRfAddr = DestRfAddr;
  LAN_CopyNBytes(PDestRfAddr, PDestLANAddr, LAN_ADDR_LENGTH);
  LAN_CopyNBytes(PDestRfAddr + LAN_ADDR_LENGTH, RF_ZONE, RF_ZONE_LENGTH);
  if (NRF24_Status == PRX)
  {
    NRF24_set_PTX(PDestRfAddr);
    hal_nrf_write_tx_pload(PPacket, RF_PAYLOAD_LENGTH);                         // load message into radio
    CE_PULSE();                                                                 // send packet
#ifdef PIN1_TRANSMITTING
    DBG_PinSet(DBG_PIN1);
#endif
    return 1;
  }
  else
    return 0;
}

/*void NRF24_test(void)
{
  NRF24_set_PTX("Nd1");
  hal_nrf_write_tx_pload("UxO0------------------------------------", 32);        
  CE_PULSE();                                                    
}*/

void NRF24_IRQ_Callback(void)
{
uint8_t Stat;
uint8_t Buf[RF_PAYLOAD_LENGTH];
uint8_t RfAddr[RF_ADDR_LENGTH];
uint8_t *PRfAddr = RfAddr;
  Stat = hal_nrf_get_clear_irq_flags ();
  switch(Stat)
  {
    case (1 << HAL_NRF_RX_DR):                                                  // Packet received
      NRF24_Status = PTX;
      hal_nrf_read_rx_pload(Buf);  
      LAN_PushPacketToStack(Buf);
      TMet_DelayedExecution(NRF24_StopAcking, RF_ACKING_DELAY);
      break;
  
    case (1 << HAL_NRF_MAX_RT):                                                 // Max retries reached
      hal_nrf_flush_tx();                                                       // flush tx fifo, to avoid fifo jam
      hal_nrf_get_address((uint8_t)HAL_NRF_PIPE0, PRfAddr);
      LAN_ReportError(NO_RESPONSE, PRfAddr);
      NRF24_set_PRX(PLocalRfAddr);
#ifdef PIN1_TRANSMITTING
      DBG_PinReset(DBG_PIN1);
#endif
      break;
  
    case (1 << HAL_NRF_TX_DS):                                                  // Packet sent
      NRF24_set_PRX(PLocalRfAddr);
      DBG_PinSet(DBG_PIN1);
      TMet_DelayedExecution(NRF24_StopAcking, RF_TX_DELAY);
#ifdef PIN1_TRANSMITTING
      DBG_PinReset(DBG_PIN1);
#endif
      break;
     
    default:
      break;    
  }
}

/*void NRF24_CalculateRfAddr(uint8_t *PLANAddr, uint8_t *PRfAddr)
{
  LAN_CopyNBytes(PRfAddr, PLANAddr, LAN_ADDR_LENGTH);
  LAN_CopyNBytes(PRfAddr + LAN_ADDR_LENGTH, RF_ZONE, RF_ZONE_LENGTH);
}*/

void NRF24_StopAcking(void)
{
      DBG_PinReset(DBG_PIN1);
  NRF24_Status = PRX; 
}  

