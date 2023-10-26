/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   主函数C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "main.h"				//main.h 中含有TX/RX、软件SPI/硬件SPI选择配置选项


const char *g_Ashining = "ashining";
uint8_t g_TxMode = 0, g_UartRxFlag = 0;
uint8_t g_UartRxBuffer[ 100 ] = { 0 };
uint8_t g_RF24L01RxBuffer[ 32 ] = { 0 }; 
uint8_t test_status = 0;
uint8_t tx_state = 0;


/**
  * @brief :主函数 
  * @param :无
  * @note  :无
  * @retval:无
  */ 
int main( void )
{	

	uint8_t i = 0;

	//串口初始化
	drv_uart_init( 9600 );
	
	//延时初始化
	drv_delay_init( );
	
	//LED初始化
	drv_led_init( );
	
	//SPI初始化
	drv_spi_init( );
	
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
	drv_button_init( );
	
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
				drv_delay_500Ms( 1 );	
				drv_delay_500Ms( 1 );					
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
	
}

