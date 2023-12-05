#include "lcd_init.h" 
#include "main.h"
#include "lcd.h"
#include "spi.h"

/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
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
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint8_t dat)
{
	LCD_Writ_Bus(ctx,LCD_bus,dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint16_t dat)
{
	LCD_Writ_Bus(ctx,LCD_bus,dat>>8);
	LCD_Writ_Bus(ctx,LCD_bus,dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint8_t dat)
{
	LCD_spi_DC_Clr();//д����
	LCD_Writ_Bus(ctx,LCD_bus,dat);
	LCD_spi_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(LCD_ctx_t *ctx,LCD_line_t *LCD_bus,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(ctx,LCD_bus,0x2a);//�е�ַ����
		LCD_WR_DATA(ctx,LCD_bus,x1);
		LCD_WR_DATA(ctx,LCD_bus,x2);
		LCD_WR_REG(ctx,LCD_bus,0x2b);//�е�ַ����
		LCD_WR_DATA(ctx,LCD_bus,y1);
		LCD_WR_DATA(ctx,LCD_bus,y2);
		LCD_WR_REG(ctx,LCD_bus,0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(ctx,LCD_bus,0x2a);//�е�ַ����
		LCD_WR_DATA(ctx,LCD_bus,x1);
		LCD_WR_DATA(ctx,LCD_bus,x2);
		LCD_WR_REG(ctx,LCD_bus,0x2b);//�е�ַ����
		LCD_WR_DATA(ctx,LCD_bus,y1);
		LCD_WR_DATA(ctx,LCD_bus,y2);
		LCD_WR_REG(ctx,LCD_bus,0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(ctx,LCD_bus,0x2a);//�е�ַ����
		LCD_WR_DATA(ctx,LCD_bus,x1);
		LCD_WR_DATA(ctx,LCD_bus,x2);
		LCD_WR_REG(ctx,LCD_bus,0x2b);//�е�ַ����
		LCD_WR_DATA(ctx,LCD_bus,y1);
		LCD_WR_DATA(ctx,LCD_bus,y2);
		LCD_WR_REG(ctx,LCD_bus,0x2c);//������д
	}
	else
	{
		LCD_WR_REG(ctx,LCD_bus,0x2a);//�е�ַ����
		LCD_WR_DATA(ctx,LCD_bus,x1);
		LCD_WR_DATA(ctx,LCD_bus,x2);
		LCD_WR_REG(ctx,LCD_bus,0x2b);//�е�ַ����
		LCD_WR_DATA(ctx,LCD_bus,y1);
		LCD_WR_DATA(ctx,LCD_bus,y2);
		LCD_WR_REG(ctx,LCD_bus,0x2c);//������д
	}
}








