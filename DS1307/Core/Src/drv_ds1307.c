/**
 * @file       drv_ds1307.c
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

/* Includes ----------------------------------------------------------- */
#include "drv_ds1307.h"

/* Private variables -------------------------------------------------- */
static uint8_t drv_ds1307_initialize = 0;
/* Variable must be initialize to usu #define DRV_DS1307_CHECK_STATUS(x) */
static uint8_t _status = 0;

/* Private defines ---------------------------------------------------- */

#define DRV_DS1307_CHECK_ID(x)              \
  if ((x < 0) || (x > 7))                   \
    return drv_ds1307_invalid_id;           \
  if (((1 << (x)) & drv_ds1307_initialize)) \
    return drv_ds1307_id_exist;

#define DRV_DS1307_CHECK_INIT(x)             \
  if (!((1 << (x)) & drv_ds1307_initialize)) \
    return drv_ds1307_no_init;

#define DRV_DS1307_CHECK_I2C(x)             \
  if ((drv_ds1307->drv_ds1307_i2c) == NULL) \
    return drv_ds1307_invalid_parameter;

#define DRV_DS1307_CHECK_FAULT(x)          \
  {                                        \
    switch (x)                             \
    {                                      \
    case bsp_i2c_not_initialize:           \
    {                                      \
      return drv_ds1307_i2c_no_init;       \
      break;                               \
    }                                      \
    case bsp_i2c_busy:                     \
    {                                      \
      return drv_ds1307_busy;              \
      break;                               \
    }                                      \
    case bsp_i2c_error:                    \
    {                                      \
      return drv_ds1307_error;             \
      break;                               \
    }                                      \
    case bsp_i2c_invalid_parameter:        \
    {                                      \
      return drv_ds1307_invalid_parameter; \
      break;                               \
    }                                      \
    default: break;                        \
    }                                      \
  }

#define DRV_DS1307_CHECK_STATUS(x)     \
  _status = x;                         \
  if ((_status) != drv_ds1307_success) \
    return _status;

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

static uint8_t             drv_ds1307_bcd_to_bin(uint8_t bcd);
static uint8_t             drv_ds1307_bin_to_bcd(uint8_t bin);
static drv_ds1307_status_t drv_ds1307_write_reg(drv_ds1307_config_t drv_ds1307, uint8_t reg_write, uint8_t data_write);
static drv_ds1307_status_t drv_ds1307_write_multi_reg(drv_ds1307_config_t drv_ds1307, uint8_t *data_write, uint8_t size_data);
static drv_ds1307_status_t drv_ds1307_read_reg(drv_ds1307_config_t drv_ds1307, uint8_t reg_read, uint8_t *data_read);
static drv_ds1307_status_t drv_ds1307_read_multi_reg(drv_ds1307_config_t drv_ds1307, uint8_t reg_read, uint8_t *data_read, uint8_t size_data);
static uint8_t             check_leap_year(uint16_t year);
static uint8_t             day_of_month(uint8_t month, uint8_t year);
static drv_ds1307_status_t drv_ds1307_check_time_set(drv_ds1307_config_t drv_ds1307, drv_ds1307_time_t time_set);

/* Function definitions ----------------------------------------------- */

static uint8_t drv_ds1307_bcd_to_bin(uint8_t bcd)
{
  uint8_t dec = 10 * (bcd >> 4);
  dec += bcd & 0x0F;
  return dec;
}

static uint8_t drv_ds1307_bin_to_bcd(uint8_t bin)
{
  uint8_t high = bin / 10;
  uint8_t low  = bin - (high * 10);
  return high << 4 | low;
}

drv_ds1307_status_t drv_ds1307_init(drv_ds1307_config_t *drv_ds1307)
{
  /* Check device ID */
  DRV_DS1307_CHECK_ID(drv_ds1307->drv_ds1307_id);
  DRV_DS1307_CHECK_I2C(drv_ds1307->drv_ds1307_i2c);
  if (drv_ds1307->drv_ds1307_mode_time != 12)
    drv_ds1307->drv_ds1307_mode_time = 24;

  if (drv_ds1307->drv_ds1307_i2c->Instance == I2C1)
    DRV_DS1307_CHECK_FAULT(bsp_i2c1_init(drv_ds1307->drv_ds1307_i2c))
  else if (drv_ds1307->drv_ds1307_i2c->Instance == I2C2)
    DRV_DS1307_CHECK_FAULT(bsp_i2c2_init(drv_ds1307->drv_ds1307_i2c))
  else if (drv_ds1307->drv_ds1307_i2c->Instance == I2C3)
    DRV_DS1307_CHECK_FAULT(bsp_i2c3_init(drv_ds1307->drv_ds1307_i2c))

  /* Add device to private variable */
  drv_ds1307_initialize |= (1 << drv_ds1307->drv_ds1307_id);

  /* Set mode default mode 24h */
  DRV_DS1307_CHECK_STATUS(drv_ds1307_set_mode_time(*drv_ds1307, drv_ds1307->drv_ds1307_mode_time));
  return drv_ds1307_success;
}

static drv_ds1307_status_t drv_ds1307_write_reg(drv_ds1307_config_t drv_ds1307, uint8_t reg_write, uint8_t data_write)
{
  /* Check device init */
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  if (drv_ds1307.drv_ds1307_i2c->Instance == I2C1)
    DRV_DS1307_CHECK_FAULT(bsp_i2c1_write(drv_ds1307.drv_ds1307_address, reg_write, data_write))
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C2)
    DRV_DS1307_CHECK_FAULT(bsp_i2c2_write(drv_ds1307.drv_ds1307_address, reg_write, data_write))
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C3)
    DRV_DS1307_CHECK_FAULT(bsp_i2c3_write(drv_ds1307.drv_ds1307_address, reg_write, data_write))
  else
    return drv_ds1307_error;
  return drv_ds1307_success;
}

static drv_ds1307_status_t drv_ds1307_write_multi_reg(drv_ds1307_config_t drv_ds1307, uint8_t *data_write, uint8_t size_data)
{
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  if (drv_ds1307.drv_ds1307_i2c->Instance == I2C1)
    DRV_DS1307_CHECK_FAULT(bsp_i2c1_write_multi(drv_ds1307.drv_ds1307_address, data_write, size_data))
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C2)
    DRV_DS1307_CHECK_FAULT(bsp_i2c2_write_multi(drv_ds1307.drv_ds1307_address, data_write, size_data))
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C3)
    DRV_DS1307_CHECK_FAULT(bsp_i2c3_write_multi(drv_ds1307.drv_ds1307_address, data_write, size_data))
  else
    return drv_ds1307_error;
  return drv_ds1307_success;
}

static drv_ds1307_status_t drv_ds1307_read_reg(drv_ds1307_config_t drv_ds1307, uint8_t reg_read, uint8_t *data_read)
{
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  if (drv_ds1307.drv_ds1307_i2c->Instance == I2C1)
    DRV_DS1307_CHECK_FAULT(bsp_i2c1_read(drv_ds1307.drv_ds1307_address, reg_read, data_read))
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C2)
    DRV_DS1307_CHECK_FAULT(bsp_i2c2_read(drv_ds1307.drv_ds1307_address, reg_read, data_read))
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C3)
    DRV_DS1307_CHECK_FAULT(bsp_i2c3_read(drv_ds1307.drv_ds1307_address, reg_read, data_read))
  else
    return drv_ds1307_error;
  return drv_ds1307_success;
}

static drv_ds1307_status_t drv_ds1307_read_multi_reg(drv_ds1307_config_t drv_ds1307, uint8_t reg_read, uint8_t *data_read, uint8_t size_data)
{
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  if (drv_ds1307.drv_ds1307_i2c->Instance == I2C1)
    DRV_DS1307_CHECK_FAULT(bsp_i2c1_read_multi(drv_ds1307.drv_ds1307_address, reg_read, data_read, size_data))
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C2)
    DRV_DS1307_CHECK_FAULT(bsp_i2c2_read_multi(drv_ds1307.drv_ds1307_address, reg_read, data_read, size_data))
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C3)
    DRV_DS1307_CHECK_FAULT(bsp_i2c3_read_multi(drv_ds1307.drv_ds1307_address, reg_read, data_read, size_data))
  else
    return drv_ds1307_error;
  return drv_ds1307_success;
}

static uint8_t check_leap_year(uint16_t year)
{
  return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

static uint8_t day_of_month(uint8_t month, uint8_t year)
{
  switch (month)
  {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 12: return 31;
  case 4:
  case 6:
  case 9:
  case 11: return 30;
  case 2:
    if (check_leap_year(2000 + year))
      return 29;
    else
      return 28;
  default: break;
  }
  return 0;
}

static drv_ds1307_status_t drv_ds1307_check_time_set(drv_ds1307_config_t drv_ds1307, drv_ds1307_time_t time_set)
{
  if (time_set.seconds < 0 || time_set.seconds > 59)
    return drv_ds1307_invalid_parameter;
  if (time_set.minutes < 0 || time_set.minutes > 59)
    return drv_ds1307_invalid_parameter;
  if (time_set.hours < 0 || (time_set.hours > ((drv_ds1307.drv_ds1307_mode_time == 12) ? 12 : 24)))
    return drv_ds1307_invalid_parameter;
  if (time_set.day < 1 || time_set.day > 7)
    return drv_ds1307_invalid_parameter;
  if (time_set.year < 0 || time_set.year > 99)
    return drv_ds1307_invalid_parameter;
  if (time_set.month < 0 || time_set.month > 12)
    return drv_ds1307_invalid_parameter;
  if (time_set.date < 1 || (time_set.date > day_of_month(time_set.month, time_set.year)))
    return drv_ds1307_invalid_parameter;
  if (drv_ds1307.drv_ds1307_mode_time == 12)
  {
    if (time_set.am_pm != 0 && time_set.am_pm != 1)
      return drv_ds1307_invalid_parameter;
  }
  else
  {
    if (time_set.am_pm != 2)
      return drv_ds1307_invalid_parameter;
  }
  return drv_ds1307_success;
}

drv_ds1307_status_t drv_ds1307_check_device(drv_ds1307_config_t drv_ds1307)
{
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  if (drv_ds1307.drv_ds1307_i2c->Instance == I2C1)
    DRV_DS1307_CHECK_FAULT(bsp_i2c1_is_busy())
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C2)
    DRV_DS1307_CHECK_FAULT(bsp_i2c2_is_busy())
  else if (drv_ds1307.drv_ds1307_i2c->Instance == I2C3)
    DRV_DS1307_CHECK_FAULT(bsp_i2c3_is_busy())
  else
    return drv_ds1307_error;
  return drv_ds1307_success;
}

drv_ds1307_status_t drv_ds1307_set_mode_time(drv_ds1307_config_t drv_ds1307, uint8_t mode)
{
  /* Check device init */
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  uint8_t data_reg = 0;
  switch (mode)
  {
  case 12:
  {
    DRV_DS1307_CHECK_STATUS(drv_ds1307_read_reg(drv_ds1307, DRV_DS1307_HOURS, &data_reg));

    if (data_reg & DRV_DS1307_MODE_HOURS)
      return drv_ds1307_success;

    uint8_t hour = drv_ds1307_bcd_to_bin(data_reg & 0x3F);
    data_reg     = 0;
    if (hour > 12)
    {
      data_reg = DRV_DS1307_AM_PM;
      hour -= 12;
    }
    data_reg |= DRV_DS1307_MODE_HOURS | drv_ds1307_bin_to_bcd(hour);
    return drv_ds1307_write_reg(drv_ds1307, DRV_DS1307_HOURS, data_reg);
  }
  case 24:
  {
    DRV_DS1307_CHECK_STATUS(drv_ds1307_read_reg(drv_ds1307, DRV_DS1307_HOURS, &data_reg));

    if (!(data_reg & DRV_DS1307_MODE_HOURS))
      return drv_ds1307_success;

    uint8_t hour = drv_ds1307_bcd_to_bin(data_reg & 0x1F);
    if (data_reg & DRV_DS1307_AM_PM)
      hour += 12;
    data_reg = drv_ds1307_bin_to_bcd(hour);
    return drv_ds1307_write_reg(drv_ds1307, DRV_DS1307_HOURS, data_reg);
  }
  default: return drv_ds1307_invalid_parameter;
  }
  return drv_ds1307_error;
}

drv_ds1307_status_t drv_ds1307_get_time(drv_ds1307_config_t drv_ds1307, drv_ds1307_time_t *get_time)
{
  /* Check device init */
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  uint8_t data_time[7] = { 0 };
  DRV_DS1307_CHECK_STATUS(drv_ds1307_read_multi_reg(drv_ds1307, DRV_DS1307_SECONDS, data_time, 7));
  get_time->seconds = drv_ds1307_bcd_to_bin(data_time[0] & 0x7F);
  get_time->minutes = drv_ds1307_bcd_to_bin(data_time[1]);
  get_time->hours   = drv_ds1307_bcd_to_bin(data_time[2] & (drv_ds1307.drv_ds1307_mode_time == 24 ? 0x3F : 0x1F));
  if (drv_ds1307.drv_ds1307_mode_time == 12)
    get_time->am_pm = (data_time[2] >> 5) & 0x01;
  else
    get_time->am_pm = 2;
  get_time->day   = drv_ds1307_bcd_to_bin(data_time[3]);
  get_time->date  = drv_ds1307_bcd_to_bin(data_time[4]);
  get_time->month = drv_ds1307_bcd_to_bin(data_time[5]);
  get_time->year  = drv_ds1307_bcd_to_bin(data_time[6]);
  return drv_ds1307_success;
}

drv_ds1307_status_t drv_ds1307_epoch_to_date_time(drv_ds1307_time_t *date_time, uint32_t epoch_time)
{
  struct tm *_time;
  time_t     epoch_time_c = epoch_time;
  _time                   = localtime(&epoch_time_c);
  _time->tm_year = (2000 + date_time->year) - 1900;

  date_time->year    = _time->tm_year;
  date_time->month   = _time->tm_mon + 1;
  date_time->date    = _time->tm_mday;
  date_time->hours   = _time->tm_hour;
  date_time->minutes = _time->tm_min;
  date_time->seconds = _time->tm_sec;
  date_time->day     = _time->tm_wday;

  return drv_ds1307_success;
}

drv_ds1307_status_t drv_ds1307_get_epoch_time(drv_ds1307_config_t drv_ds1307, uint32_t *epoch_time)
{
  /* Check device init */
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  drv_ds1307_time_t time;
  DRV_DS1307_CHECK_STATUS(drv_ds1307_get_time(drv_ds1307, &time));

  struct tm time_date;
  memset((void *) &time_date, 0, sizeof(time_date));

  time_date.tm_year = time.year;
  time_date.tm_mon  = time.month - 1;
  time_date.tm_mday = time.date;
  if (time.am_pm == 1)
    time_date.tm_hour = time.hours + 12; /* PM */
  else
    time_date.tm_hour = time.hours;
  time_date.tm_min = time.minutes;
  time_date.tm_sec = time.seconds;

  time_date.tm_year = (2000 + time.year) - 1900;
  *epoch_time       = mktime(&time_date);
  return drv_ds1307_success;
}

drv_ds1307_status_t drv_ds1307_set_time(drv_ds1307_config_t drv_ds1307, drv_ds1307_time_t set_time)
{
  /* Check device init */
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);
  DRV_DS1307_CHECK_STATUS(drv_ds1307_check_time_set(drv_ds1307, set_time));

  uint8_t data_time[8] = { 0 };
  uint8_t reg_data     = 0;
  data_time[0]         = DRV_DS1307_SECONDS;
  DRV_DS1307_CHECK_STATUS(drv_ds1307_read_reg(drv_ds1307, DRV_DS1307_SECONDS, &reg_data));
  data_time[1] = (reg_data & 0x80) | drv_ds1307_bin_to_bcd(set_time.seconds);
  data_time[2] = drv_ds1307_bin_to_bcd(set_time.minutes);
  if (drv_ds1307.drv_ds1307_mode_time == 12)
  {
    if (set_time.am_pm)
      data_time[3] = DRV_DS1307_AM_PM;
    data_time[3] |= DRV_DS1307_MODE_HOURS | drv_ds1307_bin_to_bcd(set_time.hours);
  }
  else
    data_time[3] = drv_ds1307_bin_to_bcd(set_time.hours);
  data_time[4] = drv_ds1307_bin_to_bcd(set_time.day);
  data_time[5] = drv_ds1307_bin_to_bcd(set_time.date);
  data_time[6] = drv_ds1307_bin_to_bcd(set_time.month);
  data_time[7] = drv_ds1307_bin_to_bcd(set_time.year % 100);

  DRV_DS1307_CHECK_STATUS(drv_ds1307_write_multi_reg(drv_ds1307, data_time, 8));
  return drv_ds1307_success;
}

drv_ds1307_status_t drv_ds1307_set_clock_haft(drv_ds1307_config_t drv_ds1307, uint8_t bit_clock_haft)
{
  /* Check device init */
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  uint8_t value    = bit_clock_haft ? (1 << 7) : 0;
  uint8_t reg_data = 0;
  DRV_DS1307_CHECK_STATUS(drv_ds1307_read_reg(drv_ds1307, DRV_DS1307_SECONDS, &reg_data));
  return drv_ds1307_write_reg(drv_ds1307, DRV_DS1307_SECONDS, value | (reg_data & 0x7F));
}

drv_ds1307_status_t drv_ds1307_set_output_sqw(drv_ds1307_config_t drv_ds1307, drv_ds1307_output_sqw_t out_sqw)
{
  /* Check device init */
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  uint8_t reg_data = 0;
  DRV_DS1307_CHECK_STATUS(drv_ds1307_read_reg(drv_ds1307, DRV_DS1307_CONTROL, &reg_data));
  return drv_ds1307_write_reg(drv_ds1307, DRV_DS1307_CONTROL, out_sqw | (reg_data & 0xFC));
}

drv_ds1307_status_t drv_ds1307_config_swq(drv_ds1307_config_t drv_ds1307, uint8_t swq_mode)
{
  /* Check device init */
  DRV_DS1307_CHECK_INIT(drv_ds1307.drv_ds1307_id);

  uint8_t reg_data = 0;
  DRV_DS1307_CHECK_STATUS(drv_ds1307_read_reg(drv_ds1307, DRV_DS1307_CONTROL, &reg_data));
  return drv_ds1307_write_reg(drv_ds1307, DRV_DS1307_CONTROL, (swq_mode ? swq_mode << 4 : 0) | (reg_data & 0xEF));
}

drv_ds1307_status_t drv_ds1307_set_mode_12h(drv_ds1307_config_t *drv_ds1307)
{
  drv_ds1307->drv_ds1307_mode_time = 12;
  return drv_ds1307_set_mode_time(*drv_ds1307, 12);
}

drv_ds1307_status_t drv_ds1307_set_mode_24h(drv_ds1307_config_t *drv_ds1307)
{
  drv_ds1307->drv_ds1307_mode_time = 24;
  return drv_ds1307_set_mode_time(*drv_ds1307, 24);
}

drv_ds1307_status_t drv_ds1307_set_clock_haft_enable(drv_ds1307_config_t drv_ds1307)
{
  return drv_ds1307_set_clock_haft(drv_ds1307, 0);
}

drv_ds1307_status_t drv_ds1307_set_clock_haft_disable(drv_ds1307_config_t drv_ds1307)
{
  return drv_ds1307_set_clock_haft(drv_ds1307, 1);
}

drv_ds1307_status_t drv_ds1307_set_out_1Hz(drv_ds1307_config_t drv_ds1307)
{
  return drv_ds1307_set_output_sqw(drv_ds1307, drv_ds1307_output_sqw_1Hz);
}

drv_ds1307_status_t drv_ds1307_set_out_4096Hz(drv_ds1307_config_t drv_ds1307)
{
  return drv_ds1307_set_output_sqw(drv_ds1307, drv_ds1307_output_sqw_4096Hz);
}

drv_ds1307_status_t drv_ds1307_set_out_8192Hz(drv_ds1307_config_t drv_ds1307)
{
  return drv_ds1307_set_output_sqw(drv_ds1307, drv_ds1307_output_sqw_8192Hz);
}

drv_ds1307_status_t drv_ds1307_set_out_32768Hz(drv_ds1307_config_t drv_ds1307)
{
  return drv_ds1307_set_output_sqw(drv_ds1307, drv_ds1307_output_sqw_32768Hz);
}

drv_ds1307_status_t drv_ds1307_enable_swq(drv_ds1307_config_t drv_ds1307)
{
  return drv_ds1307_config_swq(drv_ds1307, 1);
}

drv_ds1307_status_t drv_ds1307_disable_swq(drv_ds1307_config_t drv_ds1307)
{
  return drv_ds1307_config_swq(drv_ds1307, 0);
}

/* End of file -------------------------------------------------------- */
