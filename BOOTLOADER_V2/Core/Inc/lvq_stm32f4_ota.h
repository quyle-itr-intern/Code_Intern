/**
  ******************************************************************************

  OTA For STM32F446RE
  Author:   LVQ
  Updated:  6 August 2023
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
#define OTA_UPDATE_START        'X'
#define OTA_UPDATE_FW	        'U'
#define OTA_CHECK_SIZE_FLASH	'S'
#define OTA_UPDATE_FW_COMPLETE	'Z'
#define OTA_READ_CONFIRM_CHAR   (uint8_t*) "R"
#define OTA_RESEND_DATA         (uint8_t*) "E"
#define OTA_CANCEL_UPDATE		(uint8_t*) "C"

typedef enum
{
	START_CODE  = 0U, /* Start code 	*/
	BYTE_COUNT  = 1U, /*  Byte count 	*/
	ADDRESS     = 2U, /*  Address 		*/
	RECORD_TYPE = 3U, /* Record type 	*/
	DATA        = 4U, /* Data 			*/
	CHECKSUM    = 5U, /* Checksum 		*/
	DONE        = 6U  /* work is done 	*/
}
field_hex_files_t;

typedef enum
{
	STATE_ERRORS = 0U,    /* There is a error */
	STATE_NO_ERRORS,      /* There is no errors */
	STATE_PENDING,        /* Working is pending  */
	STATE_ERRORS_TIMEOUT  /* There is a error due to timeout */
}
status_process_t;

void start_up_bootloader(void);
void start_up_firmware_update(void);

#endif
