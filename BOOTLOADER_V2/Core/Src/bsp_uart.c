/**
  ******************************************************************************

  BSP UART For STM32F446RE
  Author:   LVQ
  Updated:  9 August 2022

  ******************************************************************************
*/

#include "bsp_uart.h"

void bsp_uart_printf(UART_HandleTypeDef *huart, uint8_t *string)
{
  HAL_UART_Transmit(huart, string, sizeof(string), TIME_OUT_TRANSMIT_UART);
}

void bsp_uart_receive_to_idle_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
  if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(huart, data, size))
  {
    Error_Handler();
  }
}

void bsp_uart_dma_unregister_callback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID)
{
  HAL_DMA_UnRegisterCallback(hdma, CallbackID);
}