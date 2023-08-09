/**
  ******************************************************************************

  DRIVER LED For STM32F446RE
  Author:   LVQ
  Updated:  9 August 2022

  ******************************************************************************
*/

#ifndef DRIVER_LED_H
#define DRIVER_LED_H

#include "main.h"
#include "bsp_led.h"

void led_rgb_init(void);

void led_rgb_set_fade_out(uint32_t color);

void led_rgb_set_fade_in(uint32_t color);

void led_red_set_percent(uint8_t percent);

void led_green_set_percent(uint8_t percent);

void led_blue_set_percent(uint8_t percent);

#endif
