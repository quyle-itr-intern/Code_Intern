/**
 * @file       app_ota.h
 * @copyright  Copyright (C) 2023 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    1.0.0
 * @date       2023-08-14
 * @author     quyle-itr-intern
 *
 * @brief      app ota
 *
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef APP_OTA_H
#define APP_OTA_H

/* Includes ----------------------------------------------------------- */
#include "bsp_flash.h"
#include "bsp_uart.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

/* Public defines ----------------------------------------------------- */
#define TRUE                       1
#define FALSE                      0
#define APP_OTA_RECIEVE_TIMEOUT_S  5
#define APP_OTA_COMMUNICATION      'F'
#define APP_OTA_UPDATE_START       'X'
#define APP_OTA_RECEIVE_DATA       ':'
#define APP_OTA_UPDATE_FW          'U'
#define APP_OTA_CHECK_SIZE_FLASH   'S'
#define APP_OTA_UPDATE_FW_COMPLETE 'Z'
#define APP_OTA_READ_CONFIRM_CHAR  (uint8_t *) "R"
#define APP_OTA_RESEND_DATA        (uint8_t *) "E"
#define APP_OTA_CANCEL_UPDATE      (uint8_t *) "C"

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief field hex files
 */
typedef enum
{
  START_CODE  = 0U, /*  Start code 	  */
  BYTE_COUNT  = 1U, /*  Byte count 	  */
  ADDRESS     = 2U, /*  Address 		  */
  RECORD_TYPE = 3U, /*  Record type 	*/
  DATA        = 4U, /*  Data 			    */
  CHECKSUM    = 5U, /*  Checksum 		  */
  DONE        = 6U  /*  work is done 	*/
} 
app_ota_field_hex_files_t;

/**
 * @brief status process
 */
typedef enum
{
  STATE_ERRORS = 0U,   /* There is a error */
  STATE_NO_ERRORS,     /* There is no errors */
  STATE_PENDING,       /* Working is pending  */
  STATE_ERRORS_TIMEOUT /* There is a error due to timeout */
} 
app_ota_status_process_t;

/**
 * @brief enum function
 */
enum
{
  APP_OTA_READ_FLASH = 1U,    
  APP_OTA_WRITE_FLASH,   
  APP_OTA_EXECUTE_APPLICATION_USER,      
  APP_OTA_GET_BOOTLOADER_VERSION, 
  APP_OTA_GET_DEVICE_INFORMATION,
};

/**
 * @brief struct hex form data
 */
typedef struct
{
  uint8_t byte_count;
  uint8_t address[2];
  uint8_t record_type;
  uint8_t data[16];
  uint8_t check_sum;
} 
app_ota_hex_form_data_t;

/* Public macros ------------------------------------------------------ */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief  start up bootloader
 *
 * @return
 */
void app_ota_start_up_bootloader(void);

/**
 * @brief  jumb to firmware
 *
 * @return
 */
void app_ota_jump_to_firmware(void);

uint8_t app_ota_handle_data_receive(app_ota_hex_form_data_t *hex_data, uint8_t *data, uint16_t size);
void    app_ota_handle_data_receive_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);

#endif

/* End of file -------------------------------------------------------- */