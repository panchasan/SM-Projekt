/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bmp280.h"
#include "bmp280_defs.h"
#include "bmp280_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BMP280_SPI (&hspi4)
#define BMP280_CS1 1
#define BMP280_CS2 2

#define enc_counter_max 3999  // max temperature
#define enc_counter_min 1499   // min temperature
#define enc_counter_step 25   // step of temperature 0.25degree

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

char buffor[100];
char rx_buffer[15];


uint32_t enc_counter = enc_counter_min;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Usart */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3)
	{
 	 HAL_UART_Receive_IT(&huart3, (uint8_t*)rx_buffer, 4);
	}
 }

 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 {
	 if(huart == &huart3){
		 HAL_UART_Transmit_IT(&huart3, (uint8_t*)rx_buffer, 4);
	 }
 }

 /* Encoder */
  void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
  {
  	if(GPIO_Pin == ENC_CLK_Pin)
  	{
  		if(HAL_GPIO_ReadPin(ENC_DT_GPIO_Port, ENC_DT_Pin) == GPIO_PIN_RESET)
  		{
  			enc_counter = (enc_counter >= enc_counter_max) ? enc_counter_max: (enc_counter + enc_counter_step);
  		}
  		else
  		{
  			enc_counter = (enc_counter <= enc_counter_min) ? enc_counter_min: (enc_counter - enc_counter_step);
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
	struct bmp280_uncomp_data bmp280_1_data;

	int32_t temp32, temp32_2;
	double temp;
	float temperature;

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
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  MX_SPI4_Init();
  /* USER CODE BEGIN 2 */
   int8_t rslt;
    struct bmp280_config conf;

    rslt = bmp280_init(&bmp280_1);

    /*Always read the current settings before writing especially when all the configuration is not modified */
    rslt = bmp280_get_config(&conf, &bmp280_1);

    /* congiguring the temperature oversampling, filter coefficient and output data rate */
    /* Overwrite the desired settings */

    conf.filter = BMP280_FILTER_OFF;

    /* Temperature oversampling set at 1x */
    conf.os_temp = BMP280_OS_1X;

    /* Pressure oversampling set at 1x */
    conf.os_pres = BMP280_OS_1X;

    /* Setting the output data rate as 1 Hz (1000ms) */
    conf.odr = BMP280_ODR_1000_MS;

    rslt = bmp280_set_config(&conf, &bmp280_1);

    /* Always set the power mode after setting the configuration */
    rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp280_1);

    /* Receive massage about value of temperature */
    HAL_UART_Receive_IT(&huart3, (uint8_t*)rx_buffer, 4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  rslt = bmp280_get_uncomp_data(&bmp280_1_data, &bmp280_1);

	  rslt = bmp280_get_comp_temp_32bit(&temp32, bmp280_1_data.uncomp_temp, &bmp280_1);

	  rslt = bmp280_get_comp_temp_double(&temp, bmp280_1_data.uncomp_temp, &bmp280_1);
	  temperature = (float)temp32*0.01;

	  /* Sending the temperature measured by the sensor
	  	   * Maciej jak już bedziesz miał to LCD to tuaj możesz tego sprintf uzyc bu jakoś pokaza cstopnie celcjusza
	  	   * zmienna temperatura to jest ta z czujnika, ona ma postać XX.XXstopni, np 25.02 stopnia
	  	   * */

	  //sprintf((char*)buffor, "Temperatura: %.2f \r\n", temperature);
	  //HAL_UART_Transmit(&huart3, (uint8_t*)buffor, strlen(buffor), 1000);

	  bmp280_1.delay_ms(1000);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
