/**
  ******************************************************************************

  DRIVER DS1307 For STM32F446RE
  Author:   LVQ
  Updated:  10 August 2023

  ******************************************************************************
*/

#include "driver_ds1307.h"

drv_ds1307_status_t drv_ds1307_init(void)
{
  return bsp_ds1307_init();
}

drv_ds1307_status_t drv_ds1307_check_device(void)
{
  return bsp_ds1307_check_device();
}

drv_ds1307_status_t drv_ds1307_get_time(drv_ds1307_time_t *time)
{
  uint8_t data_time[7] = { 0 };
  if (bsp_ds1307_get_time(data_time) != bsp_ds1307_success)
    return drv_ds1307_error;
  time->seconds = data_time[0];
  time->minutes = data_time[1];
  time->hours   = data_time[2];
  time->day     = data_time[3];
  time->date    = data_time[4];
  time->month   = data_time[5];
  time->year    = data_time[6];
  return drv_ds1307_success;
}

drv_ds1307_status_t drv_ds1307_set_time(drv_ds1307_time_t time)
{
  return bsp_ds1307_set_time(time.seconds, time.minutes, time.hours, time.date, time.day, time.month, time.year);
}

drv_ds1307_status_t drv_ds1307_set_mode_12h(void)
{
  return bsp_ds1307_set_mode_time(12);
}

drv_ds1307_status_t drv_ds1307_set_mode_24h(void)
{
  return bsp_ds1307_set_mode_time(24);
}

drv_ds1307_status_t drv_ds1307_set_clock_haft_enable(void)
{
  return bsp_ds1307_set_clock_haft(0);
}

drv_ds1307_status_t drv_ds1307_set_clock_haft_disable(void)
{
  return bsp_ds1307_set_clock_haft(1);
}

drv_ds1307_status_t drv_ds1307_set_out_1Hz(void)
{
  return bsp_ds1307_set_output_sqw(bsp_ds1307_output_sqw_1Hz);
}

drv_ds1307_status_t drv_ds1307_set_out_4096Hz(void)
{
  return bsp_ds1307_set_output_sqw(bsp_ds1307_output_sqw_4096Hz);
}

drv_ds1307_status_t drv_ds1307_set_out_8192Hz(void)
{
  return bsp_ds1307_set_output_sqw(bsp_ds1307_output_sqw_8192Hz);
}

drv_ds1307_status_t drv_ds1307_set_out_32768Hz(void)
{
  return bsp_ds1307_set_output_sqw(bsp_ds1307_output_sqw_32768Hz);
}

drv_ds1307_status_t drv_ds1307_enable_swq(void)
{
  return bsp_ds1307_config_swq(1);
}

drv_ds1307_status_t drv_ds1307_disable_swq(void)
{
  return bsp_ds1307_config_swq(0);
}
