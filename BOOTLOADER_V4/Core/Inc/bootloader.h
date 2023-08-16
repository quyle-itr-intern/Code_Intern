/**
 * @file       bootloader.h
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

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BOOTLOADER_H
#define BOOTLOADER_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
#include "bsp_uart.h"
#include "bootloader_command.h"

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

void bootloader_start_up(void);

#endif

/* End of file -------------------------------------------------------- */
