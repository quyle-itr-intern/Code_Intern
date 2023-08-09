/**
  ******************************************************************************

  OTA For STM32F446RE
  Author:   LVQ
  Updated:  6 August 2023
  Algorithm:

  ******************************************************************************
*/

#include "app_ota.h"
#include <stdlib.h>

#define ADDRESS_FIRMWARE_APPLICATION 0x08008000U
#define STM32F446xx 1
#define USART_UD &huart1

uint8_t flag_ota_update = FALSE;
uint8_t flag_size_flash = FALSE;
uint8_t flag_ota_complete = FALSE;
uint8_t flag_earse_ok = FALSE;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart1_rx;

#ifdef STM32F401xx
uint32_t size_current = 0;
uint8_t index = 0;
uint32_t max_size_flash = 245760;
#elif STM32F446xx
uint32_t max_size_flash = 507904;
#endif

/*****************************************************************/
uint8_t g_flag_cplt_dma = FALSE;

#define RX_BUFFER_SIZE 50
uint8_t data_receive_dma[RX_BUFFER_SIZE];

uint32_t size_current;
uint8_t rx_buffer[50];
uint8_t *buffer_save_data_handle;

volatile uint32_t size_data_firmware = 0;
volatile uint16_t data_write_flash[50] = {0}; /* Array to store the data of line to pass it to Flash Writer */
volatile uint32_t address_write_flash;		  /* Pointer to store the address in to write */

void app_ota_get_high_address(app_ota_hex_form_data_t hex_data)
{
	uint32_t address = 0;
	address |= hex_data.data[0] << 8 | hex_data.data[1];
	address_write_flash = 0x08000000 | (address << 16); /* Address Save Firmware Update */
}

void app_ota_write_flash_memory(app_ota_hex_form_data_t *hex_data)
{
	uint8_t data_count, i;
	uint8_t counter_index = 0;

	switch (hex_data->record_type)
	{
	case 4: /* Extended Linear Address Record: used to identify the extended linear address  */
	{
		app_ota_get_high_address(*hex_data);
		break;
	}
	case 5: /* Start Linear Address Record: the address where the program starts to run      */
	{
		break;
	}
	case 0: /* Data Rrecord: used to record data, most records of HEX files are data records */
	{
		/* Get Character Count */
		data_count = hex_data->byte_count;
		size_data_firmware += data_count;

		/* Set full address */
		address_write_flash = address_write_flash & 0xFFFF0000;
		address_write_flash = address_write_flash |
							  (hex_data->address[0] << 8) |
							  (hex_data->address[1]);

		/* Check address wite flash data */
		if (address_write_flash < ADDRESS_FIRMWARE_APPLICATION)
		{
			address_write_flash = 0;
			return;
		}

		/* Get the data of the record */
		for (i = 0; i < data_count; i += 2)
		{
			data_write_flash[counter_index] = (hex_data->data[i + 1] << 8) | (hex_data->data[i]);
			counter_index++;
		}

		if (data_count % 2 != 0)
		{
			data_write_flash[counter_index] = 0xFF00 | (hex_data->data[data_count - 1]);
			bsp_flash_write(address_write_flash, (uint32_t *)data_write_flash, data_count / 4 + 1);
		}
		else
			bsp_flash_write(address_write_flash, (uint32_t *)data_write_flash, data_count / 4);
		break;
	}
	default:
		break;
	}
}

void app_ota_start_up_bootloader(void)
{
	HAL_DMA_UnRegisterCallback(&hdma_usart1_rx, HAL_DMA_XFER_HALFCPLT_CB_ID);
	if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(&huart1, data_receive_dma, RX_BUFFER_SIZE))
	{
		Error_Handler();
	}

	buffer_save_data_handle = rx_buffer;

	uint32_t time_last = HAL_GetTick();
	while (HAL_GetTick() - time_last < 3000)
	{
		if (flag_ota_update == TRUE && flag_size_flash == TRUE)
		{
			flag_earse_ok = FALSE;
			uint8_t start_sector = bsp_get_sector_from_address(ADDRESS_FIRMWARE_APPLICATION);
			uint8_t end_sector = bsp_get_sector_from_address(ADDRESS_FIRMWARE_APPLICATION + size_current);
			uint8_t number_sector = end_sector - start_sector + 1;

			/* Erase sector for write data */
			for (uint8_t i = 0; i < number_sector; i++)
			{
				bsp_flash_erase(bsp_get_address_from_sector(start_sector));
				start_sector++;
			}
			flag_earse_ok = TRUE;
			while (flag_ota_complete == FALSE)
			{
			}
		}
	}
}

void app_ota_jump_to_firmware(void)
{
	NVIC_DisableIRQ(USART1_IRQn);
	HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);

	HAL_UART_DeInit(&huart1);
	HAL_UART_DeInit(&huart2);
	HAL_DMA_DeInit(&hdma_usart1_rx);
	HAL_RCC_DeInit();

	SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
	__set_MSP(*((volatile uint32_t *)ADDRESS_FIRMWARE_APPLICATION));
	uint32_t u32JumpAddress = *((volatile uint32_t *)(ADDRESS_FIRMWARE_APPLICATION + 4));
	void (*reset_handler)(void) = (void *)u32JumpAddress;
	reset_handler();
}

void app_ota_parse_element(uint8_t *des, uint8_t *buff, uint8_t length)
{
	uint8_t count_array = 0U;
	uint8_t buffer_temp[32];

	/* Convert data string to data hex and save to des buffer*/
	for (uint8_t count = 0U; count < length; count++)
	{
		if (buff[count] >= '0' && buff[count] <= '9')
			buffer_temp[count] = buff[count] - 48;
		else if (buff[count] >= 'A' && buff[count] <= 'F')
			buffer_temp[count] = buff[count] - 65 + 10;
		else
			return;
		if (count % 2 != 0)
		{
			des[count_array] = ((buffer_temp[count - 1] << 4) | (buffer_temp[count]));
			count_array++;
		}
	}
}

uint8_t app_ota_handle_data_receive(app_ota_hex_form_data_t *hex_data, uint8_t *data, uint16_t size)
{
	uint8_t cal_check_sum = 0;
	app_ota_field_hex_files_t state = START_CODE;
	uint8_t buffer_save_data_hex[16];

	/* parse string to hex form data hex */
	for (uint8_t count = 0; count < size; count++)
	{
		switch (state)
		{
		case START_CODE:
		{
			if (data[count] != ':')
				return STATE_ERRORS;
			else
				state = BYTE_COUNT;
			break;
		}
		case BYTE_COUNT:
		{
			app_ota_parse_element(buffer_save_data_hex, &data[count], 2U);
			hex_data->byte_count = buffer_save_data_hex[0];
			count = 2U;
			state = ADDRESS;
			break;
		}
		case ADDRESS:
		{
			app_ota_parse_element(buffer_save_data_hex, &data[count], 4U);
			hex_data->address[0] = buffer_save_data_hex[0];
			hex_data->address[1] = buffer_save_data_hex[1];
			count = 6U;
			state = RECORD_TYPE;
			break;
		}
		case RECORD_TYPE:
		{
			app_ota_parse_element(buffer_save_data_hex, &data[count], 2U);
			hex_data->record_type = buffer_save_data_hex[0];
			count = 8U;
			state = DATA;
			break;
		}
		case DATA:
		{
			app_ota_parse_element(buffer_save_data_hex, &data[count], (hex_data->byte_count * 2U));
			for (count = 0; count < hex_data->byte_count; count++)
			{
				hex_data->data[count] = buffer_save_data_hex[count];
			}
			count = 8U + (hex_data->byte_count * 2U);
			state = CHECKSUM;
			break;
		}
		case CHECKSUM:
		{
			app_ota_parse_element(buffer_save_data_hex, &data[count], 2U);
			hex_data->check_sum = buffer_save_data_hex[0];
			state = DONE;
			break;
		}
		case DONE:
		{
			cal_check_sum = hex_data->byte_count + hex_data->address[0] + hex_data->address[1] + hex_data->record_type;
			for (count = 0; count < hex_data->byte_count; count++)
			{
				cal_check_sum += hex_data->data[count];
			}
			cal_check_sum = ~cal_check_sum + 1U;
			if (cal_check_sum == hex_data->check_sum)
				return STATE_NO_ERRORS;
			else
				return STATE_ERRORS;
		}
		default:
			return STATE_ERRORS;
		}
	}
	return STATE_ERRORS;
}

void app_ota_handle_data_receive_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
	app_ota_hex_form_data_t hex_data;
	uint8_t *buff = data;
	switch (buff[0])
	{
	case APP_OTA_UPDATE_FW:
	{
		flag_ota_update = TRUE;
		HAL_UART_Transmit(USART_UD, APP_OTA_READ_CONFIRM_CHAR, 1, 10);
		break;
	}
	case APP_OTA_CHECK_SIZE_FLASH:
	{
		size_current = 0;
		uint8_t index = 1;
		while (buff[index] != '\n')
		{
			size_current = size_current * 10 + (buff[index] - 48);
			index++;
		}
		if (size_current > max_size_flash)
			HAL_UART_Transmit(USART_UD, APP_OTA_CANCEL_UPDATE, 1, 10);
		else
		{
			HAL_UART_Transmit(USART_UD, APP_OTA_READ_CONFIRM_CHAR, 1, 10);
			flag_size_flash = TRUE;
		}
		break;
	}
	case APP_OTA_UPDATE_START:
	{
		if (flag_earse_ok == TRUE)
		{
			flag_earse_ok = FALSE;
			HAL_UART_Transmit(USART_UD, APP_OTA_READ_CONFIRM_CHAR, 1, 10);
		}
		break;
	}
	case ':':
	{
		if ((app_ota_handle_data_receive(&hex_data, buff, size) == STATE_NO_ERRORS) && flag_ota_update && flag_size_flash)
		{
			app_ota_write_flash_memory(&hex_data);
			if (!address_write_flash)
				HAL_UART_Transmit(USART_UD, APP_OTA_RESEND_DATA, 1, 10);
			else
				HAL_UART_Transmit(USART_UD, APP_OTA_READ_CONFIRM_CHAR, 1, 10);
		}
		else
			HAL_UART_Transmit(USART_UD, APP_OTA_RESEND_DATA, 1, 10);
		break;
	}
	case APP_OTA_UPDATE_FW_COMPLETE:
	{
		flag_ota_complete = TRUE;
		HAL_UART_Transmit(USART_UD, APP_OTA_READ_CONFIRM_CHAR, 1, 10);
		break;
	}
	default:
		break;
	}
	free(buff);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
	uint16_t number_char_receive;
	uint8_t check_data_full[50];

	if ((huart->Instance == USART1))
	{
		static uint16_t old_pos = 0;
		uint16_t i;

		if (g_flag_cplt_dma)
		{
			g_flag_cplt_dma = FALSE;
			for (i = 0; i < size - old_pos; i++)
			{
				check_data_full[i] = data_receive_dma[old_pos + i];
			}
			app_ota_hex_form_data_t hex_data;
			if (app_ota_handle_data_receive(&hex_data, check_data_full, size - old_pos) == STATE_ERRORS)
				return;
		}

		if (size != old_pos)
		{
			if (size > old_pos)
			{
				number_char_receive = size - old_pos;
				for (i = 0; i < number_char_receive; i++)
				{
					buffer_save_data_handle[i] = data_receive_dma[old_pos + i];
				}
			}
			else
			{
				number_char_receive = RX_BUFFER_SIZE - old_pos;
				for (i = 0; i < number_char_receive; i++)
				{
					buffer_save_data_handle[i] = data_receive_dma[old_pos + i];
				}
				if (size > 0)
				{
					for (i = 0; i < size; i++)
					{
						buffer_save_data_handle[number_char_receive + i] = data_receive_dma[i];
					}
					number_char_receive += size;
				}
			}

#ifdef DEBUG
			HAL_UART_Transmit(&huart2, buffer_save_data_handle, number_char_receive, 100);
			HAL_UART_Transmit(&huart2, (uint8_t *)"\n", 1, 100);
#endif
			app_ota_handle_data_receive_dma(huart, buffer_save_data_handle, number_char_receive);
		}
		old_pos = size;
	}
}
