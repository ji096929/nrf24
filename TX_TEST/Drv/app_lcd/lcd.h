#ifndef __LCD_H
#define __LCD_H		

#ifdef __cplusplus
extern "C"{
#endif
	
#include "main.h"
#include "lcd_bsp.h"
#define STM32F103



extern LCD_line_t LCD_bus_1;
extern LCD_line_t LCD_bus_2;	

void LCD_spi_RES_Clr(void);
void LCD_spi_RES_Set(void);
void LCD_spi_DC_Set(void);
void LCD_spi_DC_Clr(void);
void LCD_spi_CS_Set(void);
void LCD_spi_CS_Clr(void);
void LCD_spi_BLK_Set(void);
void LCD_spi_BLK_Clr(void);
void TFT_LCD_Init(void);
void LCD_Init(LCD_line_t *LCD_bus);

void LCD_Fill(LCD_line_t *LCD_bus,uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_DrawPoint(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t color);//在指定位置画一个点 
void LCD_DrawLine(LCD_line_t *LCD_bus,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//在指定位置画一条线
void LCD_DrawRectangle(LCD_line_t *LCD_bus,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//在指定位置画一个矩形
void Draw_Circle(LCD_line_t *LCD_bus,uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//在指定位置画一个圆

void LCD_ShowChinese(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示汉字串
void LCD_ShowChinese12x12(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个32x32汉字

void LCD_ShowChar(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示一个字符
void LCD_ShowString(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示字符串
uint32_t mypow(uint8_t m,uint8_t n);//求幂
void LCD_ShowIntNum(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示整数变量
void LCD_ShowFloatNum1(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示两位小数变量

void LCD_ShowPicture(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);//显示图片

//字库函数
void ZK_command(LCD_line_t *LCD_bus,uint8_t dat);
uint8_t get_data_from_ROM(void);
void get_n_bytes_data_from_ROM(LCD_line_t *LCD_bus,uint8_t AddrHigh,uint8_t AddrMid,uint8_t AddrLow,uint8_t *pBuff,uint8_t DataLen);
void Display_GB2312(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint16_t fc,uint16_t bc);
void Display_GB2312_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc);
void Display_Asc(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint16_t fc,uint16_t bc);
void Display_Asc_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc);
void Display_Arial_TimesNewRoman(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint16_t fc,uint16_t bc);
void Display_Arial_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc);
void Display_TimesNewRoman_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc);



	
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#ifdef __cplusplus
}
#endif

#endif





