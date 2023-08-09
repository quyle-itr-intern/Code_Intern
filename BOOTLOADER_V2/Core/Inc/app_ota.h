/**
  ******************************************************************************

  OTA For STM32F446RE
  Author:   LVQ
  Updated:  6 August 2023
  Algorithm:

  ******************************************************************************
*/

#ifndef APP_OTA_H
#define APP_OTA_H

#include "bsp_flash.h"
#include "bsp_uart.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

#define TRUE                       1
#define FALSE                      0

/* To change these values you must change it too in NodeMCU code*/
#define APP_OTA_RECIEVE_TIMEOUT_S  5
#define APP_OTA_UPDATE_START       'X'
#define APP_OTA_UPDATE_FW          'U'
#define APP_OTA_CHECK_SIZE_FLASH   'S'
#define APP_OTA_UPDATE_FW_COMPLETE 'Z'
#define APP_OTA_READ_CONFIRM_CHAR  (uint8_t *) "R"
#define APP_OTA_RESEND_DATA        (uint8_t *) "E"
#define APP_OTA_CANCEL_UPDATE      (uint8_t *) "C"

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
app_ota_field_hex_files_t;

typedef enum
{
  STATE_ERRORS = 0U,   /* There is a error */
  STATE_NO_ERRORS,     /* There is no errors */
  STATE_PENDING,       /* Working is pending  */
  STATE_ERRORS_TIMEOUT /* There is a error due to timeout */
} 
app_ota_status_process_t;

typedef struct
{
  uint8_t byte_count;
  uint8_t address[2];
  uint8_t record_type;
  uint8_t data[16];
  uint8_t check_sum;
} 
app_ota_hex_form_data_t;

void app_ota_start_up_bootloader(void);
void app_ota_jump_to_firmware(void);

#endif
