/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define READ_PIN_IT ( !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) )

#define TIME_HOLDING 	3000
#define TIME_CLICK   	500
#define TIME_RELAX		300
#define TIME_DEBOUNCE	50

enum {
	IT_Start,
	IT_End
};

typedef enum {
	LED1_2_3_OFF = 0,
	LED1_ON_LED2_3_OFF,
	LED2_ON_LED1_3_OFF,
	LED3_ON_LED1_2_OFF,
} LED_Status_t;

typedef enum {
	BUTTON_PRESS_STATUS,
	BUTTON_CLICK_STATUS,
	BUTTON_ONE_CLICK_STATUS,
	BUTTON_DOUBLE_CLICK_STATUS,
	BUTTON_HOLDING_STATUS,
	BUTTON_NOISE_STATUS,
	BUTTON_OVER_CLICK_STATUS,
	BUTTON_END_STATUS,
} BUTTON_Status_t;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define LED1_ON 	HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_RESET)
#define LED1_OFF 	HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_SET)
#define LED2_ON		HAL_GPIO_WritePin(GPIOC, LED2_Pin, GPIO_PIN_RESET)
#define LED2_OFF	HAL_GPIO_WritePin(GPIOC, LED2_Pin, GPIO_PIN_SET)
#define LED3_ON		HAL_GPIO_WritePin(GPIOC, LED3_Pin, GPIO_PIN_RESET)
#define LED3_OFF	HAL_GPIO_WritePin(GPIOC, LED3_Pin, GPIO_PIN_SET)

uint8_t u8button_status = BUTTON_END_STATUS;
uint8_t u8statusIT = IT_End;
uint8_t u8statusLED = LED1_2_3_OFF;
uint8_t u8laststatusLED = LED1_2_3_OFF;

uint8_t u8button_click = 0;
uint8_t u8button_double_click = 0;
uint8_t u8button_hold = 0;
uint32_t u32timeButton = 0;

void LED_Toggle(uint32_t count)
{
	while(count--)
	{
		LED1_ON; LED2_ON; LED3_ON;
		HAL_Delay(100);
		LED1_OFF; LED2_OFF, LED3_OFF;
		HAL_Delay(100);
	}
}

void Set_status_led(void)
{
	switch(u8statusLED)
	{
	  case LED1_2_3_OFF:
	  {
		  LED1_OFF; LED2_OFF, LED3_OFF;
		  break;
	  }
	  case LED1_ON_LED2_3_OFF:
	  {
		  LED1_ON; LED2_OFF; LED3_OFF;
		  break;
	  }
	  case LED2_ON_LED1_3_OFF:
	  {
		  LED2_ON; LED1_OFF; LED3_OFF;
		  break;
	  }
	  case LED3_ON_LED1_2_OFF:
	  {
		  LED3_ON; LED1_OFF; LED2_OFF;
		  break;
	  }
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	while( u8statusIT == IT_Start )
	{
		switch(u8button_status)
		{
			case BUTTON_PRESS_STATUS:
			{
				uwTick = 0;
				while( HAL_GetTick() < TIME_DEBOUNCE )
				{
					HAL_Delay(1);
					if( !READ_PIN_IT )
					{
						u8button_status = BUTTON_NOISE_STATUS;
						break;
					}
				}
				u8button_status = BUTTON_CLICK_STATUS;
				break;
			}
			case BUTTON_CLICK_STATUS:
			{
				uwTick = 0;
				while( READ_PIN_IT )
				{
					HAL_Delay(1);
					if( HAL_GetTick() > TIME_HOLDING )
					{
						u8button_status = BUTTON_HOLDING_STATUS;
						break;
					}
				}
				if( !READ_PIN_IT && HAL_GetTick() < TIME_CLICK )
				{
					u8button_status = BUTTON_ONE_CLICK_STATUS;
					break;
				}
				if( !READ_PIN_IT && HAL_GetTick() > TIME_CLICK && HAL_GetTick() < TIME_HOLDING )
				{
					u8button_status = BUTTON_NOISE_STATUS;
					break;
				}
				break;
			}
			case BUTTON_ONE_CLICK_STATUS:
			{
				uwTick = 0;
				while( !READ_PIN_IT )
				{
					HAL_Delay(1);
					if( HAL_GetTick() > TIME_RELAX )
					{
						u8button_click = 1;
						u8button_status = BUTTON_END_STATUS;
						break;
					}
				}
				if( READ_PIN_IT && HAL_GetTick() < TIME_RELAX )
				{
					u8button_status = BUTTON_DOUBLE_CLICK_STATUS;
					break;
				}
				break;
			}
			case BUTTON_DOUBLE_CLICK_STATUS:
			{
				uwTick = 0;
				while( READ_PIN_IT )
				{
					HAL_Delay(1);
					if( HAL_GetTick() > TIME_CLICK )
					{
						u8button_status = BUTTON_NOISE_STATUS;
						break;
					}
				}
				if( !READ_PIN_IT && HAL_GetTick() < TIME_CLICK )
				{
					u8button_status = BUTTON_OVER_CLICK_STATUS;
					break;
				}
				break;
			}
			case BUTTON_HOLDING_STATUS:
			{
				u8button_hold = 1;
				u8button_status = BUTTON_END_STATUS;
				break;
			}
			case BUTTON_NOISE_STATUS:
			{
				u8statusIT = IT_End;
				u8button_click = u8button_double_click = u8button_hold = 0;
				break;
			}
			case BUTTON_OVER_CLICK_STATUS:
			{
				uwTick = 0;
				while( !READ_PIN_IT )
				{
					HAL_Delay(1);
					if( HAL_GetTick() > 300 )
					{
						u8button_double_click = 1;
						u8button_status = BUTTON_END_STATUS;
						break;
					}
				}
				if( READ_PIN_IT && HAL_GetTick() < 500 )
				{
					u8button_status = BUTTON_NOISE_STATUS;
					break;
				}
				break;
			}
			case BUTTON_END_STATUS:
			{
				if( u8button_hold )
				{
					if( u8laststatusLED ==  LED1_2_3_OFF )
					{
						u8statusLED = LED1_ON_LED2_3_OFF;
						u8laststatusLED = u8statusLED;
						u8button_hold = 0;
					}
					else
					{
						u8statusLED = LED1_2_3_OFF;
						u8laststatusLED = u8statusLED;
						u8button_hold = 0;
					}
					Set_status_led();
					while( READ_PIN_IT )
					{
						HAL_Delay(1);
					}
				}
				else if( u8button_click && u8laststatusLED !=  LED1_2_3_OFF)
				{
					u8statusLED++;
					if( u8statusLED > 3)
					  u8statusLED = LED1_ON_LED2_3_OFF;
					u8button_click = 0;
					Set_status_led();
				}
				else if( u8button_double_click )
				{
					LED_Toggle(5);
					u8button_double_click = 0;
					Set_status_led();
				}
				u8statusIT = IT_End;
				break;
			}
		}
	}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED3_Pin|LED2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED3_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_13 )
  {
	  u8statusIT = IT_Start;
	  u8button_status = BUTTON_PRESS_STATUS;
  }
  else
  {
      __NOP();
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
