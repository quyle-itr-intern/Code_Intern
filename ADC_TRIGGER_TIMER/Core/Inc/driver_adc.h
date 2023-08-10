/**
  ******************************************************************************

  DRIVER ADC For STM32F446RE
  Author:   LVQ
  Updated:  8 August 2022

  ******************************************************************************
*/

#ifndef DRIVER_ADC_H
#define DRIVER_ADC_H

#include "main.h"
#include "bsp_adc.h"

/**
 * @brief  adc init
 *
 * @param[in]     volts 	data set adc volts
 *
 * @return
 */
void adc_init(void);

/**
 * @brief  adc set volts
 *
 * @param[in]     volts 	data set adc volts
 *
 * @return
 */
void adc_set_volts(float volts);

/**
 * @brief  adc get volts
 *
 * @param[in]     volts 	pointer save data
 *
 * @return
 */
void adc_get_volts(float *volts);

#endif
