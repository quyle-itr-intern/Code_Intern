/**
 * @file       bsp_spi.h
 * @copyright  Copyright (C) 2023 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    v1.0.0
 * @date       2023-08-20
 * @author     quyle-itr-intern
 *
 * @brief      handle spi
 *
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BSP_SPI_H
#define __BSP_SPI_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

typedef enum
{
  bsp_spi_error,
  bsp_spi_success,
  bsp_spi_busy,
  bsp_spi_not_initialize,
  bsp_spi_invalid_parameter,
} bsp_spi_status_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

bsp_spi_status_t bsp_spi1_init(SPI_HandleTypeDef *hspi1);
bsp_spi_status_t bsp_spi2_init(SPI_HandleTypeDef *hspi2);
bsp_spi_status_t bsp_spi3_init(SPI_HandleTypeDef *hspi3);

bsp_spi_status_t bsp_spi1_write(uint8_t reg_write, uint8_t data_write);
bsp_spi_status_t bsp_spi1_read(uint8_t reg_read, uint8_t *data_read);

bsp_spi_status_t bsp_spi2_write(uint8_t reg_write, uint8_t data_write);
bsp_spi_status_t bsp_spi2_read(uint8_t reg_read, uint8_t *data_read);

bsp_spi_status_t bsp_spi3_write(uint8_t reg_write, uint8_t data_write);
bsp_spi_status_t bsp_spi3_read(uint8_t reg_read, uint8_t *data_read);

#endif  // __BSP_SPI_H

/* End of file -------------------------------------------------------- */
