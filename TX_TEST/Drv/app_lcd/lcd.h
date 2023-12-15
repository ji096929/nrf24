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
void LCD_DrawPoint(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t color);//��ָ��λ�û�һ���� 
void LCD_DrawLine(LCD_line_t *LCD_bus,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//��ָ��λ�û�һ����
void LCD_DrawRectangle(LCD_line_t *LCD_bus,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//��ָ��λ�û�һ������
void Draw_Circle(LCD_line_t *LCD_bus,uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//��ָ��λ�û�һ��Բ

void LCD_ShowChinese(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ���ִ�
void LCD_ShowChinese12x12(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����12x12����
void LCD_ShowChinese16x16(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����16x16����
void LCD_ShowChinese24x24(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����24x24����
void LCD_ShowChinese32x32(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����32x32����

void LCD_ShowChar(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾһ���ַ�
void LCD_ShowString(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ�ַ���
uint32_t mypow(uint8_t m,uint8_t n);//����
void LCD_ShowIntNum(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��������
void LCD_ShowFloatNum1(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��λС������

void LCD_ShowPicture(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);//��ʾͼƬ

//�ֿ⺯��
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
void ShowValue(void);

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
#define NAME_X 		1
#define ID_X		49
#define VALUE_X		80
#define NORMAL_SIZE	12



#ifdef __cplusplus
}
#endif

#endif





