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
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
const char *g_Ashining = "ashining";
uint8_t g_TxMode = 0, g_UartRxFlag = 0;
uint8_t g_UartRxBuffer[ 100 ] = { 0 };
uint8_t g_RF24L01RxBuffer[ 32 ] = { 0 }; 
uint8_t test_status = 0;
uint8_t tx_state = 0;
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
  MX_SPI2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	uint8_t i = 0;

	//串口初始化
//	drv_uart_init( 9600 );
	
	//延时初始化
//	drv_delay_init( );
	
	//LED初始化
	drv_led_init( );
	
	//SPI初始化
//	drv_spi_init( );
	
	//RF24L01引脚初始化
	NRF24L01_Gpio_Init( );
	
	//检测nRF24L01
	NRF24L01_check( );	

	//RF24L01初始化	
	RF24L01_Init( );
	
	//LED指示灯默认关闭，便于观察
	led_red_off( );
//	led_green_off( );
	for( i = 0; i < 6; i++ )
	{
		led_red_flashing( );
//		led_green_flashing( );
		drv_delay_500Ms( 1 );  //检测到芯片 闪烁3个周期
	}
		
	
#ifdef	__RF24L01_TX_TEST__		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* 发送 **********************************************//
//*****************************************************************************************//
//=========================================================================================//	
	
	//按键初始化
//	drv_button_init( );
	
		//发送模式
	while( 1 )	
	{
		//模式切换
//		if( BUTOTN_PRESS_DOWN == drv_button_check( ))
//		{
////			g_TxMode = 1 - g_TxMode;		//模式会在 TX_MODE_1( 0 ),TX_MODE_2( 1 )之间切换
//			
//			//状态显示清零
//			led_green_off( );
//			led_red_off( );
//			
//			if( TX_MODE_1 == g_TxMode )
//			{
//				for( i = 0; i < 6; i++ )		
//				{
////					led_red_flashing( );	//固定发送模式，红灯闪烁3次
//					drv_delay_500Ms( 1 );		
//				}
//			}
//			else
//			{
//				for( i = 0; i < 6; i++ )
//				{
//					led_green_flashing( );	//串口发送模式，绿灯闪烁3次
//					drv_delay_500Ms( 1 );
//				}
//			}
//		}
		
		//发送
		if(tx_state == 0)
		{
			RF24L01_Set_Mode( MODE_TX );
			led_red_flashing( );	
			NRF24L01_TxPacket( (uint8_t *)g_Ashining, 8 );		//模式1发送固定字符,1S一包
			tx_state = 1;
//			drv_delay_500Ms( 1 );	
//			drv_delay_500Ms( 1 );	
		
		}
		else
		{	
//			//查询串口数据
//			i = drv_uart_rx_bytes( g_UartRxBuffer );
//			
//			if( 0 != i )
//			{
//				NRF24L01_TxPacket( g_UartRxBuffer, i );
//				led_red_flashing( );
//			}
			RF24L01_Set_Mode(MODE_RX);
			test_status = NRF24L01_RxPacket(g_RF24L01RxBuffer);
			if(0 != test_status)
			{
				led_red_flashing();
				tx_state = 0;
				test_status = 0;
				drv_delay_500Ms( 1 );	
				drv_delay_500Ms( 1 );		
//				drv_delay_500Ms( 1 );	
//				drv_delay_500Ms( 1 );					
			}
		}
	}
	
#else		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* 接收 **********************************************//
//*****************************************************************************************//
//=========================================================================================//	
	
	RF24L01_Set_Mode( MODE_RX );		//接收模式
	while( 1 )
	{
		i = NRF24L01_RxPacket( g_RF24L01RxBuffer );		//接收字节
		test_status = i;
		if( 0 != i )
		{
			led_red_flashing( );
			RF24L01_Set_Mode( MODE_TX );
			NRF24L01_TxPacket( (uint8_t *)g_Ashining, 8 );			
			i = 0;
			drv_delay_500Ms( 1 );	
			drv_delay_500Ms( 1 );		
//			drv_uart_tx_bytes( g_RF24L01RxBuffer,i);	//输出接收到的字节
			
		}
	}
		

	
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  while (1)
//  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
