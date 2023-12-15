#include "lcd.h"
#include "lcd_bsp.h"
#include "lcdfont.h"
#include "debug.h"
#if defined(STM32F103)
#include "spi.h"
#include "gpio.h"
#endif

#if defined(STM32F103)
LCD_line_t LCD_bus_1;
LCD_line_t LCD_bus_2;
#endif

#if defined(STM32F103)
#define LCD_BUS_1 hspi1
#define LCD_BUS_2 hspi2
#endif

#if defined(STM32F103)
#define RES_GPIO GPIOA
#define RES_PIN GPIO_PIN_8

#define DC_GPIO GPIOA
#define DC_PIN GPIO_PIN_9

#define CS_GPIO GPIOA
#define CS_PIN GPIO_PIN_10

#define BLK_GPIO GPIOA
#define BLK_PIN GPIO_PIN_11
#endif

static uint8_t LCD_spi_trans(void *handle, uint8_t aData);

LCD_ctx_t LCD_ctx;
void TFT_LCD_Init(void)
{
	LCD_ctx.tx_cmd = LCD_spi_trans;
#if defined(STM32F103)
	// LCD_bus_1.handle=&LCD_BUS_1;
	// ������ע��,���ʹ����SPI2������
	LCD_bus_2.handle = &LCD_BUS_2;
#endif
}

void LCD_Init(LCD_line_t *LCD_bus)
{
	// MX_GPIO_Init();//��ʼ��GPIO
	TFT_LCD_Init();
	LCD_ctx.handle = &LCD_bus->handle;
	LCD_spi_RES_Clr(); // ��λ
	HAL_Delay(100);
	LCD_spi_RES_Set();
	HAL_Delay(100);

	LCD_spi_BLK_Set(); // �򿪱���
	HAL_Delay(100);

	//************* Start Initial Sequence **********//
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0x11); // Sleep out
	HAL_Delay(120);						 // Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xB1);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x05);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3C);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3C);
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xB2);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x05);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3C);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3C);
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xB3);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x05);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3C);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3C);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x05);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3C);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3C);
	//------------------------------------End ST7735S Frame Rate---------------------------------//
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xB4); // Dot inversion
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x03);
	//------------------------------------ST7735S Power Sequence---------------------------------//
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xC0);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x28);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x08);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x04);
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xC1);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0XC0);
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xC2);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x0D);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x00);
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xC3);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x8D);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x2A);
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xC4);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x8D);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xC5); // VCOM
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x1A);
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0x36); // MX, MY, RGB mode
	if (USE_HORIZONTAL == 0)
		LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x00);
	else if (USE_HORIZONTAL == 1)
		LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0xC0);
	else if (USE_HORIZONTAL == 2)
		LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x70);
	else
		LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0xA0);
	//------------------------------------ST7735S Gamma Sequence---------------------------------//
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xE0);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x04);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x22);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x07);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x0A);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x2E);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x30);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x25);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x2A);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x28);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x26);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x2E);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3A);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x00);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x01);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x03);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x13);
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0xE1);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x04);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x16);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x06);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x0D);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x2D);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x26);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x23);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x27);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x27);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x25);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x2D);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x3B);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x00);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x01);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x04);
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------//
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0x3A); // 65k mode
	LCD_WR_DATA8(&LCD_ctx, LCD_bus, 0x05);
	LCD_WR_REG(&LCD_ctx, LCD_bus, 0x29); // Display on
	LCD_Fill(LCD_bus, 0, 0, LCD_W, LCD_H, WHITE);
}

/******************************************************************************
	  ����˵������ָ�����������ɫ
	  ������ݣ�xsta,ysta   ��ʼ����
				xend,yend   ��ֹ����
								color       Ҫ������ɫ
	  ����ֵ��  ��
******************************************************************************/
void LCD_Fill(LCD_line_t *LCD_bus, uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint16_t i, j;
	LCD_Address_Set(&LCD_ctx, LCD_bus, xsta, ysta, xend - 1, yend - 1); // ������ʾ��Χ
	for (i = ysta; i < yend; i++)
	{
		for (j = xsta; j < xend; j++)
		{
			LCD_WR_DATA(&LCD_ctx, LCD_bus, color);
		}
	}
}

/******************************************************************************
	  ����˵������ָ��λ�û���
	  ������ݣ�x,y ��������
				color �����ɫ
	  ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint16_t color)
{
	LCD_ctx.handle = LCD_bus->handle;
	LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x, y); // ���ù��λ��
	LCD_WR_DATA(&LCD_ctx, LCD_bus, color);
}

/******************************************************************************
	  ����˵��������
	  ������ݣ�x1,y1   ��ʼ����
				x2,y2   ��ֹ����
				color   �ߵ���ɫ
	  ����ֵ��  ��
******************************************************************************/
void LCD_DrawLine(LCD_line_t *LCD_bus, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; // ������������
	delta_y = y2 - y1;
	uRow = x1; // �����������
	uCol = y1;
	if (delta_x > 0)
		incx = 1; // ���õ�������
	else if (delta_x == 0)
		incx = 0; // ��ֱ��
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; // ˮƽ��
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; // ѡȡ��������������
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		LCD_DrawPoint(LCD_bus, uRow, uCol, color); // ����
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

/******************************************************************************
	  ����˵����������
	  ������ݣ�x1,y1   ��ʼ����
				x2,y2   ��ֹ����
				color   ���ε���ɫ
	  ����ֵ��  ��
******************************************************************************/
void LCD_DrawRectangle(LCD_line_t *LCD_bus, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	LCD_ctx.handle = LCD_bus->handle;
	LCD_DrawLine(LCD_bus, x1, y1, x2, y1, color);
	LCD_DrawLine(LCD_bus, x1, y1, x1, y2, color);
	LCD_DrawLine(LCD_bus, x1, y2, x2, y2, color);
	LCD_DrawLine(LCD_bus, x2, y1, x2, y2, color);
}

/******************************************************************************
	  ����˵������Բ
	  ������ݣ�x0,y0   Բ������
				r       �뾶
				color   Բ����ɫ
	  ����ֵ��  ��
******************************************************************************/
void Draw_Circle(LCD_line_t *LCD_bus, uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	LCD_ctx.handle = LCD_bus->handle;
	int a, b;
	a = 0;
	b = r;
	while (a <= b)
	{
		LCD_DrawPoint(LCD_bus, x0 - b, y0 - a, color); // 3
		LCD_DrawPoint(LCD_bus, x0 + b, y0 - a, color); // 0
		LCD_DrawPoint(LCD_bus, x0 - a, y0 + b, color); // 1
		LCD_DrawPoint(LCD_bus, x0 - a, y0 - b, color); // 2
		LCD_DrawPoint(LCD_bus, x0 + b, y0 + a, color); // 4
		LCD_DrawPoint(LCD_bus, x0 + a, y0 - b, color); // 5
		LCD_DrawPoint(LCD_bus, x0 + a, y0 + b, color); // 6
		LCD_DrawPoint(LCD_bus, x0 - b, y0 + a, color); // 7
		a++;
		if ((a * a + b * b) > (r * r)) // �ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}

/******************************************************************************
	  ����˵������ʾ���ִ�
	  ������ݣ�x,y��ʾ����
				*s Ҫ��ʾ�ĺ��ִ�
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				sizey �ֺ� ��ѡ 16 24 32
				mode:  0�ǵ���ģʽ  1����ģʽ
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	while (*s != 0)
	{
		if (sizey == 12)
			LCD_ShowChinese12x12(LCD_bus, x, y, s, fc, bc, sizey, mode);
		else if (sizey == 16)
			LCD_ShowChinese16x16(LCD_bus, x, y, s, fc, bc, sizey, mode);
		else if (sizey == 24)
			LCD_ShowChinese24x24(LCD_bus, x, y, s, fc, bc, sizey, mode);
		else if (sizey == 32)
			LCD_ShowChinese32x32(LCD_bus, x, y, s, fc, bc, sizey, mode);
		else
			return;
		s += 2;
		x += sizey;
	}
}

/******************************************************************************
	  ����˵������ʾ����12x12����
	  ������ݣ�x,y��ʾ����
				*s Ҫ��ʾ�ĺ���
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				sizey �ֺ�
				mode:  0�ǵ���ģʽ  1����ģʽ
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese12x12(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t i, j, m = 0;
	uint16_t k;
	uint16_t HZnum;		  // ������Ŀ
	uint16_t TypefaceNum; // һ���ַ���ռ�ֽڴ�С
	uint16_t x0 = x;
	TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

	HZnum = sizeof(tfont12) / sizeof(typFNT_GB12); // ͳ�ƺ�����Ŀ
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1)))
		{
			LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizey - 1, y + sizey - 1);
			for (i = 0; i < TypefaceNum; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) // �ǵ��ӷ�ʽ
					{
						if (tfont12[k].Msk[i] & (0x01 << j))
							LCD_WR_DATA(&LCD_ctx, LCD_bus, fc);
						else
							LCD_WR_DATA(&LCD_ctx, LCD_bus, bc);
						m++;
						if (m % sizey == 0)
						{
							m = 0;
							break;
						}
					}
					else // ���ӷ�ʽ
					{
						if (tfont12[k].Msk[i] & (0x01 << j))
							LCD_DrawPoint(LCD_bus, x, y, fc); // ��һ����
						x++;
						if ((x - x0) == sizey)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}

/******************************************************************************
	  ����˵������ʾ����16x16����
	  ������ݣ�x,y��ʾ����
				*s Ҫ��ʾ�ĺ���
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				sizey �ֺ�
				mode:  0�ǵ���ģʽ  1����ģʽ
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese16x16(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t i, j, m = 0;
	uint16_t k;
	uint16_t HZnum;		  // ������Ŀ
	uint16_t TypefaceNum; // һ���ַ���ռ�ֽڴ�С
	uint16_t x0 = x;
	TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
	HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); // ͳ�ƺ�����Ŀ
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
		{
			LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizey - 1, y + sizey - 1);
			for (i = 0; i < TypefaceNum; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) // �ǵ��ӷ�ʽ
					{
						if (tfont16[k].Msk[i] & (0x01 << j))
							LCD_WR_DATA(&LCD_ctx, LCD_bus, fc);
						else
							LCD_WR_DATA(&LCD_ctx, LCD_bus, bc);
						m++;
						if (m % sizey == 0)
						{
							m = 0;
							break;
						}
					}
					else // ���ӷ�ʽ
					{
						if (tfont16[k].Msk[i] & (0x01 << j))
							LCD_DrawPoint(LCD_bus, x, y, fc); // ��һ����
						x++;
						if ((x - x0) == sizey)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}

/******************************************************************************
	  ����˵������ʾ����24x24����
	  ������ݣ�x,y��ʾ����
				*s Ҫ��ʾ�ĺ���
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				sizey �ֺ�
				mode:  0�ǵ���ģʽ  1����ģʽ
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese24x24(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t i, j, m = 0;
	uint16_t k;
	uint16_t HZnum;		  // ������Ŀ
	uint16_t TypefaceNum; // һ���ַ���ռ�ֽڴ�С
	uint16_t x0 = x;
	TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
	HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); // ͳ�ƺ�����Ŀ
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
		{
			LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizey - 1, y + sizey - 1);
			for (i = 0; i < TypefaceNum; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) // �ǵ��ӷ�ʽ
					{
						if (tfont24[k].Msk[i] & (0x01 << j))
							LCD_WR_DATA(&LCD_ctx, LCD_bus, fc);
						else
							LCD_WR_DATA(&LCD_ctx, LCD_bus, bc);
						m++;
						if (m % sizey == 0)
						{
							m = 0;
							break;
						}
					}
					else // ���ӷ�ʽ
					{
						if (tfont24[k].Msk[i] & (0x01 << j))
							LCD_DrawPoint(LCD_bus, x, y, fc); // ��һ����
						x++;
						if ((x - x0) == sizey)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}

/******************************************************************************
	  ����˵������ʾ����32x32����
	  ������ݣ�x,y��ʾ����
				*s Ҫ��ʾ�ĺ���
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				sizey �ֺ�
				mode:  0�ǵ���ģʽ  1����ģʽ
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese32x32(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t i, j, m = 0;
	uint16_t k;
	uint16_t HZnum;		  // ������Ŀ
	uint16_t TypefaceNum; // һ���ַ���ռ�ֽڴ�С
	uint16_t x0 = x;
	TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
	HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); // ͳ�ƺ�����Ŀ
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
		{
			LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizey - 1, y + sizey - 1);
			for (i = 0; i < TypefaceNum; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) // �ǵ��ӷ�ʽ
					{
						if (tfont32[k].Msk[i] & (0x01 << j))
							LCD_WR_DATA(&LCD_ctx, LCD_bus, fc);
						else
							LCD_WR_DATA(&LCD_ctx, LCD_bus, bc);
						m++;
						if (m % sizey == 0)
						{
							m = 0;
							break;
						}
					}
					else // ���ӷ�ʽ
					{
						if (tfont32[k].Msk[i] & (0x01 << j))
							LCD_DrawPoint(LCD_bus, x, y, fc); // ��һ����
						x++;
						if ((x - x0) == sizey)
						{
							x = x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}

/******************************************************************************
	  ����˵������ʾ�����ַ�
	  ������ݣ�x,y��ʾ����
				num Ҫ��ʾ���ַ�
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				sizey �ֺ�
				mode:  0�ǵ���ģʽ  1����ģʽ
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t temp, sizex, t, m = 0;
	uint16_t i, TypefaceNum; // һ���ַ���ռ�ֽڴ�С
	uint16_t x0 = x;
	sizex = sizey / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
	num = num - ' ';														// �õ�ƫ�ƺ��ֵ
	LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizex - 1, y + sizey - 1); // ���ù��λ��
	for (i = 0; i < TypefaceNum; i++)
	{
		if (sizey == 12)
			temp = ascii_1206[num][i]; // ����6x12����
		else if (sizey == 16)
			temp = ascii_1608[num][i]; // ����8x16����
		else if (sizey == 24)
			temp = ascii_2412[num][i]; // ����12x24����
		else if (sizey == 32)
			temp = ascii_3216[num][i]; // ����16x32����
		else
			return;
		for (t = 0; t < 8; t++)
		{
			if (!mode) // �ǵ���ģʽ
			{
				if (temp & (0x01 << t))
					LCD_WR_DATA(&LCD_ctx, LCD_bus, fc);
				else
					LCD_WR_DATA(&LCD_ctx, LCD_bus, bc);
				m++;
				if (m % sizex == 0)
				{
					m = 0;
					break;
				}
			}
			else // ����ģʽ
			{
				if (temp & (0x01 << t))
					LCD_DrawPoint(LCD_bus, x, y, fc); // ��һ����
				x++;
				if ((x - x0) == sizex)
				{
					x = x0;
					y++;
					break;
				}
			}
		}
	}
}

/******************************************************************************
	  ����˵������ʾ�ַ���
	  ������ݣ�x,y��ʾ����
				*p Ҫ��ʾ���ַ���
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				sizey �ֺ�
				mode:  0�ǵ���ģʽ  1����ģʽ
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	while (*p != '\0')
	{
		LCD_ShowChar(LCD_bus, x, y, *p, fc, bc, sizey, mode);
		x += sizey / 2;
		p++;
	}
}

/******************************************************************************
	  ����˵������ʾ����
	  ������ݣ�m������nָ��
	  ����ֵ��  ��
******************************************************************************/
uint32_t mypow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}

/******************************************************************************
	  ����˵������ʾ��������
	  ������ݣ�x,y��ʾ����
				num Ҫ��ʾ��������
				len Ҫ��ʾ��λ��
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				sizey �ֺ�
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowIntNum(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t t, temp;
	uint8_t enshow = 0;
	uint8_t sizex = sizey / 2;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(LCD_bus, x + t * sizex, y, ' ', fc, bc, sizey, 0);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(LCD_bus, x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

/******************************************************************************
	  ����˵������ʾ��λС������
	  ������ݣ�x,y��ʾ����
				num Ҫ��ʾС������
				len Ҫ��ʾ��λ��
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				sizey �ֺ�
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowFloatNum1(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t t, temp, sizex;
	uint16_t num1;
	sizex = sizey / 2;
	num1 = num * 100;
	for (t = 0; t < len; t++)
	{
		temp = (num1 / mypow(10, len - t - 1)) % 10;
		if (t == (len - 2))
		{
			LCD_ShowChar(LCD_bus, x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
			t++;
			len += 1;
		}
		LCD_ShowChar(LCD_bus, x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

/******************************************************************************
	  ����˵������ʾͼƬ
	  ������ݣ�x,y�������
				length ͼƬ����
				width  ͼƬ���
				pic[]  ͼƬ����
	  ����ֵ��  ��
******************************************************************************/
void LCD_ShowPicture(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[])
{
	LCD_ctx.handle = LCD_bus->handle;
	uint16_t i, j;
	uint32_t k = 0;
	LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + length - 1, y + width - 1);
	for (i = 0; i < length; i++)
	{
		for (j = 0; j < width; j++)
		{
			LCD_WR_DATA8(&LCD_ctx, LCD_bus, pic[k * 2]);
			LCD_WR_DATA8(&LCD_ctx, LCD_bus, pic[k * 2 + 1]);
			k++;
		}
	}
}

static uint8_t LCD_spi_trans(void *handle, uint8_t aData)
{
#if defined(STM32F103)
	uint8_t status;
	LCD_spi_CS_Clr();
	status = HAL_SPI_Transmit(handle, &aData, sizeof(aData), 10);
	LCD_spi_CS_Set();
	return status;
#endif
}

void LCD_spi_RES_Clr(void)
{
#if defined(STM32F103)
	HAL_GPIO_WritePin(RES_GPIO, RES_PIN, GPIO_PIN_RESET);
#endif
}

void LCD_spi_RES_Set(void)
{
#if defined(STM32F103)
	HAL_GPIO_WritePin(RES_GPIO, RES_PIN, GPIO_PIN_SET);
#endif
}

void LCD_spi_DC_Clr(void)
{
#if defined(STM32F103)
	HAL_GPIO_WritePin(DC_GPIO, DC_PIN, GPIO_PIN_RESET);
#endif
}

void LCD_spi_DC_Set(void)
{
#if defined(STM32F103)
	HAL_GPIO_WritePin(DC_GPIO, DC_PIN, GPIO_PIN_SET);
#endif
}

void LCD_spi_CS_Clr(void)
{
#if defined(STM32F103)
	HAL_GPIO_WritePin(CS_GPIO, CS_PIN, GPIO_PIN_RESET);
#endif
}

void LCD_spi_CS_Set(void)
{
#if defined(STM32F103)
	HAL_GPIO_WritePin(CS_GPIO, CS_PIN, GPIO_PIN_SET);
#endif
}

void LCD_spi_BLK_Clr(void)
{
#if defined(STM32F103)
	HAL_GPIO_WritePin(BLK_GPIO, BLK_PIN, GPIO_PIN_RESET);
#endif
}

void LCD_spi_BLK_Set(void)
{
#if defined(STM32F103)
	HAL_GPIO_WritePin(BLK_GPIO, BLK_PIN, GPIO_PIN_SET);
#endif
}





void ShowValue(void)
{
	// 12�ֺŵ��ĸ�Ӣ����ĸռ��24�������굥λ
	LCD_ShowString(&LCD_bus_2, NAME_X, 1, "NAME", BLACK, WHITE, NORMAL_SIZE, 0);
	LCD_ShowString(&LCD_bus_2, ID_X, 1, "ID", BLACK, WHITE, NORMAL_SIZE, 0);
	LCD_ShowString(&LCD_bus_2, VALUE_X, 1, "VALUE", BLACK, WHITE, NORMAL_SIZE, 0);

       //LCD_ShowString(&LCD_bus_2, VALUE_X, 24,"150", BLACK, WHITE, NORMAL_SIZE, 0);
	LCD_ShowFloatNum1(&LCD_bus_2, VALUE_X, 24,10.1, BLACK, WHITE, NORMAL_SIZE, 0);
	for (uint8_t i = 1; i <= Value_True.num_u16; i++)
	{
		LCD_ShowString(&LCD_bus_2, NAME_X, (1 + i) * NORMAL_SIZE, Value_True.Value_True_u16[i].name, BLACK, WHITE, NORMAL_SIZE, 0);
		LCD_ShowFloatNum1(&LCD_bus_2, ID_X, (1 + i) * NORMAL_SIZE, Value_True.Value_True_u16[i].id, BLACK, WHITE, NORMAL_SIZE, 0);
		LCD_ShowFloatNum1(&LCD_bus_2, VALUE_X, (1 + i) * NORMAL_SIZE, Value_True.Value_True_u16[i].value, BLACK, WHITE, NORMAL_SIZE, 0);
	}

	for (uint8_t i = 1; i <= Value_True.num_16; i++)
	{
		LCD_ShowString(&LCD_bus_2, NAME_X, (1 + Value_True.num_16 + i) * NORMAL_SIZE, Value_True.Value_True_16[i].name, BLACK, WHITE, NORMAL_SIZE, 0);
		LCD_ShowFloatNum1(&LCD_bus_2, ID_X, (1 + Value_True.num_16 + i) * NORMAL_SIZE, Value_True.Value_True_16[i].id, BLACK, WHITE, NORMAL_SIZE, 0);
		LCD_ShowFloatNum1(&LCD_bus_2, VALUE_X, (1 + Value_True.num_16 + i) * NORMAL_SIZE, Value_True.Value_True_16[i].value, BLACK, WHITE, NORMAL_SIZE, 0);
	}

	for (uint8_t i = 1; i <= Value_True.num_32; i++)
	{
		LCD_ShowString(&LCD_bus_2, NAME_X, (1 + Value_True.num_16 + Value_True.num_u16 + i) * NORMAL_SIZE, Value_True.Value_True_32[i].name, BLACK, WHITE, NORMAL_SIZE, 0);
		LCD_ShowFloatNum1(&LCD_bus_2, ID_X, (1 + Value_True.num_16 + Value_True.num_u16 + i) * NORMAL_SIZE, Value_True.Value_True_32[i].id, BLACK, WHITE, NORMAL_SIZE, 0);
		LCD_ShowFloatNum1(&LCD_bus_2, VALUE_X, (1 + Value_True.num_16 + Value_True.num_u16 + i) * NORMAL_SIZE, Value_True.Value_True_32[i].value, BLACK, WHITE, NORMAL_SIZE, 0);
	}
}