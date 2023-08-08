/**
  ******************************************************************************

  OTA For STM32F446RE
  Author:   LVQ
  Updated:  6 August 2023
  Algorithm:

  ******************************************************************************
*/

#include "lvq_stm32f4_ota.h"
#include <stdlib.h>

#define ADDRESS_FIRMWARE_APPLICATION 0x08008000U
#define STM32F446xx 1
#define USART_UD &huart1

//#define DEBUG 0

uint8_t flag_ota_update = FALSE;
uint8_t flag_size_flash = FALSE;
uint8_t flag_ota_complete = FALSE;
uint8_t flag_earse_ok = FALSE;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart1_rx;

#ifdef STM32F401xx
		uint32_t SizeCurrent = 0;
		uint8_t Index = 0;
		uint32_t MaxSizeFlash = 245760;
#elif STM32F446xx
		uint32_t SizeCurrent = 0;
		uint8_t Index = 0;
		uint32_t MaxSizeFlash = 507904;
#endif

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

/*****************************************************************/
field_hex_files_t state_file;
uint8_t g_flag_cplt_dma = FALSE;

#define RX_BUFFER_SIZE		50
uint8_t cInternalBufferData[32];
uint8_t u8data_receive_dma[RX_BUFFER_SIZE];
field_hex_files_t State;

uint8_t aRXBufferA[50];
uint8_t aRXBufferB[50];
uint8_t checkamount[50];

uint8_t aTextInfoStart[] = "\r\nUpdate firmware for stm32 bootloader uart\r\n";

__IO uint32_t     uwNbReceivedChars;
uint8_t *pBufferReadyForUser;
uint8_t *pBufferReadyForReception;

volatile uint32_t u32SizeDataFirmware = 0;
volatile uint16_t u32DataWriteFlash[50] = {0};   /* Array to store the data of line to pass it to Flash Writer */
volatile uint32_t u32AddressWriteFlash;           /* Pointer to store the address in to write */

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

void start_up_bootloader(void)
{
	HAL_DMA_UnRegisterCallback(&hdma_usart1_rx, HAL_DMA_XFER_HALFCPLT_CB_ID);
	if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(&huart1, u8data_receive_dma, RX_BUFFER_SIZE))
	{
	  Error_Handler();
	}

	pBufferReadyForReception = aRXBufferA;
	pBufferReadyForUser      = aRXBufferB;
	uwNbReceivedChars        = 0;

	HAL_UART_Transmit(&huart2, aTextInfoStart, sizeof(aTextInfoStart), 100);

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
					LVQ_Flash_Erase(LVQ_GetAddressFromSector(StartSector));;
					StartSector++;
			}
			flag_earse_ok = TRUE;
			while( flag_ota_complete == FALSE )
			{

			}
		}
	}
}

void start_up_firmware_update(void)
{
  NVIC_DisableIRQ(USART1_IRQn);
  HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);

  HAL_UART_DeInit(&huart1);
  HAL_UART_DeInit(&huart2);
  HAL_DMA_DeInit(&hdma_usart1_rx);
  HAL_RCC_DeInit();

  SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
  __set_MSP(*((volatile uint32_t*) ADDRESS_FIRMWARE_APPLICATION));
  uint32_t u32JumpAddress = *( (volatile uint32_t*) (ADDRESS_FIRMWARE_APPLICATION + 4) );
  void (*reset_handler) (void) = (void *) u32JumpAddress;
  reset_handler();
}

void Parse_Element(uint8_t *buff, uint8_t u8length)
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
        {
           return;
        }

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
				Parse_Element(&pData[u8Count], 2U);
				hex_data.u8ByteCount = cInternalBufferData[0];
				u8Count = 2U;
				State = ADDRESS;
				break;
			}
			case ADDRESS:
			{
				Parse_Element(&pData[u8Count], 4U);
				hex_data.u8Addr[0] = cInternalBufferData[0];
				hex_data.u8Addr[1] = cInternalBufferData[1];
				u8Count = 6U;
				State = RECORD_TYPE;
				break;
			}
			case RECORD_TYPE:
			{
				Parse_Element(&pData[u8Count], 2U);
				hex_data.u8RecordType = cInternalBufferData[0];
				u8Count = 8U;
				State = DATA;
				break;
			}
			case DATA:
			{
				Parse_Element(&pData[u8Count], (hex_data.u8ByteCount*2U));
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
				Parse_Element(&pData[u8Count], 2U);
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

uint8_t handle_data_receive(uint8_t* pData, uint16_t Size)
{
  return handle_data_hex(pData, Size);
}

void handle_data_receive_dma(UART_HandleTypeDef *huart, uint8_t* pData, uint16_t Size)
{
  uint8_t* pBuff = pData;
  switch( pBuff[0] )
  {
	  case OTA_UPDATE_FW:
	  {
		flag_ota_update = TRUE;
		HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
		break;
	  }
	  case OTA_CHECK_SIZE_FLASH:
	  {
        Index = 1;
        SizeCurrent = 0;
		while( pBuff[Index] != '\n' )
		{
			SizeCurrent = SizeCurrent * 10 + ( pBuff[Index] - 48 );
			Index++;
		}
		if( SizeCurrent > MaxSizeFlash )
			HAL_UART_Transmit(USART_UD, OTA_CANCEL_UPDATE, 1, 10);
		else
		{
			HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
			flag_size_flash = TRUE;
		}
		break;
	  }
	  case OTA_UPDATE_START:
	  {
	    if( flag_earse_ok == TRUE )
	    {
	    	flag_earse_ok = FALSE;
		    HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
	    }
		break;
	  }
	  case ':':
	  {
		if( ( handle_data_receive(pBuff, Size) == STATE_NO_ERRORS ) && flag_ota_update && flag_size_flash )
		{
	      write_flash_memory();
	      if( !u32AddressWriteFlash )
	    	  HAL_UART_Transmit(USART_UD, OTA_RESEND_DATA, 1, 10);
	      else
	    	  HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
		}
	    else
		  HAL_UART_Transmit(USART_UD, OTA_RESEND_DATA, 1, 10);
		break;
	  }
	  case OTA_UPDATE_FW_COMPLETE:
	  {
		flag_ota_complete = TRUE;
		HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
		break;
	  }
	  default:
		break;
  }
  free(pBuff);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if( (huart->Instance == USART1) )
  {
	static uint16_t old_pos = 0;
	static uint8_t *ptemp;
	uint16_t i;

	if( g_flag_cplt_dma )
	{
	  g_flag_cplt_dma = FALSE;
	  for (i = 0; i < Size - old_pos; i++)
	  {
		  checkamount[i] = u8data_receive_dma[old_pos + i];
	  }
	  if( handle_data_hex(checkamount, Size - old_pos) == STATE_ERRORS )
	    return;
	}

	if (Size != old_pos)
	{
	  if (Size > old_pos)
	  {
	    uwNbReceivedChars = Size - old_pos;
	    for (i = 0; i < uwNbReceivedChars; i++)
		{
			pBufferReadyForUser[i] = u8data_receive_dma[old_pos + i];
		}
	  }
	  else
	  {
	    uwNbReceivedChars = RX_BUFFER_SIZE - old_pos;
		for (i = 0; i < uwNbReceivedChars; i++)
		{
			pBufferReadyForUser[i] = u8data_receive_dma[old_pos + i];
		}
	   if (Size > 0)
	   {
	     for (i = 0; i < Size; i++)
		 {
		   pBufferReadyForUser[uwNbReceivedChars + i] = u8data_receive_dma[i];
		 }
		 uwNbReceivedChars += Size;
	   }
	  }

	  #ifdef DEBUG
	    HAL_UART_Transmit(&huart2, pBufferReadyForUser, uwNbReceivedChars, 100);
	    HAL_UART_Transmit(&huart2, (uint8_t*) "\n", 1, 100);
	  #endif
	  handle_data_receive_dma(huart, pBufferReadyForUser, uwNbReceivedChars);

	  ptemp = pBufferReadyForUser;
	  pBufferReadyForUser = pBufferReadyForReception;
	  pBufferReadyForReception = ptemp;
   }
   old_pos = Size;
  }
}
