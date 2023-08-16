/**
 * @file       bsp_uart.h
 * @copyright  Copyright (C) 2023 QuyLe Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    1.0.0
 * @date       2023-08-14
 * @author     quyle-itr-intern
 *
 * @brief      bsp uart
 *
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_USART_H
#define BSP_USART_H

/* Includes ----------------------------------------------------------- */
#include "app_ota.h"
#include "main.h"

/* Public defines ----------------------------------------------------- */
#define TIME_OUT_TRANSMIT_UART 1000

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

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

/**
 * @brief  uart init
 *
 * @return
 *
 */
void bsp_uart_init(void);

/**
 * @brief  uart deinit peripheral
 *
 * @return
 *
 */
void bsp_uart_deinit_peripheral(void);

#endif

/* End of file -------------------------------------------------------- */