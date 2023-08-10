/**
  ******************************************************************************

  BSP ADC For STM32F446RE
  Author:   LVQ
  Updated:  8 August 2022

  ******************************************************************************
*/

#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "main.h"

/**
 * @brief  adc init
 *
 * @return
 */
void bsp_adc_init(void);

/**
 * @brief  bsp set adc volts
 *
 * @param[in]     volts 	data set adc volts
 *
 * @return
 */
void bsp_adc_set_volts(float volts);

/**
 * @brief  bsp get adc volts
 *
 * @param[in]     volts 	pointer save data
 *
 * @return
 */
void bsp_adc_get_volts(float *volts);

#endif