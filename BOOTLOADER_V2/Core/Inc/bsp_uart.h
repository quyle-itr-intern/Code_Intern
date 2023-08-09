/**
  ******************************************************************************

  BSP UART For STM32F446RE
  Author:   LVQ
  Updated:  9 August 2022

  ******************************************************************************
*/

#ifndef BSP_USART_H
#define BSP_USART_H

#include "main.h"

#define TIME_OUT_TRANSMIT_UART 100

/**
 * @brief  print string to uart
 *
 * @param[in]     huart 		uart transfer data
 * @param[out]    data       	data transfer through uart
 *
 * @return
 *
 */
void bsp_uart_printf(UART_HandleTypeDef *huart, uint8_t *data);

#endif
