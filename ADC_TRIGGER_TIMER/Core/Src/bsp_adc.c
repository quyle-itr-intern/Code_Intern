/**
  ******************************************************************************

  BSP ADC For STM32F446RE
  Author:   LVQ
  Updated:  8 August 2022

  ******************************************************************************
*/

#include "bsp_adc.h"

#include <stdlib.h>
#include <string.h>

extern ADC_HandleTypeDef  hadc1;
extern TIM_HandleTypeDef  htim3;
extern TIM_HandleTypeDef  htim8;
extern UART_HandleTypeDef huart2;

#define RANGE_PWM (18000 - 1)
uint8_t rx_data = 0;
uint8_t count   = 0;
char    data_buffer[5];
char    volts[5];
float   fvolts     = 0;
float   fadc_volts = 0;

void bsp_adc_init(void)
{
  HAL_ADC_Start_IT(&hadc1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

void bsp_adc_set_volts(float volts)
{
  fvolts = (fvolts / 3.3) * RANGE_PWM;
  __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, (uint32_t) fvolts);
}

void bsp_adc_get_volts(float *volts)
{
  *volts = fadc_volts;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (rx_data == '\n')
  {
    data_buffer[count++] = '\n';
    HAL_UART_Transmit(huart, (uint8_t *) data_buffer, count, HAL_MAX_DELAY);
    strcpy(volts, data_buffer);
    memset(data_buffer, 0, count);
    count  = 0;
    fvolts = atof(volts);
    fvolts = (fvolts / 3.3) * RANGE_PWM;
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, (uint32_t) fvolts);
  }
  else
  {
    data_buffer[count++] = rx_data;
  }
  HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  fadc_volts = HAL_ADC_GetValue(&hadc1) * 1.0 / 4095 * 3.3;
}
