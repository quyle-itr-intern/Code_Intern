/**
  ******************************************************************************

  BSP DS1307 For STM32F446RE
  Author:   LVQ
  Updated:  10 August 2022

  ******************************************************************************
*/

#include "bsp_ds1307.h"

uint8_t bsp_ds1307_bcd_to_bin(uint8_t bcd)
{
  uint8_t dec = 10 * (bcd >> 4);
  dec += bcd & 0x0F;
  return dec;
}

uint8_t bsp_ds1307_bin_to_bcd(uint8_t bin)
{
  uint8_t low  = 0;
  uint8_t high = 0;
  /* High nibble */
  high = bin / 10;
  /* Low nibble */
  low = bin - (high * 10);
  /* Return */
  return high << 4 | low;
}

bsp_ds1307_status_t bsp_ds1307_write_reg(uint8_t reg_write, uint8_t data_write)
{
  uint8_t data[2] = { reg_write, data_write };
  if (HAL_I2C_Master_Transmit(BSP_I2C_NUMBER, BSP_I2C_ADDRESS, data, 2, BSP_DS1307_TIME_OUT) == HAL_OK)
    return bsp_ds1307_success;
  else
    return bsp_ds1307_error;
}

bsp_ds1307_status_t bsp_ds1307_write_multi_reg(uint8_t *data_write, uint8_t size)
{
  if (HAL_I2C_Master_Transmit(BSP_I2C_NUMBER, BSP_I2C_ADDRESS, data_write, size, BSP_DS1307_TIME_OUT) == HAL_OK)
    return bsp_ds1307_success;
  else
    return bsp_ds1307_error;
}

bsp_ds1307_status_t bsp_ds1307_read_multi_reg(uint8_t reg_read, uint8_t *data_read, uint8_t size)
{
  if (HAL_I2C_Master_Transmit(BSP_I2C_NUMBER, BSP_I2C_ADDRESS, &reg_read, 1, BSP_DS1307_TIME_OUT) != HAL_OK)
    return bsp_ds1307_error;
  if (HAL_I2C_Master_Receive(BSP_I2C_NUMBER, BSP_I2C_ADDRESS, data_read, size, BSP_DS1307_TIME_OUT) == HAL_OK)
    return bsp_ds1307_success;
  else
    return bsp_ds1307_error;
}

bsp_ds1307_status_t bsp_ds1307_read_reg(uint8_t reg_read, uint8_t *data_read)
{
  if (HAL_I2C_Master_Transmit(BSP_I2C_NUMBER, BSP_I2C_ADDRESS, &reg_read, 1, BSP_DS1307_TIME_OUT) != HAL_OK)
    return bsp_ds1307_error;
  if (HAL_I2C_Master_Receive(BSP_I2C_NUMBER, BSP_I2C_ADDRESS, data_read, 1, BSP_DS1307_TIME_OUT) == HAL_OK)
    return bsp_ds1307_success;
  else
    return bsp_ds1307_error;
}

bsp_ds1307_status_t bsp_ds1307_init(void)
{
  uint8_t reg_data = 0;
  bsp_ds1307_read_reg(BSP_DS1307_SECONDS, &reg_data);
  return bsp_ds1307_write_reg(BSP_DS1307_SECONDS, 0 | (reg_data & 0x7f));
}

bsp_ds1307_status_t bsp_ds1307_check_device(void)
{
  if (HAL_I2C_IsDeviceReady(BSP_I2C_NUMBER, BSP_I2C_ADDRESS, 5, 1000) == HAL_OK)
    return bsp_ds1307_success;
  else
    return bsp_ds1307_error;
}

bsp_ds1307_status_t bsp_ds1307_set_mode_time(uint8_t mode)
{
  uint8_t data_reg;
  switch (mode)
  {
  case 12:
  {
    bsp_ds1307_read_reg(BSP_DS1307_HOURS, &data_reg);
    data_reg |= (1 << BSP_DS1307_MODE_HOURS);
    return bsp_ds1307_write_reg(BSP_DS1307_HOURS, data_reg);
  }
  case 24:
  {
    bsp_ds1307_read_reg(BSP_DS1307_HOURS, &data_reg);
    data_reg &= ~(1 << BSP_DS1307_MODE_HOURS);
    return bsp_ds1307_write_reg(BSP_DS1307_HOURS, data_reg);
  }
  default: return bsp_ds1307_error;
  }
  return bsp_ds1307_error;
}

bsp_ds1307_status_t bsp_ds1307_get_time(uint8_t *data_time)
{
  bsp_ds1307_read_multi_reg(BSP_DS1307_SECONDS, data_time, 7);
  data_time[0] = bsp_ds1307_bcd_to_bin(data_time[0] & 0x7F);
  data_time[1] = bsp_ds1307_bcd_to_bin(data_time[1]);
  data_time[2] = bsp_ds1307_bcd_to_bin(data_time[2] & 0x3F);
  data_time[3] = bsp_ds1307_bcd_to_bin(data_time[3]);
  data_time[4] = bsp_ds1307_bcd_to_bin(data_time[4]);
  data_time[5] = bsp_ds1307_bcd_to_bin(data_time[5]);
  data_time[6] = bsp_ds1307_bcd_to_bin(data_time[6]);
  return bsp_ds1307_success;
}

bsp_ds1307_status_t bsp_ds1307_set_time(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t day, uint8_t date, uint8_t month, uint8_t year)
{
  uint8_t data_time[8] = { 0 };
  uint8_t reg_data     = 0;
  data_time[0]         = BSP_DS1307_SECONDS;
  bsp_ds1307_read_reg(BSP_DS1307_SECONDS, &reg_data);
  data_time[1] = (reg_data & 0x80) | bsp_ds1307_bin_to_bcd(seconds);
  data_time[2] = bsp_ds1307_bin_to_bcd(minutes);
  bsp_ds1307_read_reg(BSP_DS1307_HOURS, &reg_data);
  data_time[3] = (reg_data & 0xC0) | bsp_ds1307_bin_to_bcd(hours);
  data_time[4] = bsp_ds1307_bin_to_bcd(day);
  data_time[5] = bsp_ds1307_bin_to_bcd(date);
  data_time[6] = bsp_ds1307_bin_to_bcd(month);
  data_time[7] = bsp_ds1307_bin_to_bcd(year % 100);
  return bsp_ds1307_write_multi_reg(data_time, 8);
}

bsp_ds1307_status_t bsp_ds1307_set_clock_haft(uint8_t bit_clock_haft)
{
  uint8_t value    = bit_clock_haft ? (1 << 7) : 0;
  uint8_t reg_data = 0;
  bsp_ds1307_read_reg(BSP_DS1307_SECONDS, &reg_data);
  return bsp_ds1307_write_reg(BSP_DS1307_SECONDS, value | (reg_data & 0x7F));
}

bsp_ds1307_status_t bsp_ds1307_set_output_sqw(bsp_output_sqw_t out_sqw)
{
  uint8_t reg_data = 0;
  bsp_ds1307_read_reg(BSP_DS1307_CONTROL, &reg_data);
  return bsp_ds1307_write_reg(BSP_DS1307_CONTROL, out_sqw | (reg_data & 0xFC));
}

bsp_ds1307_status_t bsp_ds1307_config_swq(uint8_t mode)
{
  uint8_t reg_data = 0;
  bsp_ds1307_read_reg(BSP_DS1307_CONTROL, &reg_data);
  return bsp_ds1307_write_reg(BSP_DS1307_CONTROL, (mode ? mode << 4 : 0) | (reg_data & 0xEF));
}

/******************************************************************************/

bsp_ds1307_status_t bsp_ds1307_get_second(uint8_t *second)
{
  uint8_t reg_data = 0;
  if (bsp_ds1307_read_reg(BSP_DS1307_SECONDS, &reg_data) != bsp_ds1307_success)
    return bsp_ds1307_error;
  *second = bsp_ds1307_bcd_to_bin(reg_data & 0x7F);
  return bsp_ds1307_success;
}

bsp_ds1307_status_t bsp_ds1307_get_minute(uint8_t *minute)
{
  uint8_t reg_data = 0;
  if (bsp_ds1307_read_reg(BSP_DS1307_MINUTES, &reg_data) != bsp_ds1307_success)
    return bsp_ds1307_error;
  *minute = bsp_ds1307_bcd_to_bin(reg_data);
  return bsp_ds1307_success;
}

bsp_ds1307_status_t bsp_ds1307_get_hour(uint8_t *hour)
{
  uint8_t reg_data = 0;
  if (bsp_ds1307_read_reg(BSP_DS1307_HOURS, &reg_data) != bsp_ds1307_success)
    return bsp_ds1307_error;
  *hour = bsp_ds1307_bcd_to_bin(reg_data & 0x3F);
  return bsp_ds1307_success;
}