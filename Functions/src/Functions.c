/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "DBG.h"
#include "Functions.h"
#include "LCD.h"
#include "LAN.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/    
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

u16 Cnt;

void FuncParseL1(u8 *Data)
{
  if (*(Data) == '0')
    DBG_Led_TurnOFF(LED1);
  if (*(Data) == '1')
    DBG_Led_TurnON(LED1);
}

void FuncParseL2(u8 *Data)
{
  if (*(Data) == '0')
    DBG_Led_TurnOFF(LED2);
  if (*(Data) == '1')
    DBG_Led_TurnON(LED2);
}

void FuncParseCL(u8 *Data)
{
  LCD_setColor(White);
  DBG_PrintDEC_XXXX(Cnt);
  DBG_Print(":");
  DBG_PrintNCharsCR(Data, 4);
  Cnt++;
}

void FuncParseSQ(u8 *Data)
{
static u8 x, y;
  LCD_setRGBColor(*(Data), *(Data + 1), *(Data + 2));
  LCD_SetPixel(x + 60, y + 60);
  x++;
  if (x == 128)
  {
    x = 0;
    y++;
    if (y == 128)
      y = 0;
  }
  //LAN_PreparePacketToSend(DownRFAddr, "Dd2 CS:Answer*");  
}

void FuncParseFR(u8 *Data)
{
  LCD_setRGBColor(*(Data), 0, 0);
  LCD_Rectangle(9, 40, 69, 140);
}

void FuncParseFG(u8 *Data)
{
  LCD_setRGBColor(0, *(Data), 0);
  LCD_Rectangle(89, 40, 149, 140);
}

void FuncParseFB(u8 *Data)
{
  LCD_setRGBColor(0, 0, *(Data));
  LCD_Rectangle(169, 40, 229, 140);
}

void DIO_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable peripheral clock on APB2 bus */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_DIO, ENABLE);  

  /* Configure DIO pins */
  /*GPIO_InitStructure.GPIO_Pin = DO0 | DO1 | DO2 | DO3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DIO_PORT, &GPIO_InitStructure);*/
}

void FuncParseDO(u8 *Data)
{
BitAction State;
u16 Pin;
  if (*Data == 'R')
    State = Bit_SET;
  if (*Data == 'S')
    State = Bit_RESET;
  Pin = 1 << ((*(Data + 1)) - 0x30 + 11);
  //GPIO_WriteBit(DIO_PORT, Pin, State);
}


