/**
 * @file       bootloader.c
 * @copyright  Copyright (C) 2019 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    1.0.0
 * @date       2023-08-14
 * @author     quy-itr-intern
 *
 * @brief      handle bootloader
 *
 * @note
 */

/* Includes ----------------------------------------------------------- */
#include "bootloader.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */

void bootloader_handle_command(bootloader_command_data_t comand_data)
{
  switch (comand_data.cmd)
  {
  case COMMAND_READ:
  {
    /* code */
    bsp_uart1_printf((uint8_t *) "COMMAND_READ\r\n");
    break;
  }
  case COMMAND_WRITE:
  {
    /* code */
    bsp_uart1_printf((uint8_t *) "COMMAND_WRITE\r\n");
    break;
  }
  case COMMAND_ERASE:
  {
    /* code */
    bsp_uart1_printf((uint8_t *) "COMMAND_ERASE\r\n");
    break;
  }
  case COMMAND_JUMP:
  {
    /* code */
    bsp_uart1_printf((uint8_t *) "COMMAND_JUMP\r\n");
    break;
  }
  case COMMAND_UPDATE:
  {
    /* code */
    if (comand_data.status_update == 1)
    {
      bsp_uart1_printf((uint8_t *) "Update firmware\r\n");
    }
    else if (comand_data.status_update == 2)
    {
      bsp_uart1_printf((uint8_t *) "Begin Update firmware\r\n");
    }
    else if (comand_data.status_update == 3)
    {
      bsp_uart1_printf((uint8_t *) "End Update firmware\r\n");
    }
    else if (comand_data.status_update == 4)
    {
      char tx[50];
      sprintf(tx, "Size flash: %ld\r\n", comand_data.size_flash);
      bsp_uart1_printf((uint8_t *) "COMMAND_UPDATE ");
      bsp_uart1_printf((uint8_t *) tx);
    }
    else if (comand_data.status_update == 5)
    {
      for (uint8_t i = 0; i < comand_data.len; i++)
      {
        char tx[2];
        sprintf(tx, "%c", comand_data.data[i]);
        bsp_uart1_printf((uint8_t *) tx);
      }
      bsp_uart1_printf((uint8_t *) "\r\n");
    }
    break;
  }
  default: break;
  }
}

void bootloader_start_up(void)
{
  bsp_uart_set_callback_bootloader_command(bootloader_handle_command);
  bsp_uart_init();

  while (1)
  {
    /* code */
  }
}

/* End of file -------------------------------------------------------- */
