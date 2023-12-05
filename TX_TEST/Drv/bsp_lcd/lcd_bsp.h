#ifndef __LCD_BSP_H
#define __LCD_BSP_H 

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"
#include "stm32f1xx_hal_gpio_ex.h"


#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif
typedef uint8_t (*LCD_tx_ptr)(void *,uint8_t);
typedef uint8_t (*LCD_rx_ptr)(void *,uint8_t);

typedef struct
{
	uint8_t rxdata;
}LCD_feedback_t;

typedef struct
{
	uint8_t txdata;
} LCD_status_t;

typedef struct
{
	uint16_t	cmd;
} LCD_command_t;

typedef struct
{
	uint8_t SPI_ID;
} LCD_parameter_t;
typedef struct
{
	LCD_feedback_t feedback;
	LCD_status_t status;
	LCD_command_t command;
	LCD_parameter_t parameter;
}LCD_t;

typedef struct
{
	LCD_t SPI_trans;
	void *handle;
}LCD_line_t;

typedef struct
{
	LCD_tx_ptr tx_cmd;
	
	void *handle;
}LCD_ctx_t;
//-----------------LCD端口定义---------------- 


#define ZK_CS_Clr()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)//CS2
#define ZK_CS_Set()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)


extern uint8_t SPI1_ReadWriteByte(uint8_t TxData);
void LCD_Writ_Bus(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint8_t dat);//写入一个字节
void LCD_WR_DATA(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint16_t dat);//写入两个字节
void LCD_WR_REG(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint8_t dat);//写入一个指令
void LCD_Address_Set(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数

#ifdef __cplusplus
}
#endif

#endif




