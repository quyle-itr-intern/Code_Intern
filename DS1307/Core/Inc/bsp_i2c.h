/**
 * @file       bsp_i2c.h
 * @copyright  Copyright (C) 2023 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    major.minor.patch
 * @date       2023-08-19
 * @author     quyle-itr-intern
 *
 * @brief      handle i2c
 *
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BSP_I2C_H
#define __BSP_I2C_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

typedef enum
{
  bsp_i2c_busy = 0U,
  bsp_i2c_ready,
  bsp_i2c_error,
  bsp_i2c_success,
  bsp_i2c_invalid_parameter,
  bsp_i2c_not_initialize,
} bsp_i2c_status_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

bsp_i2c_status_t bsp_i2c1_init(I2C_HandleTypeDef *hi2c1);
bsp_i2c_status_t bsp_i2c2_init(I2C_HandleTypeDef *hi2c2);
bsp_i2c_status_t bsp_i2c3_init(I2C_HandleTypeDef *hi2c3);

bsp_i2c_status_t bsp_i2c1_is_busy(void);
bsp_i2c_status_t bsp_i2c2_is_busy(void);
bsp_i2c_status_t bsp_i2c3_is_busy(void);

bsp_i2c_status_t bsp_i2c1_write(uint8_t address_slave, uint8_t reg_write, uint8_t data_write);
bsp_i2c_status_t bsp_i2c1_write_multi(uint8_t address_slave, uint8_t *data_write, uint16_t size_data);
bsp_i2c_status_t bsp_i2c1_read(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read);
bsp_i2c_status_t bsp_i2c1_read_multi(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data);

bsp_i2c_status_t bsp_i2c2_write(uint8_t address_slave, uint8_t reg_write, uint8_t data_write);
bsp_i2c_status_t bsp_i2c2_write_multi(uint8_t address_slave, uint8_t *data_write, uint16_t size_data);
bsp_i2c_status_t bsp_i2c2_read(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read);
bsp_i2c_status_t bsp_i2c2_read_multi(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data);

bsp_i2c_status_t bsp_i2c3_write(uint8_t address_slave, uint8_t reg_write, uint8_t data_write);
bsp_i2c_status_t bsp_i2c3_write_multi(uint8_t address_slave, uint8_t *data_write, uint16_t size_data);
bsp_i2c_status_t bsp_i2c3_read(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read);
bsp_i2c_status_t bsp_i2c3_read_multi(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data);

#endif  // __BSP_I2C_H

/* End of file -------------------------------------------------------- */
