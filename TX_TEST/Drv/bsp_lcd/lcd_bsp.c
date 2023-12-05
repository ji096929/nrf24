#include "lcd_init.h" 
#include "main.h"
#include "lcd.h"
#include "spi.h"

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint8_t dat) 
{		
	LCD_spi_CS_Clr();
	//SPI1_ReadWriteByte(dat);
	ctx->tx_cmd(LCD_bus->handle,dat);
	//HAL_SPI_Transmit(LCD_bus->handle,&dat,sizeof(dat),1);
	LCD_spi_CS_Set();	
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint8_t dat)
{
	LCD_Writ_Bus(ctx,LCD_bus,dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint16_t dat)
{
	LCD_Writ_Bus(ctx,LCD_bus,dat>>8);
	LCD_Writ_Bus(ctx,LCD_bus,dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint8_t dat)
{
	LCD_spi_DC_Clr();//写命令
	LCD_Writ_Bus(ctx,LCD_bus,dat);
	LCD_spi_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(ctx,LCD_bus,0x2a);//列地址设置
		LCD_WR_DATA(ctx,LCD_bus,x1);
		LCD_WR_DATA(ctx,LCD_bus,x2);
		LCD_WR_REG(ctx,LCD_bus,0x2b);//行地址设置
		LCD_WR_DATA(ctx,LCD_bus,y1);
		LCD_WR_DATA(ctx,LCD_bus,y2);
		LCD_WR_REG(ctx,LCD_bus,0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(ctx,LCD_bus,0x2a);//列地址设置
		LCD_WR_DATA(ctx,LCD_bus,x1);
		LCD_WR_DATA(ctx,LCD_bus,x2);
		LCD_WR_REG(ctx,LCD_bus,0x2b);//行地址设置
		LCD_WR_DATA(ctx,LCD_bus,y1);
		LCD_WR_DATA(ctx,LCD_bus,y2);
		LCD_WR_REG(ctx,LCD_bus,0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(ctx,LCD_bus,0x2a);//列地址设置
		LCD_WR_DATA(ctx,LCD_bus,x1);
		LCD_WR_DATA(ctx,LCD_bus,x2);
		LCD_WR_REG(ctx,LCD_bus,0x2b);//行地址设置
		LCD_WR_DATA(ctx,LCD_bus,y1);
		LCD_WR_DATA(ctx,LCD_bus,y2);
		LCD_WR_REG(ctx,LCD_bus,0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(ctx,LCD_bus,0x2a);//列地址设置
		LCD_WR_DATA(ctx,LCD_bus,x1);
		LCD_WR_DATA(ctx,LCD_bus,x2);
		LCD_WR_REG(ctx,LCD_bus,0x2b);//行地址设置
		LCD_WR_DATA(ctx,LCD_bus,y1);
		LCD_WR_DATA(ctx,LCD_bus,y2);
		LCD_WR_REG(ctx,LCD_bus,0x2c);//储存器写
	}
}








