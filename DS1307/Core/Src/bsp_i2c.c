/**
 * @file       bsp_i2c.c
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

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"

/* Private defines ---------------------------------------------------- */
#define BSP_I2C_TIMEOUT 100

#define BSP_I2C_CHECK_INITIALIZE(x) \
  if (!(x))                         \
    return bsp_i2c_not_initialize;

/* Private enumerate/structure ---------------------------------------- */
I2C_HandleTypeDef *bsp_i2c1 = NULL;
I2C_HandleTypeDef *bsp_i2c2 = NULL;
I2C_HandleTypeDef *bsp_i2c3 = NULL;

typedef struct
{
  uint8_t bsp_i2c1_init;
  uint8_t bsp_i2c2_init;
  uint8_t bsp_i2c3_init;
} bsp_i2c_initialize_t;

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static bsp_i2c_initialize_t bsp_i2c_init = { .bsp_i2c1_init = 0, .bsp_i2c2_init = 0, .bsp_i2c3_init = 0 };

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */

/* ===================================== BSP I2C INIT ===================================== */

bsp_i2c_status_t bsp_i2c1_init(I2C_HandleTypeDef *hi2c1)
{
  if (bsp_i2c_init.bsp_i2c1_init)
    return bsp_i2c_success;
  /* Check the I2C handle allocation */
  if (hi2c1 == NULL)
    return bsp_i2c_error;
  if (hi2c1->Instance != I2C1)
    return bsp_i2c_invalid_parameter;
  bsp_i2c1                   = hi2c1;
  bsp_i2c_init.bsp_i2c1_init = 1;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c2_init(I2C_HandleTypeDef *hi2c2)
{
  if (bsp_i2c_init.bsp_i2c2_init)
    return bsp_i2c_success;
  /* Check the I2C handle allocation */
  if (hi2c2 == NULL)
    return bsp_i2c_error;
  if (hi2c2->Instance != I2C2)
    return bsp_i2c_invalid_parameter;
  bsp_i2c2                   = hi2c2;
  bsp_i2c_init.bsp_i2c2_init = 1;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c3_init(I2C_HandleTypeDef *hi2c3)
{
  if (bsp_i2c_init.bsp_i2c3_init)
    return bsp_i2c_success;
  /* Check the I2C handle allocation */
  if (hi2c3 == NULL)
    return bsp_i2c_error;
  if (hi2c3->Instance != I2C3)
    return bsp_i2c_invalid_parameter;
  bsp_i2c3                   = hi2c3;
  bsp_i2c_init.bsp_i2c3_init = 1;
  return bsp_i2c_success;
}

/* ===================================== BSP I2C1 READ/WRITE ===================================== */

bsp_i2c_status_t bsp_i2c1_write(uint8_t address_slave, uint8_t reg_write, uint8_t data_write)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c1_init);
  uint8_t data[2] = { reg_write, data_write };
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c1) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c1, address_slave, data, 2, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c1_write_multi(uint8_t address_slave, uint8_t *data_write, uint16_t size_data)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c1_init);
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c1) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c1, address_slave, data_write, size_data, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c1_read(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c1_init);
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c1) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c1, address_slave, &reg_read, 1, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  /* Receive data */
  if (HAL_I2C_Master_Receive(bsp_i2c1, address_slave, data_read, 1, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c1_read_multi(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c1_init);
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c1) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c1, address_slave, &reg_read, 1, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  /* Receive data */
  if (HAL_I2C_Master_Receive(bsp_i2c1, address_slave, data_read, size_data, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

/* ===================================== BSP I2C2 READ/WRITE ===================================== */

bsp_i2c_status_t bsp_i2c2_write(uint8_t address_slave, uint8_t reg_write, uint8_t data_write)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c2_init);
  uint8_t data[2] = { reg_write, data_write };
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c2) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c2, address_slave, data, 2, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c2_write_multi(uint8_t address_slave, uint8_t *data_write, uint16_t size_data)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c2_init);
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c2) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c2, address_slave, data_write, size_data, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c2_read(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c2_init);
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c2) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c2, address_slave, &reg_read, 1, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  /* Receive data */
  if (HAL_I2C_Master_Receive(bsp_i2c2, address_slave, data_read, 1, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c2_read_multi(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c2_init);
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c2) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c2, address_slave, &reg_read, 1, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  /* Receive data */
  if (HAL_I2C_Master_Receive(bsp_i2c2, address_slave, data_read, size_data, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

/* ===================================== BSP I2C3 READ/WRITE ===================================== */

bsp_i2c_status_t bsp_i2c3_write(uint8_t address_slave, uint8_t reg_write, uint8_t data_write)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c3_init);
  uint8_t data[2] = { reg_write, data_write };
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c3) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c3, address_slave, data, 2, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c3_write_multi(uint8_t address_slave, uint8_t *data_write, uint16_t size_data)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c3_init);
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c3) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c3, address_slave, data_write, size_data, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c3_read(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c3_init);
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c3) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c3, address_slave, &reg_read, 1, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  /* Receive data */
  if (HAL_I2C_Master_Receive(bsp_i2c3, address_slave, data_read, 1, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

bsp_i2c_status_t bsp_i2c3_read_multi(uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data)
{
  /* Check initialize bsp i2c */
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c3_init);
  /* Get status bus I2C */
  if (HAL_I2C_GetState(bsp_i2c3) == HAL_I2C_STATE_BUSY)
    return bsp_i2c_busy;
  /* Transmit data */
  if (HAL_I2C_Master_Transmit(bsp_i2c3, address_slave, &reg_read, 1, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  /* Receive data */
  if (HAL_I2C_Master_Receive(bsp_i2c3, address_slave, data_read, size_data, BSP_I2C_TIMEOUT) == HAL_ERROR)
    return bsp_i2c_error;
  return bsp_i2c_success;
}

/* ===================================== BSP I2C IS BUSY ===================================== */

bsp_i2c_status_t bsp_i2c1_is_busy()
{
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c1_init);
  return (HAL_I2C_GetState(bsp_i2c1) == HAL_I2C_STATE_BUSY) ? bsp_i2c_busy : bsp_i2c_ready;
}

bsp_i2c_status_t bsp_i2c2_is_busy()
{
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c2_init);
  return (HAL_I2C_GetState(bsp_i2c2) == HAL_I2C_STATE_BUSY) ? bsp_i2c_busy : bsp_i2c_ready;
}

bsp_i2c_status_t bsp_i2c3_is_busy()
{
  BSP_I2C_CHECK_INITIALIZE(bsp_i2c_init.bsp_i2c3_init);
  return (HAL_I2C_GetState(bsp_i2c3) == HAL_I2C_STATE_BUSY) ? bsp_i2c_busy : bsp_i2c_ready;
}

/* End of file -------------------------------------------------------- */
