/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/

#include "platform_config.h"
#include "hw_config.h"
#include "TimeMeter.h"
#include "LAN.h"
#include "DBG.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Tst = 1;

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

#ifdef USE_HY_STM32MINI
int main()
{
u32 R;
u8 Rf[3];
  Set_System();
  TMet_Init();
  DBG_Init();
  TMet_Delay(50);
  LAN_Init();
  R = 0;
  hal_nrf_get_address((u8)HAL_NRF_PIPE0, Rf);
  //LAN_PushPacketToFIFO("SR0SBUd1Rd1Rd2Rd3\0\0\0\0\0\0");
  //LAN_PushPacketToFIFO("SR0SBNd3Md2Nd3Ud1\0\0\0\0\0\0");
  while(1){
    R++;
    if (R == 10000)
    {
      //LAN_PushPacketToFIFO("DXXXFF--Test UART message----------------");
      R = 0;
    }
    LAN_PushStack();
  };
}
#endif

#ifdef USE_TE_STM32F103
int main()
{
u32 i;
u32 R;
  Set_System();
  TMet_Init();
  DBG_Init();
  TMet_Delay(50);
  LAN_Init();
  R = 0;
  //LAN_PushPacketToFIFO("SR0SBUd1Nd1Nd3Nd2\0\0\0\0\0\0");
  //LAN_PushPacketToFIFO("SR0SBUd1Nd1Nd2Ed3");
  //LAN_PushPacketToFIFO("SR0SBUd1Nd1Nd2Nd3\0\0\0\0\0\0");
  //LAN_PushPacketToFIFO("SR0SBUd1Nd1NdmLAD\0\0\0\0\0\0");
  //LAN_PushPacketToFIFO("SR0SBxxxNdmNd1LAD\0\0\0\0\0\0");
  //for (i = 0; i < 1000; i++)
  while(1)
  {
    LAN_PushStack();
  }
    while(1){
    //LAN_PushStack();
    R++;
    /*if (R == 1000000)
      LAN_PushPacketToFIFO("UFGxFF--Test UART message----------------");
    if (R == 1000000)
      LAN_PushPacketToFIFO("UFGxFF--Test UART message----------------");
    if (R == 2000000)
    {
      LAN_PushPacketToFIFO("UFG0FF--Test UART message----------------");
      R = 0;
    }*/
    /*if (R == 1000)
      LAN_PushPacketToFIFO("USQzzz--Test UART message----------------");
    if (R == 2000)
      LAN_PushPacketToFIFO("USQ00z--Test UART message----------------");
    if (R == 11000)
      LAN_PushPacketToFIFO("USQ0z0--Test UART message----------------");*/
    /*if (R == 16000)
      LAN_PushPacketToFIFO("DSQz00--Test UART message----------------");
    if (R == 2000000)
      R = 0;*/
    LAN_PushStack();
  };
}
#endif

#ifdef USE_NK_STM32F103
int main()
{
uint8_t i;
  Set_System();
  TMet_Init();
  DBG_Init();
  TMet_Delay(50);
  LAN_Init();
  LAN_PushPacketToStack("SR0SBXXUd3NdmNLAD\0\0\0");
  while(1)
  {
    TMet_Delay(20);
    LAN_PushStack();
    i++;
    
    if (i == 10)
    {
      LAN_PushPacketToStack("UDO1--------------------------");
    }
    if (i == 20)
    {
      i = 0;
      LAN_PushPacketToStack("UDO0--------------------------");
    }
  };
}
#endif

#ifdef USE_SWTCH10
int main()
{
u32 R;
  Set_System();
  TMet_Init();
  //DBG_Init();
  //DIO_Init();
  TMet_Delay(50);
  LAN_Init();
  //LAN_PushPacketToFIFO("SR0SBUd1Nd1Nd2LAD\0\0\0");
  //LAN_PushPacketToFIFO("DXXXFF--Test UART message----------------");
  R = 0;
  while(1){
    R++;
    //for (R = 0; R < 1000000; R++);
    //DBG_Led_TurnON(LED1);
    //for (R = 0; R < 1000000; R++);
    //DBG_Led_TurnOFF(LED1);
    LAN_PushStack();
  };
}
#endif
/*int main()
{
u32 R;
  Set_System();
  TMet_Init();
  DBG_Init();
  TMet_Delay(50);
  LAN_Init();
  //NRF24_TransceiverInit("Nd2");
  R = 0;
  while(1){
    R++;
    if (R == 10000)
    {
      NRF24_SendPacket("Nd2", "UXXXFF--Test UART message----------------");
      R = 0;
    }
    LAN_PushStack();
  };
}*/

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
