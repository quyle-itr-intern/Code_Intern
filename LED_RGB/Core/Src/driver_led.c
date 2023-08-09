
/**
  ******************************************************************************

  DRIVER LED For STM32F446RE
  Author:   LVQ
  Updated:  9 August 2022

  ******************************************************************************
*/

#include "driver_led.h"

void led_rgb_init(void)
{
  bsp_led_rgb_init();
}

void led_red_set_percent(uint8_t percent)
{
  bsp_led_red_set_duty(percent);
}

void led_green_set_percent(uint8_t percent)
{
  bsp_led_green_set_duty(percent);
}

void led_blue_set_percent(uint8_t percent)
{
  bsp_led_blue_set_duty(percent);
}

void led_rgb_set_fade_out(uint32_t color)
{
  uint8_t duty_color_red   = (color >> 16 & 0xFF);
  uint8_t duty_color_green = (color >> 8 & 0xFF);
  uint8_t duty_color_blue  = (color & 0xFF);

  for (uint8_t i = 1; i < 101; i++)
  {
    bsp_led_red_set_duty(duty_color_red * (i * 1.0 / 100));
    bsp_led_green_set_duty(duty_color_green * (i * 1.0 / 100));
    bsp_led_blue_set_duty(duty_color_blue * (i * 1.0 / 100));
    HAL_Delay(25);
  }
}

void led_rgb_set_fade_in(uint32_t color)
{
  uint8_t duty_color_red   = (color >> 16 & 0xFF);
  uint8_t duty_color_green = (color >> 8 & 0xFF);
  uint8_t duty_color_blue  = (color & 0xFF);

  for (uint8_t i = 100; i > 0; i--)
  {
    bsp_led_red_set_duty(duty_color_red * (i * 1.0 / 100));
    bsp_led_green_set_duty(duty_color_green * (i * 1.0 / 100));
    bsp_led_blue_set_duty(duty_color_blue * (i * 1.0 / 100));
    HAL_Delay(25);
  }
}
