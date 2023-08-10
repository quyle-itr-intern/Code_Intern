/**
  ******************************************************************************

  DRIVER ADC For STM32F446RE
  Author:   LVQ
  Updated:  8 August 2022

  ******************************************************************************
*/

#include "driver_adc.h"

void adc_init(void)
{
  bsp_adc_init();
}

void adc_set_volts(float volts)
{
  bsp_adc_set_volts(volts);
}

void adc_get_volts(float *volts)
{
  bsp_adc_get_volts(volts);
}
