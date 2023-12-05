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
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "drv_RF24L01.h"
#include "drv_spi.h"
#include "debug.h"
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t a = 0;
uint8_t b = 0;
uint8_t count =0 ;
	int16_t test1=10;
	uint16_t test2=10;
	float test3=10.66f;
	int16_t test4=10;
	int16_t test5[6]={1,2,3,4,5,6};
	int time=20;
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim2);
  RF24L01_Port_Init();
  // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
  // NRF24L01_Read_Status_Register(&NRF24L01_rtx);
//	HAL_Delay(1100);
  RF24L01_Init(&NRF24L01_rtx);
  //NRF24L01_Set_Speed(&NRF24L01_rtx,SPEED_1M);
    NRF24L01_Set_Power(&NRF24L01_rtx, PWR_18DB);

      NRF24L01_Write_Reg(&NRF24L01_rtx,RF_SETUP ,0x06 );
			  RF24L01_Set_Mode(&NRF24L01_rtx, MODE_TX);
    //NRF24L01_Write_Reg(&NRF24L01_rtx, CONFIG,0x02 );
    // 0x03接收，0x02发送
    //RF24L01_SET_CE_HIGH();
    
  NRF24L01_bus.TxPacket.Txlength = 32;
 
//  NRF24L01_bus.TxPacket.Txbuffer[2]=123;
//  NRF24L01_bus.TxPacket.Txbuffer[3]=121;
//  NRF24L01_bus.TxPacket.Txbuffer[5]=13;
   //NRF24L01_bus.TxPacket.Txbuffer[1]=1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	extern TransmitPacket transmit_packet;
	InitPacket();
	AddParameter_i("test_1",&test1,&transmit_packet);
	AddParameter_ui("test_2",&test2,&transmit_packet);
	AddParameter_f("test_3",&test3,&transmit_packet);
	AddParameter_i("test_4",&test4,&transmit_packet);
	AddParameter_i("test_5",test5,&transmit_packet);
	AddParameter_i("test_6",test5+1,&transmit_packet);
	AddParameter_i("test_7",test5+2,&transmit_packet);
	AddParameter_i("test_8",test5+3,&transmit_packet);
	AddParameter_i("test_9",test5+4,&transmit_packet);
	AddParameter_i("test_10",test5+5,&transmit_packet);
  while (1)
  {
		test1++;
		if(test1>100) test1-=100;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		TransmitParameter();
		HAL_Delay(time);
	 // NRF24L01_check(&NRF24L01_rtx);
	 // HAL_Delay(10);
	 // count++;
	 // NRF24L01_bus.TxPacket.Txbuffer[1]=count;
    //NRF24L01_RxPacket(&NRF24L01_rtx, &NRF24L01_bus);
    //NRF24L01_TxPacket(&NRF24L01_rtx,&NRF24L01_bus);
    a = NRF24L01_Read_Reg(&NRF24L01_rtx, STATUS);
    b =NRF24L01_bus.RxPacket.Rxbuffer[1];
   
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  
    if (htim == (&htim1))
  {
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&ADC_Value,20);
	//NRF_Intercommunication( NRF24L01_rtx, NRF24L01_bus );
  }
  
      if (htim == (&htim2))
  {
   
    	//NRF_Intercommunication( NRF24L01_rtx, NRF24L01_bus );
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
