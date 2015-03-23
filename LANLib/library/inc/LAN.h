/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LAN_H
#define __LAN_H


#if defined (STM32F10X_LD) || defined (STM32F10X_MD) || defined (STM32F10X_HD) || defined (STM32F10X)
  #define STM32F10X
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {FALSE = 0, TRUE = !FALSE} bool;

/*LAN errors                                                                  */
typedef enum {
  NO_RESPONSE,
  UNKNOWN_ADDR_PREFIX,
  STACK_FULL
} TLAN_Errors;

/* Transiever modes                                                 */
typedef enum {
  NON_CONFIGURED,
  PRX,
  PTX,
  ACKING
} LAN_TransceiverModesTypeDef;


/* Exported constants --------------------------------------------------------*/
#ifdef USE_TE_STM32F103
  #define LAN_USE_NRFTRANCIEVER
  #define LAN_USE_UARTTRANCIEVER
  #define DEFAULT_LOCAL_LAN_ADDR        "d1"

#elif USE_HY_STM32MINI
  #define LAN_USE_NRFTRANCIEVER
  #define LAN_USE_UARTTRANCIEVER
  #define DEFAULT_LOCAL_LAN_ADDR        "d2"

#elif USE_NK_STM32F103
  #define LAN_USE_NRFTRANCIEVER
  #define DEFAULT_LOCAL_LAN_ADDR        "d3"

#elif USE_SWTCH10
  #define LAN_USE_NRFTRANCIEVER
  #define DEFAULT_LOCAL_LAN_ADDR        "d2"

#elif USE_WNEURO_MINI
  #define LAN_USE_NRFTRANCIEVER
  #define LAN_USE_UARTTRANCIEVER
  #define DEFAULT_LOCAL_LAN_ADDR        "dm"

#endif

#define LAN_PACKET_LENGTH               32
#define LAN_ADDR_LENGTH                 2
#define BRIDGE_LENGTH                   9
#define STACK_LENGTH                    8

#define DIR_OFFSET                      0
#define DIR_LENGTH                      1
#define DIR_UPSTREAM                    "U" 
#define DIR_DOWNSTREAM                  "D" 
#define DIR_SPECSTREAM                  "S" 

#define LOCAL_STREAM                    "LAD"
#define STREAM_LENGTH                   (INTERFACE_LENGTH + LAN_ADDR_LENGTH)

#define RF_INTERFACE                    "N"
#define UART_INTERFACE                  "U"
#define INTERFACE_LENGTH                1

#define FUNC_OFFSET                     (DIR_OFFSET + DIR_LENGTH)
#define FUNC_LENGTH                     2
#define FUNC_R0                         "R0"
#define FUNC_CL                         "CL"
#define FUNC_L1                         "L1"
#define FUNC_L2                         "L2"
#define FUNC_CL                         "CL"
#define FUNC_FR                         "FR"
#define FUNC_FG                         "FG"
#define FUNC_FB                         "FB"
#define FUNC_SQ                         "SQ"
#define FUNC_DO                         "DO"

#define R0_TYPE_LENGTH                  2                          
#define R0_SET_BRIDGE                   "SB"  
#define R0_REPORT_ERROR                 "RE"
#define R0_RE_TYPE_LENGTH               2
#define R0_RE_NORESPONSE                "NR"
#define R0_RE_UNKNOWNPREFIX             "UP"

#define DATA_OFFSET                     (FUNC_OFFSET + FUNC_LENGTH)
#define DOWN_LAN_ADDR_OFFSET            (LAN_PACKET_LENGTH - LAN_ADDR_LENGTH)

//#define R0_TYPE_OFFSET                  (FUNC_OFFSET + FUNC_LENGTH)
//#define R0_SB_PATH_OFFSET               (SPEC_TYPE_OFFSET + R0_TYPE_LENGTH)

/*Exported macro -------------------------------------------------------------*/
#define CompareSpecPacketType(Pack, PType) ((*(Pack + SPEC_TYPE_OFFSET) == *(PType)) && (*(Pack + SPEC_TYPE_OFFSET + 1) == *(PType + 1)))

//#define IsLANAddrNull(A) ((*(A) == 0) && (*(A + 1) == 0) && (*(A + 2) == 0))
#define AssignFuncAddr(A, B) *(A) = *(B); *(A + 1) = *(B + 1)

#define SetPacketDir(A, B) *(A + DIR_OFFSET) = *(B)
#define SetPacketFuncAddr(A, B) *(A + FUNC_OFFSET) = *(B); *(A + FUNC_OFFSET + 1) = *(B + 1)
#define SetPacketDevLANAddr(A, B) *(A  + DOWN_LAN_ADDR_OFFSET) = *(B); *(A + DOWN_LAN_ADDR_OFFSET + 1) = *(B + 1); *(A + DOWN_LAN_ADDR_OFFSET + 2) = *(B + 2)

/* Exported functions ------------------------------------------------------- */
void LAN_CopyNBytes(uint8_t *copy, uint8_t *orig, uint8_t N);
bool LAN_CompareNBytes(uint8_t *p1, uint8_t *p2, uint8_t N);

void LAN_Init(void);
void LAN_PushStack(void);
bool LAN_GetPacketFromStack(uint8_t *PPacket);
uint8_t LAN_PushPacketToStack(uint8_t *Packet);
void LAN_ClearPacketFromStack(void);
void LAN_ClearPacket(uint8_t *Data);                                                 //Fill *Data with 0's
void LAN_ReportError(TLAN_Errors Error, uint8_t *Note);

//void LAN_Test(void);


/* External variables --------------------------------------------------------*/

#endif   /* __LAN_H */
/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
