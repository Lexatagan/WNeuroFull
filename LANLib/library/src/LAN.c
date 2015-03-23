/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "LAN.h"
#include "Functions.h"
#if defined LAN_USE_NRFTRANCIEVER
  #include "NRF24Tranciever.h"
#endif
#if defined LAN_USE_UARTTRANCIEVER
  #include "UARTTransceiver.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t LocalLANAddr[LAN_ADDR_LENGTH];
uint8_t *PLocalLANAddr = LocalLANAddr;
uint8_t UpStream[STREAM_LENGTH];
uint8_t *PUpStream    = UpStream;
uint8_t DownStream[STREAM_LENGTH];
uint8_t *PDownStream  = DownStream;
uint16_t LAN_Error;

uint8_t StackStartPointer = 0;
uint8_t StackEndPointer = 0;
bool StackOverflow = FALSE;
uint8_t LAN_Stack[LAN_PACKET_LENGTH * STACK_LENGTH];

uint8_t ActualPacket[LAN_PACKET_LENGTH];
bool ExecutionOK;
TLAN_Errors LAN_ErrorType;

bool NRF24_Presence = FALSE;
bool UART_Presence = FALSE;

/* Private function prototypes -----------------------------------------------*/
void LAN_CopyPacket(uint8_t *copy, uint8_t *orig);
void ParsePacket(uint8_t *Packet);
void ParseR0(uint8_t *Packet);
void SetUpDownLANAddr(uint8_t *Path);

void LAN_ExecutePacket(uint8_t *Packet);
void LAN_SendPacket(uint8_t *LANAddr, uint8_t *Packet);
void LAN_AssemblePacket(uint8_t *Dir, uint8_t *FuncAddr, uint8_t *Data);
void LAN_InkError(TLAN_Errors ErrorType);
void LAN_BuildReportErrorPacket(uint8_t *Error, uint8_t *Note, uint8_t Qnt);

void LAN_ClearPacket(uint8_t *Data){
uint8_t i;
  for (i = 0; i < LAN_PACKET_LENGTH; i++)
    *(Data++) = 0;
}

bool LAN_CompareNBytes(uint8_t *p1, uint8_t *p2, uint8_t N){
uint8_t i;
  for(i = 0; i < N; i++)
    if (*(p1++) != *(p2++))
      return FALSE;
  return TRUE;
}

void LAN_CopyNBytes(uint8_t *copy, uint8_t *orig, uint8_t N){
uint8_t i;
  for(i = 0; i < N; i++)
    *(copy++) = *(orig++);
}

void LAN_Init(void){
  LAN_CopyNBytes(PLocalLANAddr, DEFAULT_LOCAL_LAN_ADDR, LAN_ADDR_LENGTH);

#if defined LAN_USE_NRFTRANCIEVER
  NRF24_Presence = NRF24_TransceiverInit(PLocalLANAddr);
#endif
#if defined LAN_USE_UARTTRANCIEVER
  UART_Presence = UART_TransceiverInit(PLocalLANAddr);
#endif
}

void ParsePacket(uint8_t *Packet){
uint8_t *FuncAddrAddress = Packet + FUNC_OFFSET;
uint8_t *CommandAddress = Packet + DATA_OFFSET;
  
  if (LAN_CompareNBytes(FuncAddrAddress, FUNC_R0, FUNC_LENGTH))
    ParseR0(Packet);
  
  /*if (LAN_CompareNBytes(FuncAddrAddress, FUNC_L1, FUNC_LENGTH))
    FuncParseL1(CommandAddress);
  if (LAN_CompareNBytes(FuncAddrAddress, FUNC_L2, FUNC_LENGTH))
    FuncParseL2(CommandAddress);
  if (LAN_CompareNBytes(FuncAddrAddress, FUNC_CL, FUNC_LENGTH))
    FuncParseCL(CommandAddress);
  if (LAN_CompareNBytes(FuncAddrAddress, FUNC_FR, FUNC_LENGTH))
    FuncParseFR(CommandAddress);
  if (LAN_CompareNBytes(FuncAddrAddress, FUNC_FG, FUNC_LENGTH))
    FuncParseFG(CommandAddress);
  if (LAN_CompareNBytes(FuncAddrAddress, FUNC_FB, FUNC_LENGTH))
    FuncParseFB(CommandAddress);
  if (LAN_CompareNBytes(FuncAddrAddress, FUNC_SQ, FUNC_LENGTH))
    FuncParseSQ(CommandAddress);*/
  if (LAN_CompareNBytes(FuncAddrAddress, FUNC_DO, FUNC_LENGTH))
    FuncParseL1(CommandAddress);
}

void ParseR0(uint8_t *Packet){
  if (LAN_CompareNBytes(Packet + DATA_OFFSET, R0_SET_BRIDGE, R0_TYPE_LENGTH))
  {
    SetUpDownLANAddr(Packet + DATA_OFFSET + R0_TYPE_LENGTH);
    if (!LAN_CompareNBytes(PUpStream, LOCAL_STREAM, STREAM_LENGTH))
      LAN_SendPacket(PUpStream, Packet);
  }
}

void SetUpDownLANAddr(uint8_t *Path){
uint8_t i;
  for (i = 1; i < BRIDGE_LENGTH; i++)                                           //LANAddr with index '0' belongs to Host
    if (LAN_CompareNBytes(Path + i * STREAM_LENGTH, PLocalLANAddr, LAN_ADDR_LENGTH))
    {
      if (i == (BRIDGE_LENGTH - 1))
        LAN_CopyNBytes(PUpStream, LOCAL_STREAM, STREAM_LENGTH);
      else
        LAN_CopyNBytes(PUpStream, Path + (i + 1) * STREAM_LENGTH, STREAM_LENGTH);
      LAN_CopyNBytes(PDownStream, Path + (i - 1) * STREAM_LENGTH, STREAM_LENGTH);
    }
}

/*void LAN_Test(void){
  if (LAN_CheckRFActivity())
    DBG_Led_TurnON(LED1);
  else
    DBG_Led_TurnOFF(LED1);
}*/

/*bool LAN_CheckRFActivity(void){
vu16 i;
uint8_t Cnt = 0;
  while (Cnt < 10)
  {
    if (hal_nrf_get_carrier_detect())
      Cnt = 0;
    else
      Cnt++;
    DBG_PAUSE(300);                                                            //1800 - 300us
  }
  return TRUE;
/*u16 t = 1200;
  while(t--)
  {
    if (hal_nrf_get_carrier_detect())
       DBG_PIN2_PULSE(10);
  }
}*/

uint8_t LAN_PushPacketToStack(uint8_t *Packet){
  if ((StackEndPointer == StackStartPointer) && StackOverflow)
  {
    LAN_ReportError(STACK_FULL, 0);
    return 0;
  }
  LAN_CopyNBytes(LAN_Stack + StackEndPointer * LAN_PACKET_LENGTH, Packet, LAN_PACKET_LENGTH);
  //LAN_CopyPacket(LAN_Stack + StackEndPointer * LAN_PACKET_LENGTH, Packet);
  StackEndPointer++;
  if (StackEndPointer == STACK_LENGTH)
  {
    StackEndPointer = 0;
    StackOverflow = TRUE;
  }
  return 1;
}

bool LAN_GetPacketFromStack(uint8_t *PPacket)
{
  if ((StackEndPointer == StackStartPointer) && (!StackOverflow))
    return FALSE;
  LAN_CopyNBytes(PPacket, LAN_Stack + StackStartPointer * LAN_PACKET_LENGTH, LAN_PACKET_LENGTH);
  //LAN_CopyPacket(PPacket, LAN_Stack + StackStartPointer * LAN_PACKET_LENGTH);
  return TRUE;
}

void LAN_ClearPacketFromStack(void)
{
  if ((StackEndPointer == StackStartPointer) && (!StackOverflow))
    return;
  StackStartPointer++;
  if (StackStartPointer == STACK_LENGTH)
  {
    StackStartPointer = 0;
    StackOverflow = FALSE;
  }
  return;
}
                                 
void LAN_PushStack(void)
{
bool Res;
  Res = LAN_GetPacketFromStack(ActualPacket);
  if (Res)
  {
    ExecutionOK = TRUE;
    LAN_ExecutePacket(ActualPacket);
    if (ExecutionOK)
      LAN_ClearPacketFromStack();
  }
}

void LAN_ExecutePacket(uint8_t *Packet){
#ifdef LED1_EXECUTEPACKET
  DBG_Led_TurnOFF(LED1);
#endif

  if (LAN_CompareNBytes(Packet + DIR_OFFSET, DIR_UPSTREAM, DIR_LENGTH))
    if (LAN_CompareNBytes(PUpStream, LOCAL_STREAM, STREAM_LENGTH))
      ParsePacket(Packet);
    else
      LAN_SendPacket(PUpStream, Packet);
  
  else if (LAN_CompareNBytes(Packet + DIR_OFFSET, DIR_DOWNSTREAM, DIR_LENGTH))
    LAN_SendPacket(PDownStream, Packet);

  else if (LAN_CompareNBytes(Packet + DIR_OFFSET, DIR_SPECSTREAM, DIR_LENGTH))
    ParsePacket(Packet);
    
#ifdef LED1_EXECUTEPACKET
  DBG_Led_TurnON(LED1);
#endif
}

void LAN_SendPacket(uint8_t *PStream, uint8_t *Packet){
  if (LAN_CompareNBytes(PStream + LAN_ADDR_LENGTH, UART_INTERFACE, INTERFACE_LENGTH))
  {
#ifdef LAN_USE_UARTTRANCIEVER
    if (!UART_SendPacket(PStream, Packet))
#endif
      ExecutionOK = FALSE;
  }
  if (LAN_CompareNBytes(PStream + LAN_ADDR_LENGTH, RF_INTERFACE, INTERFACE_LENGTH))
  {
#ifdef LAN_USE_NRFTRANCIEVER
    if (!NRF24_SendPacket(PStream, Packet))
#endif
      ExecutionOK = FALSE;
  }
  else
    LAN_ReportError(UNKNOWN_ADDR_PREFIX, PStream);
}

void LAN_InkError(TLAN_Errors ErrorType){
  LAN_Error++;
  LAN_ErrorType = ErrorType;
}

void LAN_ReportError(TLAN_Errors Error, uint8_t *Note)
{
/*
  if (Error == NO_RESPONSE)
    LAN_BuildReportErrorPacket(R0_RE_NORESPONSE, Note, LAN_ADDR_LENGTH);
  if (Error == UNKNOWN_ADDR_PREFIX)
    LAN_BuildReportErrorPacket(R0_RE_UNKNOWNPREFIX, Note, INTERFACE_LENGTH);
  if (Error == STACK_FULL)
  {
  }
  LAN_InkError(Error);*/
}

void LAN_BuildReportErrorPacket(uint8_t *Error, uint8_t *Note, uint8_t Qnt){
//uint8_t ErrorPacket[LAN_PACKET_LENGTH] = {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'};
uint8_t ErrorPacket[LAN_PACKET_LENGTH];
uint8_t *PErrorPacket = ErrorPacket;
  LAN_CopyNBytes(PErrorPacket, DIR_DOWNSTREAM, DIR_LENGTH);
  LAN_CopyNBytes(PErrorPacket + FUNC_OFFSET, FUNC_R0, FUNC_LENGTH);
  LAN_CopyNBytes(PErrorPacket + DATA_OFFSET, R0_REPORT_ERROR, R0_TYPE_LENGTH);
  LAN_CopyNBytes(PErrorPacket + DATA_OFFSET + R0_TYPE_LENGTH, Error, R0_RE_TYPE_LENGTH);
  LAN_CopyNBytes(PErrorPacket + DATA_OFFSET + R0_TYPE_LENGTH + R0_RE_TYPE_LENGTH, Note, Qnt);
  LAN_CopyNBytes(PErrorPacket + DOWN_LAN_ADDR_OFFSET, PLocalLANAddr, LAN_ADDR_LENGTH);
  LAN_PushPacketToStack(PErrorPacket);
}
/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
