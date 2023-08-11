/**
  ******************************************************************************

  DRIVER DS1307 For STM32F446RE
  Author:   LVQ
  Updated:  10 August 2023

  ******************************************************************************
*/

#ifndef DRIVER_DS1307_H
#define DRIVER_DS1307_H

#include "main.h"
#include "bsp_ds1307.h"

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

typedef enum
{
  drv_ds1307_error = 0,
  drv_ds1307_success,
  drv_ds1307_no_device,
  drv_ds1307_ready,
} 
drv_ds1307_status_t;

drv_ds1307_status_t drv_ds1307_init(void);

drv_ds1307_status_t drv_ds1307_check_device(void);

drv_ds1307_status_t drv_ds1307_set_clock_haft_enable(void);

drv_ds1307_status_t drv_ds1307_set_clock_haft_disable(void);

drv_ds1307_status_t drv_ds1307_set_mode_12h(void);

drv_ds1307_status_t drv_ds1307_set_mode_24h(void);

drv_ds1307_status_t drv_ds1307_set_time(drv_ds1307_time_t time);

drv_ds1307_status_t drv_ds1307_get_time(drv_ds1307_time_t* time);

drv_ds1307_status_t drv_ds1307_enable_swq(void);

drv_ds1307_status_t drv_ds1307_disable_swq(void);

drv_ds1307_status_t drv_ds1307_set_out_1Hz(void);

drv_ds1307_status_t drv_ds1307_set_out_4096Hz(void);

drv_ds1307_status_t drv_ds1307_set_out_8192Hz(void);

drv_ds1307_status_t drv_ds1307_set_out_32768Hz(void);

#endif
