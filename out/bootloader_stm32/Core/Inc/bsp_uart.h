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

/**
 * @brief  read string from uart
 *
 * @param[in]     huart 		uart transfer data
 * @param[out]    data       	data read through uart
 *
 * @return
 *
 */
void bsp_uart_read(UART_HandleTypeDef *huart, uint8_t *data);

/**
 * @brief  config receive data uart dma wait to idle
 *
 * @param[in]    huart 		    uart receive data
 * @param[in]    data       	data receive dma uart
 * @param[in]    size           size buffer data read from dma
 *
 * @return
 *
 */
void bsp_uart_receive_to_idle_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);

/**
 * @brief  unregister callback dma
 *
 * @param[in]    hdma 		    dma of uart
 * @param[in]    CallbackID     callback unregister
 *
 * @return
 *
 */
void bsp_uart_dma_unregister_callback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID);

#endif
