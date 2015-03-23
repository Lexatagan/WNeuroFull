/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "DBG.h"
#ifdef PRINTINGDEVICE_LCD
  #include "LCD.h"
  #include "fonts.h"
#endif
#ifdef PRINTINGDEVICE_UART
  #include "PCComm.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/    
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void UART_PrintChar(u8 data);
void DBG_PrintChar(u8 chr);
u8 HexCharSet[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void DBG_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable peripheral clock on APB2 bus */
  RCC_APB2PeriphClockCmd(DBG_RCC_APB2Periph_PIN | DBG_RCC_APB2Periph_LED, ENABLE);  

  /* Configure DBG pins */
  GPIO_InitStructure.GPIO_Pin = DBG_PIN1 | DBG_PIN2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DBG_PIN_PORT, &GPIO_InitStructure);

  /* Configure LEDs pin */
  GPIO_InitStructure.GPIO_Pin = LED1 | LED2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DBG_LED_PORT, &GPIO_InitStructure);

#ifdef PRINTINGDEVICE_LCD
  LCD_Setup();
  LCD_BackLight(1);
  LCD_setBackColor(Black);
  LCD_setColor(White);
  LCD_setFont((u8*)SmallFont);
  LCD_ChangeOrientation(LCD_PORTRAIT);
  LCD_Clear();
#endif
  
#ifdef PRINTINGDEVICE_UART
  UART_Init();
#endif
}

void DBG_Print(u8 *str)
{
  while(*str)
    DBG_PrintChar(*(str++));
}

void DBG_PrintChar(u8 chr)
{
  if ((chr < ' ') && (chr != '\n') && (chr != '\r'))
    chr = ' ';
#ifdef PRINTINGDEVICE_UART
    UART_PrintChar(chr);
#endif
#ifdef PRINTINGDEVICE_LCD
    LCD_PrintChar(chr);
#endif
}

void DBG_PrintCR(u8 *st)
{
  DBG_Print(st);
  DBG_PrintChar('\n');
  DBG_PrintChar('\r');
}

void DBG_PrintNChars(u8 *str, u8 N)
{
  while (N-- > 0)
    DBG_PrintChar(*(str++));
}

void DBG_PrintNCharsCR(u8 *str, u8 N)
{
  while (N-- > 0)
    DBG_PrintChar(*(str++));
  DBG_PrintChar('\n');
  DBG_PrintChar('\r');
}

void DBG_PrintHEX_XX(u8 N)
{
  DBG_PrintChar(HexCharSet[(N >> 4) & 0x0F]);
  DBG_PrintChar(HexCharSet[N & 0x0F]);
  DBG_PrintChar('h');
}

void DBG_PrintDEC_XX(u8 N)
{
  DBG_PrintChar(HexCharSet[(N / 10) & 0x0F]);
  DBG_PrintChar(HexCharSet[(N % 10) & 0x0F]);
}

void DBG_PrintDEC_XXXX(u16 N)
{
  DBG_PrintChar(HexCharSet[((N / 1000) % 10) & 0x0F]);
  DBG_PrintChar(HexCharSet[((N / 100) % 10) & 0x0F]);
  DBG_PrintChar(HexCharSet[((N / 10) % 10) & 0x0F]);
  DBG_PrintChar(HexCharSet[(N % 10) & 0x0F]);
}

void DBG_PinSet(u16 Pin)
{
  GPIO_WriteBit(DBG_PIN_PORT, Pin, Bit_SET);
}

void DBG_PinReset(u16 Pin)
{
  GPIO_WriteBit(DBG_PIN_PORT, Pin, Bit_RESET);
}

void DBG_PinTogle(u16 Pin)
{
  if (GPIO_ReadOutputDataBit(DBG_PIN_PORT, Pin) == Bit_SET)
    GPIO_WriteBit(DBG_PIN_PORT, Pin, Bit_RESET);
  else
    GPIO_WriteBit(DBG_PIN_PORT, Pin, Bit_SET);
}

void DBG_Led_TurnON(u16 Led)
{
  GPIO_WriteBit(DBG_LED_PORT, Led, Bit_SET);
}

void DBG_Led_TurnOFF(u16 Led)
{
  GPIO_WriteBit(DBG_LED_PORT, Led, Bit_RESET);
}

void DBG_Led_Togle(u16 Led)
{
  if (GPIO_ReadOutputDataBit(DBG_LED_PORT, Led))
    GPIO_WriteBit(DBG_LED_PORT, Led, Bit_RESET);
  else
    GPIO_WriteBit(DBG_LED_PORT, Led, Bit_SET);
}

