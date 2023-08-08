/**
  ******************************************************************************

  OTA For STM32F446RE
  Author:   LVQ
  Updated:  24January 2023
  Algorithm:

  ******************************************************************************
*/

#include "lvq_stm32f4_ota.h"

#define ADDRESS_FIRMWARE_APPLICATION 0x08008000U
#define STM32F446xx 1
#define USART_UD &huart1

uint8_t flag_ota_update = FALSE;
uint8_t flag_size_flash = FALSE;
uint8_t flag_ota_complete = FALSE;
uint8_t flag_earse_ok = FALSE;

typedef struct
{
	uint8_t u8ByteCount;
	uint8_t u8Addr[2];
	uint8_t u8RecordType;
	uint8_t u8Data[16];
	uint8_t u8CheckSum;
}
hex_form_data_t;

hex_form_data_t hex_data;

typedef void (*lvq_usart_callback_t) (uint8_t);
uint8_t handle_data_hex(uint8_t* pData, uint16_t Size);
void write_flash_memory(void);

extern UART_HandleTypeDef huart1;

lvq_usart_callback_t lvq_usart_callback = NULL;

uint8_t rx_data;

uint8_t cInternalBufferData[32];
field_hex_files_t State;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
	lvq_usart_callback(rx_data);
	HAL_UART_Receive_IT(USART_UD,&rx_data,1);
  }
}

void lvq_ota_sethighaddress(void);
void lvq_ota_writeflashmemory(void);

#ifdef STM32F401xx
		uint32_t SizeCurrent = 0;
		uint8_t Size[6];
		char TX[6];
		uint8_t Index = 0;
		uint32_t MaxSizeFlash = 245760;
#elif STM32F446xx
		uint32_t SizeCurrent = 0;
		uint8_t Size[6];
		char TX[6];
		uint8_t Index = 0;
		uint32_t MaxSizeFlash = 507904;
#endif

void lvq_ota_init(void)
{
  HAL_UART_Receive_IT(USART_UD,&rx_data,1);
}

uint8_t u8RecieveBuffer[50];            /* Array to store the received line */
volatile uint8_t u8RecieveCounter = 0;            /* Counter for received line chars */
volatile uint32_t u32SizeDataFirmware = 0;
volatile uint16_t u32DataWriteFlash[50] = {0};   /* Array to store the data of line to pass it to Flash Writer */
volatile uint32_t u32AddressWriteFlash;           /* Pointer to store the address in to write */

volatile uint8_t u8LineReceivedFlag = FALSE;      /* Flag to check if the NodeMCU finished sending a complete line */
volatile uint8_t u8FinishReceiveFlag = FALSE;     /* Flag to check if NodeMCU finished sending the code to run application code */

void lvq_ota_run(void)
{
    while (u8FinishReceiveFlag == FALSE)
    {
        if (u8LineReceivedFlag == TRUE)
        {
            switch (u8RecieveBuffer[u8RecieveCounter])
            {
				case OTA_DATA_START_CHAR:
				{
					u8RecieveCounter = 0;
					HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
					break;
				}
				case OTA_LINE_BREAK_CHAR:
				{
					if( handle_data_hex(u8RecieveBuffer, u8RecieveCounter) == STATE_NO_ERRORS )
					{
						write_flash_memory();
						if( u32AddressWriteFlash )
							HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
						else
							HAL_UART_Transmit(USART_UD, OTA_CANCEL_UPDATE, 1, 100);
					}
					u8RecieveCounter = 0;
					break;
				}
				default:
						break;
            }
            u8LineReceivedFlag = FALSE;
        }
    }
    HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
}

void lvq_ota_char_received(uint8_t rec)
{
  u8RecieveBuffer[u8RecieveCounter] = rec;
  switch (rec)
  {
	case OTA_DATA_START_CHAR:
	case OTA_LINE_BREAK_CHAR:
	  u8LineReceivedFlag = TRUE;
 	  break;
	case OTA_DATA_END_CHAR:
	  u8FinishReceiveFlag = TRUE;
	  break;
	default:
	  u8RecieveCounter++;
	  break;
  }
}

void lvq_ota_check_update(uint8_t rec)
{
  switch(rec)
  {
	case OTA_UPDATE_FW:
	{
	  flag_ota_update = TRUE;
	  HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
	  break;
	}
	case OTA_CHECK_SIZE_FLASH:
	  break;
	case '\n':
	{
		for( uint8_t i = 0; i < Index; i++)
			SizeCurrent = SizeCurrent * 10 + ( Size[i] - 48 );
		if( SizeCurrent > MaxSizeFlash )
			HAL_UART_Transmit(USART_UD, OTA_CANCEL_UPDATE, 1, 10);
		else
		{
			HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
			flag_size_flash = TRUE;
		}
		break;
	}
	default:
	{
		Size[Index++] = rec;
		break;
	}
   }
}

void lvq_usart_set_callback(void* cb)
{
  lvq_usart_callback = cb;
}

void lvq_ota_setcallback_checkupdatefirmware(void)
{
  lvq_usart_set_callback(lvq_ota_check_update);
}

void lvq_ota_setcallback_updatefirmware(void)
{
  lvq_usart_set_callback(lvq_ota_char_received);
}

void get_high_address(void)
{
	uint32_t address = 0;
	address |= hex_data.u8Data[0] << 8 | hex_data.u8Data[1];
	u32AddressWriteFlash = 0x08000000 | ( address << 16); /* Address Save Firmware Update */
}

void write_flash_memory(void)
{
	uint8_t CC, i;
	uint8_t u8data_rawCounter = 0;

	switch (hex_data.u8RecordType)
	{
		case 4: /* Extended Linear Address Record: used to identify the extended linear address  */
		{	get_high_address();
			break;
		}
		case 5: /* Start Linear Address Record: the address where the program starts to run      */
		{
			break;
		}
		case 0: /* Data Rrecord: used to record data, most records of HEX files are data records */
		{
			/* Get Character Count */
			CC = hex_data.u8ByteCount;
			u32SizeDataFirmware += CC;

			/* Set full address */
			u32AddressWriteFlash = u32AddressWriteFlash & 0xFFFF0000;
			u32AddressWriteFlash = u32AddressWriteFlash 	|
								 ( hex_data.u8Addr[0] << 8) |
								 ( hex_data.u8Addr[1] );

			/* Check address wite flash data */
			if( u32AddressWriteFlash < 0x08004000 )
			{
				u32AddressWriteFlash = 0;
				return;
			}

			/* Get the data of the record */
			for (i = 0; i < CC; i += 2)
			{
				u32DataWriteFlash[u8data_rawCounter] = ( hex_data.u8Data[i+1] << 8) |
													   ( hex_data.u8Data[i] );
	     		u8data_rawCounter++;
			}

			if (CC % 2 != 0)
			{
				u32DataWriteFlash[u8data_rawCounter] = 0xFF00 | ( hex_data.u8Data[CC-1] );
				LVQ_Flash_Write(u32AddressWriteFlash, (uint32_t *)u32DataWriteFlash, CC / 4 + 1);
			}
			else
			{
				LVQ_Flash_Write(u32AddressWriteFlash, (uint32_t *)u32DataWriteFlash, CC / 4);
			}
			break;
		}
		default:
				break;
	}
}

void parse_element(uint8_t *buff, uint8_t u8length)
{
  uint8_t u8count_array = 0U;
  uint8_t u8temp_buffer[32];

  for(uint8_t u8Count = 0U; u8Count < u8length; u8Count++)
  {
	if(buff[u8Count] >= '0' && buff[u8Count] <= '9')
	{
      u8temp_buffer[u8Count] = buff[u8Count] - 48;
	}
	else if(buff[u8Count] >= 'A' && buff[u8Count] <= 'F')
	{
	  u8temp_buffer[u8Count] = buff[u8Count] - 65 + 10;
	}
	else
	 return;

    if(u8Count % 2 != 0)
	{
	  cInternalBufferData[u8count_array] = ((u8temp_buffer[u8Count - 1] << 4) | (u8temp_buffer[u8Count]));
	  u8count_array++;
	}
  }
}

uint8_t handle_data_hex(uint8_t* pData, uint16_t Size)
{
	uint8_t u8CaculChecksum = 0;

	State = START_CODE;
	for(uint8_t u8Count = 0; u8Count < Size; u8Count++)
	{
		switch(State)
		{
			case START_CODE:
			{
				if(pData[u8Count] != ':')
				{
					return STATE_ERRORS;
				}
				else
				{
				   State = BYTE_COUNT;
				}
				break;
			}
			case BYTE_COUNT:
			{
				parse_element(&pData[u8Count], 2U);
				hex_data.u8ByteCount = cInternalBufferData[0];
				u8Count = 2U;
				State = ADDRESS;
				break;
			}
			case ADDRESS:
			{
				parse_element(&pData[u8Count], 4U);
				hex_data.u8Addr[0] = cInternalBufferData[0];
				hex_data.u8Addr[1] = cInternalBufferData[1];
				u8Count = 6U;
				State = RECORD_TYPE;
				break;
			}
			case RECORD_TYPE:
			{
				parse_element(&pData[u8Count], 2U);
				hex_data.u8RecordType = cInternalBufferData[0];
				u8Count = 8U;
				State = DATA;
				break;
			}
			case DATA:
			{
				parse_element(&pData[u8Count], (hex_data.u8ByteCount*2U));
				for (u8Count = 0; u8Count < hex_data.u8ByteCount; u8Count++)
				{
					hex_data.u8Data[u8Count] = cInternalBufferData[u8Count];
				}
				u8Count = 8U + (hex_data.u8ByteCount*2U);
				State = CHECKSUM;
				break;
			}
			case CHECKSUM:
			{
				parse_element(&pData[u8Count], 2U);
				hex_data.u8CheckSum = cInternalBufferData[0];
				State = DONE;
				break;
			}
			case DONE:
			{
				u8CaculChecksum = hex_data.u8ByteCount + hex_data.u8Addr[0] + hex_data.u8Addr[1] + hex_data.u8RecordType;
				for (u8Count = 0; u8Count < hex_data.u8ByteCount; u8Count++)
				{
					u8CaculChecksum += hex_data.u8Data[u8Count];
				}
				u8CaculChecksum = ~u8CaculChecksum + 1U;
				if (u8CaculChecksum == hex_data.u8CheckSum )
				{
					return STATE_NO_ERRORS;
				}
				else
				{
					return STATE_ERRORS;
				}
			}
			default:
			{
				return STATE_ERRORS;
			}
	   }
	}
	return STATE_ERRORS;
}

void start_up_bootloader(void)
{
  lvq_ota_init();
  lvq_ota_setcallback_checkupdatefirmware();
  uint32_t time_last = HAL_GetTick();
  while( HAL_GetTick() - time_last < 3000 )
  {
	if( flag_ota_update == TRUE && flag_size_flash == TRUE )
	{
	  flag_earse_ok = FALSE;
	  uint8_t StartSector = LVQ_GetSectorFromAddress(ADDRESS_FIRMWARE_APPLICATION);
	  uint8_t EndSector = LVQ_GetSectorFromAddress(ADDRESS_FIRMWARE_APPLICATION + SizeCurrent);
	  uint8_t NumberSector = EndSector - StartSector + 1;
	  /* Erase sector for write data */
	  for(uint8_t i = 0; i < NumberSector; i++)
	  {
		LVQ_Flash_Erase(LVQ_GetAddressFromSector(StartSector));
		StartSector++;
	  }
	  flag_earse_ok = TRUE;
	  lvq_ota_setcallback_updatefirmware();
	  lvq_ota_run();
	}
  }
}

void start_up_firmware_update(void)
{
  NVIC_DisableIRQ(USART1_IRQn);
  HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);

  HAL_UART_DeInit(&huart1);
  HAL_RCC_DeInit();

  SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
  __set_MSP(*((volatile uint32_t*) ADDRESS_FIRMWARE_APPLICATION));
  uint32_t u32JumpAddress = *( (volatile uint32_t*) (ADDRESS_FIRMWARE_APPLICATION + 4) );
  void (*reset_handler) (void) = (void *) u32JumpAddress;
  reset_handler();
}
