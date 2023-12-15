

#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__


#include "main.h"
#include "drv_RF24L01.h"

//SPI 片选操作
		
#define spi_set_nss_high()			HAL_GPIO_WritePin(SPI_NSS_GPIO_PORT,SPI_NSS_GPIO_PIN,GPIO_PIN_SET)				//片选置高
#define spi_set_nss_low()			  HAL_GPIO_WritePin(SPI_NSS_GPIO_PORT,SPI_NSS_GPIO_PIN,GPIO_PIN_RESET)	    //片选置低


uint8_t drv_spi_read_write_byte( SPI_HandleTypeDef *hspi,uint8_t TxByte );
void drv_spi_read_write_string(SPI_HandleTypeDef *hspi, uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );

#endif

