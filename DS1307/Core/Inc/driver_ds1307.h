/**
  ******************************************************************************

  DRIVER DS1307 For STM32F446RE
  Author:   LVQ
  Updated:  10 August 2023

  ******************************************************************************
*/

#ifndef DRIVER_DS1307_H
#define DRIVER_DS1307_H

#include "bsp_ds1307.h"
#include "main.h"

/**
 * @brief  Structure for date/time
 */
typedef struct
{
  uint8_t seconds; /*!< Seconds parameter, from 00 to 59 */
  uint8_t minutes; /*!< Minutes parameter, from 00 to 59 */
  uint8_t hours;   /*!< Hours parameter, 24Hour mode, 00 to 23 */
  uint8_t day;     /*!< Day in a week, from 1 to 7 */
  uint8_t date;    /*!< Date in a month, 1 to 31 */
  uint8_t month;   /*!< Month in a year, 1 to 12 */
  uint8_t year;    /*!< Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
} 
drv_ds1307_time_t;

/**
 * @brief  Structure for driver ds1307 status
 */
typedef enum
{
  drv_ds1307_error = 0,
  drv_ds1307_success,
  drv_ds1307_no_device,
  drv_ds1307_ready,
} 
drv_ds1307_status_t;

/**
 * \brief drv_ds1307_init
 *
 * This function init ds1307 enable clock haft
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_init(void);

/**
 * \brief drv_ds1307_check_device
 *
 * This function check device connect
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_check_device(void);

/**
 * \brief drv_ds1307_set_clock_haft_enable
 *
 * This function set clock haft enable
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_set_clock_haft_enable(void);

/**
 * \brief drv_ds1307_set_clock_haft_disable
 *
 * This function set clock haft disable
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_set_clock_haft_disable(void);

/**
 * \brief drv_ds1307_set_mode_12h
 *
 * This function set mode 12h
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_set_mode_12h(void);

/**
 * \brief drv_ds1307_set_mode_24h
 *
 * This function set mode 24h
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_set_mode_24h(void);

/**
 * \brief drv_ds1307_set_time
 *
 * This function takes time and set up to ds1307
 *
 * \param time data time write ds1307
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_set_time(drv_ds1307_time_t time);

/**
 * \brief drv_ds1307_get_time
 *
 * This function takes get and save to pointer
 *
 * \param time data time read from ds1307
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_get_time(drv_ds1307_time_t *time);

/**
 * \brief drv_ds1307_enable_swq
 *
 * This function enable Square-Wave Enable
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_enable_swq(void);

/**
 * \brief drv_ds1307_disable_swq
 *
 * This function disable Square-Wave Enable
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_disable_swq(void);

/**
 * \brief drv_ds1307_set_out_1Hz
 *
 * This function set frequency of the square-wave output 1Hz
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_set_out_1Hz(void);

/**
 * \brief drv_ds1307_set_out_4096Hz
 *
 * This function set frequency of the square-wave output 4096Hz
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_set_out_4096Hz(void);

/**
 * \brief drv_ds1307_set_out_8192Hz
 *
 * This function set frequency of the square-wave output 8192Hz
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_set_out_8192Hz(void);

/**
 * \brief drv_ds1307_set_out_32768Hz
 *
 * This function set frequency of the square-wave output 32768Hz
 *
 * \return driver ds1307 status
 */
drv_ds1307_status_t drv_ds1307_set_out_32768Hz(void);

#endif
