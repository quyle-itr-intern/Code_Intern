/**
  ******************************************************************************

  OTA For STM32F446RE
  Author:   LVQ
  Updated:  24January 2023
	Algorithm: 

  ******************************************************************************
*/

#ifndef __OTA_H
#define __OTA_H

#include "lvq_stm32f4_flash.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

#define TRUE     1
#define FALSE    0

#define OTA_RECIEVE_TIMEOUT_S   5

/* To change these values you must change it too in NodeMCU code*/
#define OTA_DATA_START_CHAR     'X'
#define OTA_LINE_BREAK_CHAR     'Y'
#define OTA_DATA_END_CHAR       'Z'
#define OTA_READ_CONFIRM_CHAR   (uint8_t*) "R"
#define OTA_CANCEL_UPDATE		(uint8_t*) "C"

typedef void (*LVQ_USART_Callback_t) (uint8_t);

void LVQ_USART_Set_Callback(void* cb);

void LVQ_OTA_Init(void);
void LVQ_OTA_Run(void);

void LVQ_OTA_CharReceived(uint8_t rec);
void LVQ_OTA_CheckUpdate(uint8_t rec);

void LVQ_OTA_SetCallback_UpdateFirmware(void);
void LVQ_OTA_SetCallback_CheckUpdateFirmware(void);

#endif
