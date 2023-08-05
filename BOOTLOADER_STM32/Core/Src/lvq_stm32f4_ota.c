/**
  ******************************************************************************

  OTA For STM32F446RE
  Author:   LVQ
  Updated:  24January 2023
	Algorithm: 

  ******************************************************************************
*/

#include "lvq_stm32f4_ota.h"
#define STM32F446xx 1
#define USART_UD &huart1

uint8_t FlagOTA = FALSE;
uint8_t rx_data = 0;

extern UART_HandleTypeDef huart1;

LVQ_USART_Callback_t LVQ_USART_Callback = NULL;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		LVQ_USART_Callback(rx_data);
		HAL_UART_Receive_IT(USART_UD,&rx_data,1);
	}
}

void LVQ_USART_Set_Callback(void* cb)
{
		LVQ_USART_Callback = cb;
}

void LVQ_OTA_SetHighAddress(void);
void LVQ_OTA_WriteFlashMemory(void);

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

void LVQ_OTA_Init(void)
{
	HAL_UART_Receive_IT(USART_UD,&rx_data,1);
}

void LVQ_OTA_SetCallback_CheckUpdateFirmware(void)
{
	LVQ_USART_Set_Callback(LVQ_OTA_CheckUpdate);
}

void LVQ_OTA_SetCallback_UpdateFirmware(void)
{
	LVQ_USART_Set_Callback(LVQ_OTA_CharReceived);
}

volatile uint8_t u8RecieveBuffer[100];            /* Array to store the received line */
volatile uint8_t u8RecieveCounter = 0;            /* Counter for received line chars */
volatile uint32_t u32SizeDataFirmware = 0;
volatile uint16_t u32DataWriteFlash[100] = {0};   /* Array to store the data of line to pass it to Flash Writer */
volatile uint32_t u32AddressWriteFlash;           /* Pointer to store the address in to write */

volatile uint8_t u8LineReceivedFlag = FALSE;      /* Flag to check if the NodeMCU finished sending a complete line */
volatile uint8_t u8FinishReceiveFlag = FALSE;     /* Flag to check if NodeMCU finished sending the code to run application code */

void LVQ_OTA_Run(void)
{
    while (u8FinishReceiveFlag == FALSE)
    {

        if (u8LineReceivedFlag == TRUE)
        {
            switch (u8RecieveBuffer[u8RecieveCounter])
            {
				case OTA_DATA_START_CHAR:
						u8RecieveCounter = 0;
						HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
						break;
				case OTA_LINE_BREAK_CHAR:
						LVQ_OTA_WriteFlashMemory();
						u8RecieveCounter = 0;
						if( u32AddressWriteFlash )
							HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
						else
							HAL_UART_Transmit(USART_UD, (uint8_t*) "STM32 Flash Address Error !!!\r\n", 31, 100);
						break;
				default:
						break;
            }
            u8LineReceivedFlag = FALSE;
        }
    }
		
    HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
		
	/* NVIC_SystemReset(); */
}

void LVQ_OTA_CharReceived(uint8_t rec)
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

void LVQ_OTA_CheckUpdate(uint8_t rec)
{
		switch(rec)
		{
				case 'S':
					break;
				case 'U':
					FlagOTA = TRUE;
					HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
					break;
				case '\n':
					for( uint8_t i = 0; i < Index; i++)
						SizeCurrent = SizeCurrent * 10 + ( Size[i] - 48 );
					if( SizeCurrent > MaxSizeFlash )
						HAL_UART_Transmit(USART_UD, OTA_CANCEL_UPDATE, 1, 10);
					else
					{
						HAL_UART_Transmit(USART_UD, OTA_READ_CONFIRM_CHAR, 1, 10);
						HAL_UART_Transmit(USART_UD, (uint8_t*) "\r\nSTM32 Flash: ", 13, 100);
						sprintf(TX, "%ld", SizeCurrent);
						HAL_UART_Transmit(USART_UD, (uint8_t*) TX, sizeof(TX), 100);
						HAL_UART_Transmit(USART_UD, (uint8_t*) " Bytes\r\n", 7, 100);
					}
					break;
				default:
					Size[Index++] = rec;
					break;
		}
}

static uint8_t getHex(uint8_t Copy_uint8_tAsci)
{
    uint8_t Result = 0;

    /*0 ... 9*/
    if ((Copy_uint8_tAsci >= 48) && (Copy_uint8_tAsci <= 57))
    {
        Result = Copy_uint8_tAsci - 48;
    }

    /*A ... F*/
    else if ((Copy_uint8_tAsci >= 65) && (Copy_uint8_tAsci <= 70))
    {
        Result = Copy_uint8_tAsci - 55;
    }

    return Result;
}

void LVQ_OTA_WriteFlashMemory(void)
{
    uint8_t CC, i;
    uint8_t u8DataDigits[4];
    uint8_t u8DataCounter = 0;

    switch (getHex(u8RecieveBuffer[8]))
    {

			case 4: /* Extended Linear Address Record: used to identify the extended linear address  */
					LVQ_OTA_SetHighAddress();
					break;

			case 5: /* Start Linear Address Record: the address where the program starts to run      */
					break;

			case 0: /* Data Rrecord: used to record data, most records of HEX files are data records */

					/* LVQ_GPIO_TogglePinValue(GPIOA, GPIO_PIN_5); */
					/* Get Character Count */
					CC = (getHex(u8RecieveBuffer[1]) << 4) | getHex(u8RecieveBuffer[2]);
					u32SizeDataFirmware += CC;

					/* Get low part of address */
					u8DataDigits[0] = getHex(u8RecieveBuffer[3]);
					u8DataDigits[1] = getHex(u8RecieveBuffer[4]);
					u8DataDigits[2] = getHex(u8RecieveBuffer[5]);
					u8DataDigits[3] = getHex(u8RecieveBuffer[6]);

					/* Set full address */
					u32AddressWriteFlash = u32AddressWriteFlash & 0xFFFF0000;
					u32AddressWriteFlash = u32AddressWriteFlash |
										 (u8DataDigits[3])      |
										 (u8DataDigits[2] << 4) |
										 (u8DataDigits[1] << 8) |
										 (u8DataDigits[0] << 12);
					/* Check address wite flash data */
					if( u32AddressWriteFlash < 0x08004000 )
					{
							u32AddressWriteFlash = 0;
							return;
					}
				
					/* Get the data of the record */
					for (i = 0; i < CC / 2; i++)
					{
							u8DataDigits[0] = getHex(u8RecieveBuffer[4 * i + 9]);
							u8DataDigits[1] = getHex(u8RecieveBuffer[4 * i + 10]);
							u8DataDigits[2] = getHex(u8RecieveBuffer[4 * i + 11]);
							u8DataDigits[3] = getHex(u8RecieveBuffer[4 * i + 12]);
							u32DataWriteFlash[u8DataCounter] = (u8DataDigits[3] << 8)  |
															   (u8DataDigits[2] << 12) |
															   (u8DataDigits[1] << 0)  |
															   (u8DataDigits[0] << 4);
							u8DataCounter++;
					}

					if (CC % 2 != 0)
					{
							u8DataDigits[0] = getHex(u8RecieveBuffer[4 * (CC / 2) + 9]);
							u8DataDigits[1] = getHex(u8RecieveBuffer[4 * (CC / 2) + 10]);
							u32DataWriteFlash[u8DataCounter] = 0xFF00 | (u8DataDigits[0] << 4) | (u8DataDigits[1] << 0);
							LVQ_Flash_Write(u32AddressWriteFlash, (uint32_t *)u32DataWriteFlash, CC / 4 + 1);
					}
					else
					{
							LVQ_Flash_Write(u32AddressWriteFlash, (uint32_t *)u32DataWriteFlash, CC / 4);
					}

					break;
			default:
					break;
    }
}

void LVQ_OTA_SetHighAddress(void)
{
	  /*
    u32AddressWriteFlash = (getHex(u8RecieveBuffer[9]) << 28)  |
													 (getHex(u8RecieveBuffer[10]) << 24) |
													 (getHex(u8RecieveBuffer[11]) << 20) |
													 (getHex(u8RecieveBuffer[12]) << 16);
	  */
	
		u32AddressWriteFlash = 0x08000000; /* Address Save Firmware Update */
}


