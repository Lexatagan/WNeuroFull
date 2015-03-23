/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FuncParseL1(u8 *Data);
void FuncParseL2(u8 *Data);
void FuncParseCL(u8 *Data);
void FuncParseFR(u8 *Data);
void FuncParseFG(u8 *Data);
void FuncParseFB(u8 *Data);
void FuncParseSQ(u8 *Data);
void FuncParseDO(u8 *Data);
void DIO_Init(void);

/* External variables --------------------------------------------------------*/
#endif   /* __FUNCTIONS_H */
