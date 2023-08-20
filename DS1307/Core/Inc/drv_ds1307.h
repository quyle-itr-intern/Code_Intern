/**
 * @file       drv_ds1307.h
 * @copyright  Copyright (C) 2023 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    major.minor.patch
 * @date       2023-08-19
 * @author     quyle-itr-intern
 *
 * @brief      driver ds1307
 *
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRV_DS1307_H
#define __DRV_DS1307_H

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"
#include "main.h"

#include <string.h>
#include <time.h>

/* Public defines ----------------------------------------------------- */

/* Registers location */
#define DRV_DS1307_SECONDS    (0x00)
#define DRV_DS1307_MINUTES    (0x01)
#define DRV_DS1307_HOURS      (0x02)
#define DRV_DS1307_DAY        (0x03)
#define DRV_DS1307_DATE       (0x04)
#define DRV_DS1307_MONTH      (0x05)
#define DRV_DS1307_YEAR       (0x06)
#define DRV_DS1307_CONTROL    (0x07)
#define DRV_DS1307_MODE_HOURS (1 << 6)
#define DRV_DS1307_AM_PM      (1 << 5)

/* Public enumerate/structure ----------------------------------------- */

/**
 * @brief  Structure for bsp ds1307 status
 */
typedef enum
{
  drv_ds1307_error = 0,
  drv_ds1307_busy,
  drv_ds1307_success,
  drv_ds1307_invalid_parameter,
  drv_ds1307_invalid_id,
  drv_ds1307_id_exist,
  drv_ds1307_no_init,
  drv_ds1307_i2c_no_init,
} 
drv_ds1307_status_t;

/**
 * @brief  Structure for the frequency of the square-wave output
 */
typedef enum
{
  drv_ds1307_output_sqw_1Hz = 0,
  drv_ds1307_output_sqw_4096Hz,
  drv_ds1307_output_sqw_8192Hz,
  drv_ds1307_output_sqw_32768Hz,
} 
drv_ds1307_output_sqw_t;

/**
 * @brief  Structure for ds1307 config
 * @attention  drv_ds1307_id only valid from 0 to 7. Maximum 8 device
 *             drv_ds1307_mode_time default mode 24h
 */
typedef struct
{
  I2C_HandleTypeDef *drv_ds1307_i2c;
  uint8_t            drv_ds1307_address;
  uint8_t            drv_ds1307_id;
  uint8_t            drv_ds1307_mode_time;
} 
drv_ds1307_config_t;

/**
 * @brief  Structure for date/time
 */
typedef struct
{
  uint8_t seconds; /* Seconds parameter, from 00 to 59 */
  uint8_t minutes; /* Minutes parameter, from 00 to 59 */
  uint8_t hours;   /* Hours parameter, 24Hour mode, 00 to 23 */
  uint8_t day;     /* Day in a week, from 1 to 7 */
  uint8_t date;    /* Date in a month, 1 to 31 */
  uint8_t month;   /* Month in a year, 1 to 12 */
  uint8_t year;    /* Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
  uint8_t am_pm;   /* AM/PM Clock AM: 0, PM: 1 */
} 
drv_ds1307_time_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief bsp_ds1307_init
 *
 * This function init ds1307
 * Default : mode 24h
 *
 * @param drv_ds1307 struct data config de1307 device
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 *    - 3 : drv_ds1307_invalid_parameter
 *    - 4 : drv_ds1307_invalid_id
 *    - 5 : drv_ds1307_id_exist
 *    - 6 : drv_ds1307_no_init,
 *    - 7 : drv_ds1307_i2c_no_init
 */
drv_ds1307_status_t drv_ds1307_init(drv_ds1307_config_t *drv_ds1307);

drv_ds1307_status_t drv_ds1307_check_device(drv_ds1307_config_t drv_ds1307);

/**
 * @brief drv_ds1307_set_mode_time
 *
 * This function set mode time for ds1307
 *
 * @param drv_ds1307 struct data config de1307 device
 * @param mode       mode 12h or 24h
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 *    - 3 : drv_ds1307_invalid_parameter
 *    - 4 : drv_ds1307_invalid_id
 *    - 5 : drv_ds1307_id_exist
 *    - 6 : drv_ds1307_no_init,
 *    - 7 : drv_ds1307_i2c_no_init
 */
drv_ds1307_status_t drv_ds1307_set_mode_time(drv_ds1307_config_t drv_ds1307, uint8_t mode);

/**
 * @brief drv_ds1307_get_time
 *
 * This function get date/time from ds1307
 *
 * @param drv_ds1307 struct data config de1307 device
 * @param get_time   struct save data time
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 *    - 3 : drv_ds1307_invalid_parameter
 *    - 4 : drv_ds1307_invalid_id
 *    - 5 : drv_ds1307_id_exist
 *    - 6 : drv_ds1307_no_init,
 *    - 7 : drv_ds1307_i2c_no_init
 */
drv_ds1307_status_t drv_ds1307_get_time(drv_ds1307_config_t drv_ds1307, drv_ds1307_time_t *get_time);

/**
 * @brief drv_ds1307_get_epoch_time
 *
 * This function get date/time style epoch time from ds1307
 *
 * @param drv_ds1307 struct data config de1307 device
 * @param epoch_time style time epoch
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 *    - 3 : drv_ds1307_invalid_parameter
 *    - 4 : drv_ds1307_invalid_id
 *    - 5 : drv_ds1307_id_exist
 *    - 6 : drv_ds1307_no_init,
 *    - 7 : drv_ds1307_i2c_no_init
 */
drv_ds1307_status_t drv_ds1307_get_epoch_time(drv_ds1307_config_t drv_ds1307, uint32_t *epoch_time);

/**
 * @brief bsp_ds1307_set_time
 *
 * This function set date/time from ds1307
 *
 * @param drv_ds1307 struct data config de1307 device
 * @param set_time   struct save data time
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 *    - 3 : drv_ds1307_invalid_parameter
 *    - 4 : drv_ds1307_invalid_id
 *    - 5 : drv_ds1307_id_exist
 *    - 6 : drv_ds1307_no_init,
 *    - 7 : drv_ds1307_i2c_no_init
 */
drv_ds1307_status_t drv_ds1307_set_time(drv_ds1307_config_t drv_ds1307, drv_ds1307_time_t set_time);

/**
 * @brief bsp_ds1307_set_clock_haft
 *
 * This function set clock haft for ds1307
 *
 * @param drv_ds1307      struct data config de1307 device
 * @param bit_clock_haft  enable or disable : 1 or 0
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_clock_haft(drv_ds1307_config_t drv_ds1307, uint8_t bit_clock_haft);

/**
 * @brief drv_ds1307_set_output_sqw
 *
 * This function set frequency of the square-wave output for ds1307
 *
 * @param drv_ds1307      struct data config de1307 device
 * @param out_sqw         select frequency of the square-wave
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_output_sqw(drv_ds1307_config_t drv_ds1307, drv_ds1307_output_sqw_t out_sqw);

/**
 * @brief drv_ds1307_config_swq
 *
 * This function set disable or enable for square-wave output
 *
 * @param drv_ds1307      struct data config de1307 device
 * @param swq_mode        disable or enable square-wave output
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_config_swq(drv_ds1307_config_t drv_ds1307, uint8_t swq_mode);

/**
 * @brief drv_ds1307_set_mode_12h
 *
 * This function set mode 12h
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_mode_12h(drv_ds1307_config_t *drv_ds1307);

/**
 * @brief drv_ds1307_set_mode_24h
 *
 * This function set mode 24h
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_mode_24h(drv_ds1307_config_t *drv_ds1307);

/**
 * \brief drv_ds1307_set_clock_haft_enable
 *
 * This function set clock haft enable
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_clock_haft_enable(drv_ds1307_config_t drv_ds1307);

/**
 * \brief drv_ds1307_set_clock_haft_disable
 *
 * This function set clock haft disable
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_clock_haft_disable(drv_ds1307_config_t drv_ds1307);

/**
 * @brief drv_ds1307_set_out_1Hz
 *
 * This function set frequency of the square-wave output 1Hz
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_out_1Hz(drv_ds1307_config_t drv_ds1307);

/**
 * @brief drv_ds1307_set_out_4096Hz
 *
 * This function set frequency of the square-wave output 4096Hz
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_out_4096Hz(drv_ds1307_config_t drv_ds1307);

/**
 * @brief drv_ds1307_set_out_8192Hz
 *
 * This function set frequency of the square-wave output 8192Hz
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_out_8192Hz(drv_ds1307_config_t drv_ds1307);

/**
 * @brief drv_ds1307_set_out_32768Hz
 *
 * This function set frequency of the square-wave output 32768Hz
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_set_out_32768Hz(drv_ds1307_config_t drv_ds1307);

/**
 * @brief drv_ds1307_enable_swq
 *
 * This function enable Square-Wave Enable
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_enable_swq(drv_ds1307_config_t drv_ds1307);

/**
 * @brief drv_ds1307_disable_swq
 *
 * This function disable Square-Wave Enable
 *
 * @return
 *    - 0 : drv_ds1307_busy
 *    - 1 : drv_ds1307_success
 *    - 2 : drv_ds1307_error
 */
drv_ds1307_status_t drv_ds1307_disable_swq(drv_ds1307_config_t drv_ds1307);

#endif  // __DRV_DS1307_H

/* End of file -------------------------------------------------------- */
