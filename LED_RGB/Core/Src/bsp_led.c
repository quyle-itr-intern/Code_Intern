/**
  ******************************************************************************

  BSP LED For STM32F446RE
  Author:   LVQ
  Updated:  9 August 2022

  ******************************************************************************
*/

#include "bsp_led.h"

uint8_t data[12];
uint32_t g_color = 0;

void bsp_led_rgb_init(void)
{
  HAL_TIM_PWM_Start(RED_TIM, RED_CH);
  HAL_TIM_PWM_Start(GREEN_TIM, GREEN_CH);
  HAL_TIM_PWM_Start(BLUE_TIM, BLUE_CH);
  HAL_UARTEx_ReceiveToIdle_IT(&huart2, data, 12);
}

void bsp_led_red_set_duty(uint8_t percent)
{
  __HAL_TIM_SET_COMPARE(RED_TIM, RED_CH, (uint32_t) 999 * (percent * 1.0 / 100));
}

void bsp_led_green_set_duty(uint8_t percent)
{
  __HAL_TIM_SET_COMPARE(GREEN_TIM, GREEN_CH, (uint32_t) 999 * (percent * 1.0 / 100));
}

void bsp_led_blue_set_duty(uint8_t percent)
{
  __HAL_TIM_SET_COMPARE(BLUE_TIM, BLUE_CH, (uint32_t) 999 * (percent * 1.0 / 100));
}

static uint8_t getHex(uint8_t Copy_uint8_tAsci)
{
    uint8_t Result = 0;
    /*0 ... 9*/
    if ((Copy_uint8_tAsci >= 48) && (Copy_uint8_tAsci <= 57))
        Result = Copy_uint8_tAsci - 48;
    /*A ... F*/
    else if ((Copy_uint8_tAsci >= 65) && (Copy_uint8_tAsci <= 70))
        Result = Copy_uint8_tAsci - 55;
    return Result;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if( huart->Instance == USART2 )
    {
        /* RGB:0xFF00FF */
        if(data[0] == 'R' && data[1] == 'G' && data[2] == 'B')
        {
            g_color = getHex(data[6]) << 20 | getHex(data[7]) << 16 | getHex(data[8]) << 12 | getHex(data[9]) << 8 | getHex(data[10]) << 4 | getHex(data[11]);
            HAL_UARTEx_ReceiveToIdle_IT(&huart2, data, 12);
        }
    }
}
