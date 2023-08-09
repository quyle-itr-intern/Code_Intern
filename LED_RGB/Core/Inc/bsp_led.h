/**
  ******************************************************************************

  BSP LED For STM32F446RE
  Author:   LVQ
  Updated:  9 August 2022

  ******************************************************************************
*/

#ifndef BSP_LED_H
#define BSP_LED_H

#include "main.h"

extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;

#define RED_TIM      &htim3
#define GREEN_TIM    &htim3
#define BLUE_TIM     &htim3

#define RED_CH       TIM_CHANNEL_1
#define GREEN_CH     TIM_CHANNEL_2
#define BLUE_CH      TIM_CHANNEL_3

#define MAX_DUTY_LED 999

void bsp_led_rgb_init(void);

void bsp_led_red_set_duty(uint8_t percent);

void bsp_led_green_set_duty(uint8_t percent);

void bsp_led_blue_set_duty(uint8_t percent);

#endif
