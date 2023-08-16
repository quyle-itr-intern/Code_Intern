/**
 * @file       app_ota.c
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

/* Includes ----------------------------------------------------------- */
#include "app_ota.h"

#include <stdlib.h>

/* Private defines ---------------------------------------------------- */
#define ADDRESS_FIRMWARE_APPLICATION 0x08008000U
#define STM32F446xx                  1
#define USART_UD                     &huart1

#ifdef STM32F401xx
uint32_t size_current   = 0;
uint8_t  index          = 0;
uint32_t max_size_flash = 245760;
#elif STM32F446xx
uint32_t max_size_flash = 507904;
#endif

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
extern UART_HandleTypeDef huart1;

/* Private variables -------------------------------------------------- */
uint8_t           flag_ota_update      = FALSE;
uint8_t           flag_size_flash      = FALSE;
uint8_t           flag_ota_complete    = FALSE;
uint8_t           flag_earse_ok        = FALSE;
uint8_t           flag_ota_check_infor = FALSE;
uint32_t          size_current;
volatile uint32_t size_data_firmware = 0;
volatile uint32_t address_write_flash;
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void app_ota_write_flash_memory(app_ota_hex_form_data_t *hex_data)
{
  uint8_t  data_count, i;
  uint8_t  counter_index       = 0;
  uint16_t data_write_flash[8] = { 0 };

  switch (hex_data->record_type)
  {
  case 4: /* Extended Linear Address Record: used to identify the extended linear address  */
  {
    uint32_t address = 0;
    address |= hex_data->data[0] << 8 | hex_data->data[1];
    /* Address save firmware update */
    address_write_flash = 0x08000000 | (address << 16);
    break;
  }
  case 5: /* Start Linear Address Record: the address where the program starts to run      */
  {
    break;
  }
  case 0: /* Data Rrecord: used to record data, most records of HEX files are data records */
  {
    /* get character count */
    data_count = hex_data->byte_count;
    size_data_firmware += data_count;

    /* set full address */
    address_write_flash = address_write_flash & 0xFFFF0000;
    address_write_flash = address_write_flash | (hex_data->address[0] << 8) | (hex_data->address[1]);

    /* check address wite flash data */
    if (address_write_flash < ADDRESS_FIRMWARE_APPLICATION)
    {
      address_write_flash = 0;
      return;
    }

    /* get the data of the record */
    for (i = 0; i < data_count; i += 2)
    {
      data_write_flash[counter_index] = (hex_data->data[i + 1] << 8) | (hex_data->data[i]);
      counter_index++;
    }

    /* count data old or even */
    if (data_count % 2 != 0)
    {
      data_write_flash[counter_index] = 0xFF00 | (hex_data->data[data_count - 1]);
      bsp_flash_write(address_write_flash, (uint32_t *) data_write_flash, data_count / 4 + 1);
    }
    else
      bsp_flash_write(address_write_flash, (uint32_t *) data_write_flash, data_count / 4);
    break;
  }
  default: break;
  }
}

void app_ota_start_up_bootloader(void)
{
  bsp_uart_init();

  uint32_t time_last = HAL_GetTick();
  /* wait for update firmware */
  while ((HAL_GetTick() - time_last < 3000) || (flag_ota_check_infor == TRUE))
  {
    if ((flag_ota_update == TRUE) && (flag_size_flash == TRUE))
    {
      flag_earse_ok = FALSE;
      /* caculation number sector */
      uint8_t start_sector  = bsp_get_sector_from_address(ADDRESS_FIRMWARE_APPLICATION);
      uint8_t end_sector    = bsp_get_sector_from_address(ADDRESS_FIRMWARE_APPLICATION + size_current);
      uint8_t number_sector = end_sector - start_sector + 1;

      /* erase sector for write data */
      for (uint8_t i = 0; i < number_sector; i++)
      {
        bsp_flash_erase(bsp_get_address_from_sector(start_sector));
        start_sector++;
      }
      flag_earse_ok = TRUE;
      while (flag_ota_complete == FALSE)
      {
      }
    }
  }
}

void app_ota_jump_to_firmware(void)
{
  bsp_uart_deinit_peripheral();
  HAL_RCC_DeInit();

  SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
  __set_MSP(*((volatile uint32_t *) ADDRESS_FIRMWARE_APPLICATION));
  uint32_t jump_address       = *((volatile uint32_t *) (ADDRESS_FIRMWARE_APPLICATION + 4));
  void (*reset_handler)(void) = (void *) jump_address;
  reset_handler();
}

void app_ota_parse_element(uint8_t *des, uint8_t *buff, uint8_t length)
{
  uint8_t count_array = 0U;
  uint8_t buffer_temp[32];

  /* convert data string to data hex and save to des buffer*/
  for (uint8_t count = 0U; count < length; count++)
  {
    if (buff[count] >= '0' && buff[count] <= '9')
      buffer_temp[count] = buff[count] - 48;
    else if (buff[count] >= 'A' && buff[count] <= 'F')
      buffer_temp[count] = buff[count] - 65 + 10;
    else
      return;
    if (count % 2 != 0)
    {
      des[count_array] = ((buffer_temp[count - 1] << 4) | (buffer_temp[count]));
      count_array++;
    }
  }
}

uint8_t app_ota_handle_data_receive(app_ota_hex_form_data_t *hex_data, uint8_t *data, uint16_t size)
{
  uint8_t                   cal_check_sum = 0;
  app_ota_field_hex_files_t state         = START_CODE;
  uint8_t                   buffer_save_data_hex[16];

  /* parse string to hex form data hex */
  for (uint8_t count = 0; count < size; count++)
  {
    switch (state)
    {
    case START_CODE:
    {
      if (data[count] != ':')
        return STATE_ERRORS;
      else
        state = BYTE_COUNT;
      break;
    }
    case BYTE_COUNT:
    {
      app_ota_parse_element(buffer_save_data_hex, &data[count], 2U);
      hex_data->byte_count = buffer_save_data_hex[0];
      count                = 2U;
      state                = ADDRESS;
      break;
    }
    case ADDRESS:
    {
      app_ota_parse_element(buffer_save_data_hex, &data[count], 4U);
      hex_data->address[0] = buffer_save_data_hex[0];
      hex_data->address[1] = buffer_save_data_hex[1];
      count                = 6U;
      state                = RECORD_TYPE;
      break;
    }
    case RECORD_TYPE:
    {
      app_ota_parse_element(buffer_save_data_hex, &data[count], 2U);
      hex_data->record_type = buffer_save_data_hex[0];
      count                 = 8U;
      state                 = DATA;
      break;
    }
    case DATA:
    {
      app_ota_parse_element(buffer_save_data_hex, &data[count], (hex_data->byte_count * 2U));
      for (count = 0; count < hex_data->byte_count; count++)
      {
        hex_data->data[count] = buffer_save_data_hex[count];
      }
      count = 8U + (hex_data->byte_count * 2U);
      state = CHECKSUM;
      break;
    }
    case CHECKSUM:
    {
      app_ota_parse_element(buffer_save_data_hex, &data[count], 2U);
      hex_data->check_sum = buffer_save_data_hex[0];
      state               = DONE;
      break;
    }
    case DONE:
    {
      /* check sum data hex */
      cal_check_sum = hex_data->byte_count + hex_data->address[0] + hex_data->address[1] + hex_data->record_type;
      for (count = 0; count < hex_data->byte_count; count++)
      {
        cal_check_sum += hex_data->data[count];
      }
      cal_check_sum = ~cal_check_sum + 1U;
      if (cal_check_sum == hex_data->check_sum)
        return STATE_NO_ERRORS;
      else
        return STATE_ERRORS;
    }
    default: return STATE_ERRORS;
    }
  }
  return STATE_ERRORS;
}

void app_ota_communication(uint8_t choose, uint8_t *data, uint16_t size)
{
  switch (choose)
  {
  case APP_OTA_READ_FLASH:
  {
    /* code */
    char    TX[20] = { 0 };
    uint8_t data_convert[4];
    app_ota_parse_element(data_convert, &data[4], 8);
    uint32_t address_read  = (data_convert[0] << 24) | (data_convert[1] << 16) | (data_convert[2] << 8) | (data_convert[3]);
    uint32_t address_value = *(uint32_t *) address_read;
    sprintf(TX, "Address value: %lx\r\n", address_value);
    bsp_uart_printf_len(USART_UD, (uint8_t *) TX, strlen(TX));
    break;
  }
  case APP_OTA_WRITE_FLASH:
  {
    /* code */
    uint8_t data_convert[4];
    app_ota_parse_element(data_convert, &data[4], 8);
    uint32_t address_write = (data_convert[0] << 24) | (data_convert[1] << 16) | (data_convert[2] << 8) | (data_convert[3]);
    app_ota_parse_element(data_convert, &data[15], 8);
    uint32_t value = (data_convert[0] << 24) | (data_convert[1] << 16) | (data_convert[2] << 8) | (data_convert[3]);
    bsp_flash_write(address_write, &value, 1);
    bsp_uart_printf(USART_UD, (uint8_t *) "Write flash memory\r\n");
    break;
  }
  case APP_OTA_EXECUTE_APPLICATION_USER:
  {
    /* code */
    bsp_uart_printf(USART_UD, (uint8_t *) "Go To The Application Code\r\n");
    flag_ota_check_infor = FALSE;
    break;
  }
  case APP_OTA_GET_BOOTLOADER_VERSION:
  {
    /* code */
    bsp_uart_printf(USART_UD, (uint8_t *) "BOOOTLOADER STM32446RET6 V1.0\r\n");
    break;
  }
  case APP_OTA_GET_DEVICE_INFORMATION:
  {
    /* code */
    bsp_uart_printf(USART_UD, (uint8_t *) "STM32446RET6 FLASH: 512KB, RAM: 128KB\r\n");
    break;
  }
  case APP_OTA_ERASE_SECTOR_DEVICE:
  {
    /* code */
    bsp_flash_erase(bsp_get_address_from_sector(data[2] - 48));
    bsp_uart_printf(USART_UD, (uint8_t *) "Erase sector done !!!\r\n");
    break;
  }
  default:
  {
    bsp_uart_printf(USART_UD, (uint8_t *) "Error !!!\r\n");
    break;
  }
  }
}

void app_ota_show_function(void)
{
  flag_ota_check_infor = TRUE;
  bsp_uart_printf(USART_UD, (uint8_t *) "\r========== Main Menu ==========\r\n");
  bsp_uart_printf(USART_UD, (uint8_t *) "     1. Read Flash Memory \r\n");
  bsp_uart_printf(USART_UD, (uint8_t *) "     2. Write Flash Memory \r\n");
  bsp_uart_printf(USART_UD, (uint8_t *) "     3. Execute The Application Code \r\n");
  bsp_uart_printf(USART_UD, (uint8_t *) "     4. Get Bootloader Version \r\n");
  bsp_uart_printf(USART_UD, (uint8_t *) "     5. Get Device Information \r\n");
  bsp_uart_printf(USART_UD, (uint8_t *) "     6. Erase Sector Device \r\n");
  bsp_uart_printf(USART_UD, (uint8_t *) "=============================\r\n");
}

void app_ota_handle_data_receive_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
  app_ota_hex_form_data_t hex_data;
  uint8_t                *buff = data;
  switch (buff[0])
  {
  /* flag ota update */
  case APP_OTA_COMMUNICATION:
  {
    /* send confirm character */
    if (buff[1] == 'S')
      app_ota_show_function();
    else if (buff[1] == '1')
      app_ota_communication(1, buff, size);
    else if (buff[1] == '2')
      app_ota_communication(2, buff, size);
    else if (buff[1] == '3')
      app_ota_communication(3, buff, size);
    else if (buff[1] == '4')
      app_ota_communication(4, buff, size);
    else if (buff[1] == '5')
      app_ota_communication(5, buff, size);
    else if (buff[1] == '6')
      app_ota_communication(6, buff, size);
    break;
  }
  /* flag ota update */
  case APP_OTA_UPDATE_FW:
  {
    flag_ota_update = TRUE;
    /* send confirm character */
    bsp_uart_printf(USART_UD, APP_OTA_READ_CONFIRM_CHAR);
    break;
  }
  /* flag check size flash memory */
  case APP_OTA_CHECK_SIZE_FLASH:
  {
    size_current  = 0;
    uint8_t index = 1;
    while (buff[index] != '\n')
    {
      size_current = size_current * 10 + (buff[index] - 48);
      index++;
    }
    if (size_current > max_size_flash)
      /* send char cancel update because over size flash memory */
      bsp_uart_printf(USART_UD, APP_OTA_CANCEL_UPDATE);
    else
    {
      /* send confirm character */
      bsp_uart_printf(USART_UD, APP_OTA_READ_CONFIRM_CHAR);
      flag_size_flash = TRUE;
    }
    break;
  }
  /* flag ota start */
  case APP_OTA_UPDATE_START:
  {
    if (flag_earse_ok == TRUE)
    {
      flag_earse_ok = FALSE;
      /* send confirm character */
      bsp_uart_printf(USART_UD, APP_OTA_READ_CONFIRM_CHAR);
    }
    break;
  }
  /* ota data received */
  case APP_OTA_RECEIVE_DATA:
  {
    /* check data correct ? */
    if ((app_ota_handle_data_receive(&hex_data, buff, size) == STATE_NO_ERRORS) && flag_ota_update && flag_size_flash)
    {
      /* write data receive to flash memory */
      app_ota_write_flash_memory(&hex_data);
      if (!address_write_flash)
        /* address write memory errors */
        bsp_uart_printf(USART_UD, APP_OTA_CANCEL_UPDATE);
      else
        /* data write successful */
        bsp_uart_printf(USART_UD, APP_OTA_READ_CONFIRM_CHAR);
    }
    else
      /* data receive miss or error */
      bsp_uart_printf(USART_UD, APP_OTA_RESEND_DATA);
    break;
  }
  /* flag ota firmware update complete */
  case APP_OTA_UPDATE_FW_COMPLETE:
  {
    flag_ota_complete = TRUE;
    /* send confirm character */
    bsp_uart_printf(USART_UD, APP_OTA_READ_CONFIRM_CHAR);
    break;
  }
  default: break;
  }
  free(buff);
}

/* End of file -------------------------------------------------------- */
