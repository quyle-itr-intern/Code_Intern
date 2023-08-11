/**
  ******************************************************************************

  BSP DS1307 For STM32F446RE
  Author:   LVQ
  Updated:  10 August 2022

  ******************************************************************************
*/

#ifndef BSP_DS1307_H
#define BSP_DS1307_H

#include "main.h"

extern I2C_HandleTypeDef hi2c1;

#define BSP_I2C_NUMBER          &hi2c1
#define BSP_I2C_ADDRESS         (0xD0)

#define BSP_DS1307_TIME_OUT     (1000)

/* Registers location */
#define BSP_DS1307_SECONDS      (0x00)
#define BSP_DS1307_MINUTES      (0x01)
#define BSP_DS1307_HOURS        (0x02)
#define BSP_DS1307_DAY          (0x03)
#define BSP_DS1307_DATE         (0x04)
#define BSP_DS1307_MONTH        (0x05)
#define BSP_DS1307_YEAR         (0x06)
#define BSP_DS1307_CONTROL      (0x07)

/* Bits in control register */
#define BSP_DS1307_CONTROL_OUT  (7)
#define BSP_DS1307_CONTROL_SQWE (4)
#define BSP_DS1307_CONTROL_RS1  (1)
#define BSP_DS1307_CONTROL_RS0  (0)
#define BSP_DS1307_MODE_HOURS   (6)

typedef enum
{
  bsp_ds1307_error = 0,
  bsp_ds1307_success,
} bsp_ds1307_status_t;

typedef enum
{
  bsp_ds1307_output_sqw_1Hz = 0,
  bsp_ds1307_output_sqw_4096Hz,
  bsp_ds1307_output_sqw_8192Hz,
  bsp_ds1307_output_sqw_32768Hz,
} bsp_output_sqw_t;

bsp_ds1307_status_t bsp_ds1307_init(void);

bsp_ds1307_status_t bsp_ds1307_check_device(void);

bsp_ds1307_status_t bsp_ds1307_set_mode_time(uint8_t mode);

bsp_ds1307_status_t bsp_ds1307_read_reg(uint8_t reg_read, uint8_t *data_read);

bsp_ds1307_status_t bsp_ds1307_get_time(uint8_t *data_time);

bsp_ds1307_status_t bsp_ds1307_set_time(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t day, uint8_t date, uint8_t month, uint8_t year);

bsp_ds1307_status_t bsp_ds1307_set_clock_haft(uint8_t bit_clock_haft);

bsp_ds1307_status_t bsp_ds1307_set_output_sqw(bsp_output_sqw_t out_sqw);

bsp_ds1307_status_t bsp_ds1307_config_swq(uint8_t mode);

#endif
