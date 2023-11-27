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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include "weather_data.h"
#include "bme280_interface.h"
#include "bme280_defs.h"
#include "bh1750.h"
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
/* USER CODE BEGIN PFP */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

uint16_t read_dust(void);
void read_data(struct weather_data *data_collection);
void transfer_data(const struct weather_data *data_collection);
void delay_us(uint16_t us);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

struct weather_data data_collection =
{ .temperature = 0, .pressure = 0, .humidity = 0, .ambient_light = 0, .dust = 0,
		.rotations = 0 };

uint8_t request_data = 1;
uint32_t anemometer_rotations = 0;

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
  MX_USART1_UART_Init();
  MX_TIM14_Init();
  MX_I2C1_Init();
  MX_ADC_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim14);
	HAL_TIM_Base_Start(&htim16);
	HAL_ADC_Start(&hadc);
	if (BME280_init() != BME280_OK)
	{
		printf("Blad inicjalizacji!\n");
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		if (request_data)
		{
			read_data(&data_collection);
			transfer_data(&data_collection);
			request_data = 0;

		}
		HAL_SuspendTick();
		HAL_PWR_EnableSleepOnExit();
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_ResumeTick(); // Enable SysTick after wake-up
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		// HAL_Delay(100);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (!request_data)
		request_data = 1;
	HAL_PWR_DisableSleepOnExit();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == Hall_INT_Pin)
	{
		anemometer_rotations++;
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}
}

uint16_t read_dust(void)
{
	uint16_t dust = 0;
	for (uint8_t i = 0; i < 10; i++)
	{
		HAL_GPIO_WritePin(Sharp_LED_GPIO_Port, Sharp_LED_Pin, GPIO_PIN_RESET);
		delay_us(280);
		if (HAL_ADC_PollForConversion(&hadc, 10) == HAL_OK)
		{
			dust += HAL_ADC_GetValue(&hadc);
			HAL_ADC_Start(&hadc);
		}
		else
			i--;

		HAL_GPIO_WritePin(Sharp_LED_GPIO_Port, Sharp_LED_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
	}
	dust = (uint16_t) (dust / 10);
	return dust;
}

void read_data(struct weather_data *data_collection)
{
	uint16_t size = 0;
	if (BH1750_read_data(&*data_collection) != HAL_OK)
	{
		char data[80];
		size = sprintf(data, "Error while reading BH1750\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t*) &data, size, 100);
	}
	HAL_Delay(10);
	if (BME280_read_data(&*data_collection) != BME280_OK)
	{
		char data[80];
		size = sprintf(data, "Error while reading BME280\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t*) &data, size, 100);
	}
	data_collection->dust = read_dust();
	// checking rotations
	data_collection->rotations = anemometer_rotations;
	anemometer_rotations = 0;
}

void transfer_data(const struct weather_data *data_collection)
{
	char data[500];
	uint16_t size = 0;
	size =
			sprintf(data,
					"{\"Temperature\" : %ld, \"Pressure\" : %ld, \"Humidity\" : %ld, \"Ambient Light\" : %d, \"Dust\" : %d, \"Rotations\" : %ld}\r\n",
					data_collection->temperature, data_collection->pressure,
					data_collection->humidity, data_collection->ambient_light,
					data_collection->dust, data_collection->rotations);
	HAL_UART_Transmit(&huart1, (uint8_t*) &data, size, 200);
}

void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim16, 0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim16) < us)
		;  // wait for the counter to reach the us input in the parameter
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
