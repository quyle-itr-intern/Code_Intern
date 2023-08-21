/**
 * @file       bsp_spi.c
 * @copyright  Copyright (C) 2023 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    v1.0.0
 * @date       2023-08-19
 * @author     quyle-itr-intern
 *
 * @brief      handle spi
 *
 * @note
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_spi.h"

/* Private defines ---------------------------------------------------- */
#define BSP_SPI_TIMEOUT 100

#define BSP_SPI_LOCK    1
#define BSP_SPI_UNLOCK  0

#define BSP_SPI_CHECK_INITIALIZE(x) \
  if (!(x))                         \
    return bsp_spi_not_initialize;

#define BSP_SPI_CHECK_IS_BUSY(x) \
  if ((x))                       \
    return bsp_spi_busy;

/* Private enumerate/structure ---------------------------------------- */
static SPI_HandleTypeDef *bsp_spi1 = NULL;
static SPI_HandleTypeDef *bsp_spi2 = NULL;
static SPI_HandleTypeDef *bsp_spi3 = NULL;

typedef struct
{
  uint8_t bsp_spi1_init;
  uint8_t bsp_spi2_init;
  uint8_t bsp_spi3_init;
} bsp_spi_initialize_t;

typedef struct
{
  uint8_t bsp_spi1_busy;
  uint8_t bsp_spi2_busy;
  uint8_t bsp_spi3_busy;
} bsp_spi_is_busy_t;

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static bsp_spi_initialize_t bsp_spi_init    = { .bsp_spi1_init = 0, .bsp_spi2_init = 0, .bsp_spi3_init = 0 };
static bsp_spi_is_busy_t    bsp_spi_is_busy = { .bsp_spi1_busy = 0, .bsp_spi2_busy = 0, .bsp_spi3_busy = 0 };

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */

/* ===================================== BSP spi INIT ===================================== */

bsp_spi_status_t bsp_spi1_init(SPI_HandleTypeDef *hspi1)
{
  if (bsp_spi_init.bsp_spi1_init)
    return bsp_spi_success;
  /* Check the spi handle allocation */
  if (hspi1 == NULL)
    return bsp_spi_error;
  if (hspi1->Instance != SPI1)
    return bsp_spi_invalid_parameter;
  bsp_spi1                   = hspi1;
  bsp_spi_init.bsp_spi1_init = 1;
  return bsp_spi_success;
}

bsp_spi_status_t bsp_spi2_init(SPI_HandleTypeDef *hspi2)
{
  if (bsp_spi_init.bsp_spi2_init)
    return bsp_spi_success;
  /* Check the spi handle allocation */
  if (hspi2 == NULL)
    return bsp_spi_error;
  if (hspi2->Instance != SPI2)
    return bsp_spi_invalid_parameter;
  bsp_spi2                   = hspi2;
  bsp_spi_init.bsp_spi2_init = 1;
  return bsp_spi_success;
}

bsp_spi_status_t bsp_spi3_init(SPI_HandleTypeDef *hspi3)
{
  if (bsp_spi_init.bsp_spi3_init)
    return bsp_spi_success;
  /* Check the spi handle allocation */
  if (hspi3 == NULL)
    return bsp_spi_error;
  if (hspi3->Instance != SPI3)
    return bsp_spi_invalid_parameter;
  bsp_spi3                   = hspi3;
  bsp_spi_init.bsp_spi3_init = 1;
  return bsp_spi_success;
}

/* ===================================== BSP SPI1 READ/WRITE ===================================== */

bsp_spi_status_t bsp_spi1_write(uint8_t reg_write, uint8_t data_write)
{
  /* Check initialize bsp spi */
  BSP_SPI_CHECK_INITIALIZE(bsp_spi_init.bsp_spi1_init);
  BSP_SPI_CHECK_IS_BUSY(bsp_spi_is_busy.bsp_spi1_busy);

  /* SPI1 Running */
  bsp_spi_is_busy.bsp_spi1_busy = BSP_SPI_LOCK;

  /* Transmit data */
  if (HAL_SPI_Transmit(bsp_spi1, &reg_write, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI1 Stop */
    bsp_spi_is_busy.bsp_spi1_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  if (HAL_SPI_Transmit(bsp_spi1, &data_write, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI1 Stop */
    bsp_spi_is_busy.bsp_spi1_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  /* SPI1 Stop */
  bsp_spi_is_busy.bsp_spi1_busy = BSP_SPI_UNLOCK;
  return bsp_spi_success;
}

bsp_spi_status_t bsp_spi1_read(uint8_t reg_read, uint8_t *data_read)
{
  /* Check initialize bsp spi */
  BSP_SPI_CHECK_INITIALIZE(bsp_spi_init.bsp_spi1_init);
  BSP_SPI_CHECK_IS_BUSY(bsp_spi_is_busy.bsp_spi1_busy);

  /* SPI1 Running */
  bsp_spi_is_busy.bsp_spi1_busy = BSP_SPI_LOCK;

  /* Transmit data */
  if (HAL_SPI_Transmit(bsp_spi1, &reg_read, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI1 Stop */
    bsp_spi_is_busy.bsp_spi1_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  /* Receive data */
  if (HAL_SPI_Receive(bsp_spi1, data_read, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI1 Stop */
    bsp_spi_is_busy.bsp_spi1_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  /* SPI1 Stop */
  bsp_spi_is_busy.bsp_spi1_busy = BSP_SPI_UNLOCK;
  return bsp_spi_success;
}

/* ===================================== BSP SPI2 READ/WRITE ===================================== */

bsp_spi_status_t bsp_spi2_write(uint8_t reg_write, uint8_t data_write)
{
  /* Check initialize bsp spi */
  BSP_SPI_CHECK_INITIALIZE(bsp_spi_init.bsp_spi2_init);
  BSP_SPI_CHECK_IS_BUSY(bsp_spi_is_busy.bsp_spi2_busy);

  /* SPI2 Running */
  bsp_spi_is_busy.bsp_spi2_busy = BSP_SPI_LOCK;

  /* Transmit data */
  if (HAL_SPI_Transmit(bsp_spi2, &reg_write, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI2 Stop */
    bsp_spi_is_busy.bsp_spi2_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  if (HAL_SPI_Transmit(bsp_spi2, &data_write, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI2 Stop */
    bsp_spi_is_busy.bsp_spi2_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  /* SPI2 Stop */
  bsp_spi_is_busy.bsp_spi2_busy = BSP_SPI_UNLOCK;
  return bsp_spi_success;
}

bsp_spi_status_t bsp_spi2_read(uint8_t reg_read, uint8_t *data_read)
{
  /* Check initialize bsp spi */
  BSP_SPI_CHECK_INITIALIZE(bsp_spi_init.bsp_spi2_init);
  BSP_SPI_CHECK_IS_BUSY(bsp_spi_is_busy.bsp_spi2_busy);

  /* SPI2 Running */
  bsp_spi_is_busy.bsp_spi2_busy = BSP_SPI_LOCK;

  /* Transmit data */
  if (HAL_SPI_Transmit(bsp_spi2, &reg_read, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI2 Stop */
    bsp_spi_is_busy.bsp_spi2_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  /* Receive data */
  if (HAL_SPI_Receive(bsp_spi2, data_read, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI2 Stop */
    bsp_spi_is_busy.bsp_spi2_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  /* SPI2 Stop */
  bsp_spi_is_busy.bsp_spi2_busy = BSP_SPI_UNLOCK;
  return bsp_spi_success;
}

/* ===================================== BSP SPI3 READ/WRITE ===================================== */

bsp_spi_status_t bsp_spi3_write(uint8_t reg_write, uint8_t data_write)
{
  /* Check initialize bsp spi */
  BSP_SPI_CHECK_INITIALIZE(bsp_spi_init.bsp_spi3_init);
  BSP_SPI_CHECK_IS_BUSY(bsp_spi_is_busy.bsp_spi3_busy);

  /* SPI3 Running */
  bsp_spi_is_busy.bsp_spi3_busy = BSP_SPI_LOCK;

  /* Transmit data */
  if (HAL_SPI_Transmit(bsp_spi3, &reg_write, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI3 Stop */
    bsp_spi_is_busy.bsp_spi3_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  if (HAL_SPI_Transmit(bsp_spi3, &data_write, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI3 Stop */
    bsp_spi_is_busy.bsp_spi3_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  /* SPI3 Stop */
  bsp_spi_is_busy.bsp_spi3_busy = BSP_SPI_UNLOCK;
  return bsp_spi_success;
}

bsp_spi_status_t bsp_spi3_read(uint8_t reg_read, uint8_t *data_read)
{
  /* Check initialize bsp spi */
  BSP_SPI_CHECK_INITIALIZE(bsp_spi_init.bsp_spi3_init);
  BSP_SPI_CHECK_IS_BUSY(bsp_spi_is_busy.bsp_spi3_busy);

  /* SPI3 Running */
  bsp_spi_is_busy.bsp_spi3_busy = BSP_SPI_LOCK;

  /* Transmit data */
  if (HAL_SPI_Transmit(bsp_spi3, &reg_read, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI3 Stop */
    bsp_spi_is_busy.bsp_spi3_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  /* Receive data */
  if (HAL_SPI_Receive(bsp_spi3, data_read, 1, BSP_SPI_TIMEOUT) == HAL_ERROR)
  {
    /* SPI3 Stop */
    bsp_spi_is_busy.bsp_spi3_busy = BSP_SPI_UNLOCK;
    return bsp_spi_error;
  }
  /* SPI3 Stop */
  bsp_spi_is_busy.bsp_spi3_busy = BSP_SPI_UNLOCK;
  return bsp_spi_success;
}

/* End of file -------------------------------------------------------- */
