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
