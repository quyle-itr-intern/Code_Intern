/**
 * @file       bsp_gpio.h
 * @copyright  Copyright (C) 2023 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    v1.0.0
 * @date       2023-08-20
 * @author     quyle-itr-intern
 *
 * @brief      handle gpio
 *
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

void bsp_gpio_set_pin(GPIO_TypeDef *bsp_gpio_port, uint16_t bsp_gpio_pin);
void bsp_gpio_reset_pin(GPIO_TypeDef *bsp_gpio_port, uint16_t bsp_gpio_pin);

#endif  // __BSP_GPIO_H

/* End of file -------------------------------------------------------- */