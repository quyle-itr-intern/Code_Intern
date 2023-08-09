/**
  ******************************************************************************

  BSP UART For STM32F446RE
  Author:   LVQ
  Updated:  9 August 2022

  ******************************************************************************
*/

#include "bsp_uart.h"

void bsp_uart_printf(UART_HandleTypeDef *huart, uint8_t* string)
{
    HAL_UART_Transmit(huart, string, sizeof(string), TIME_OUT_TRANSMIT_UART);
}








