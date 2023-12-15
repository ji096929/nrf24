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
	// 这里先注释,如果使用了SPI2，则开启
	LCD_bus_2.handle = &LCD_BUS_2;
#endif
}

void LCD_Init(LCD_line_t *LCD_bus)
{
	// MX_GPIO_Init();//初始化GPIO
	TFT_LCD_Init();
	LCD_ctx.handle = &LCD_bus->handle;
	LCD_spi_RES_Clr(); // 复位
	HAL_Delay(100);
	LCD_spi_RES_Set();
	HAL_Delay(100);

	LCD_spi_BLK_Set(); // 打开背光
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
	  函数说明：在指定区域填充颜色
	  入口数据：xsta,ysta   起始坐标
				xend,yend   终止坐标
								color       要填充的颜色
	  返回值：  无
******************************************************************************/
void LCD_Fill(LCD_line_t *LCD_bus, uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint16_t i, j;
	LCD_Address_Set(&LCD_ctx, LCD_bus, xsta, ysta, xend - 1, yend - 1); // 设置显示范围
	for (i = ysta; i < yend; i++)
	{
		for (j = xsta; j < xend; j++)
		{
			LCD_WR_DATA(&LCD_ctx, LCD_bus, color);
		}
	}
}

/******************************************************************************
	  函数说明：在指定位置画点
	  入口数据：x,y 画点坐标
				color 点的颜色
	  返回值：  无
******************************************************************************/
void LCD_DrawPoint(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint16_t color)
{
	LCD_ctx.handle = LCD_bus->handle;
	LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x, y); // 设置光标位置
	LCD_WR_DATA(&LCD_ctx, LCD_bus, color);
}

/******************************************************************************
	  函数说明：画线
	  入口数据：x1,y1   起始坐标
				x2,y2   终止坐标
				color   线的颜色
	  返回值：  无
******************************************************************************/
void LCD_DrawLine(LCD_line_t *LCD_bus, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; // 计算坐标增量
	delta_y = y2 - y1;
	uRow = x1; // 画线起点坐标
	uCol = y1;
	if (delta_x > 0)
		incx = 1; // 设置单步方向
	else if (delta_x == 0)
		incx = 0; // 垂直线
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; // 水平线
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; // 选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		LCD_DrawPoint(LCD_bus, uRow, uCol, color); // 画点
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
	  函数说明：画矩形
	  入口数据：x1,y1   起始坐标
				x2,y2   终止坐标
				color   矩形的颜色
	  返回值：  无
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
	  函数说明：画圆
	  入口数据：x0,y0   圆心坐标
				r       半径
				color   圆的颜色
	  返回值：  无
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
		if ((a * a + b * b) > (r * r)) // 判断要画的点是否过远
		{
			b--;
		}
	}
}

/******************************************************************************
	  函数说明：显示汉字串
	  入口数据：x,y显示坐标
				*s 要显示的汉字串
				fc 字的颜色
				bc 字的背景色
				sizey 字号 可选 16 24 32
				mode:  0非叠加模式  1叠加模式
	  返回值：  无
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
	  函数说明：显示单个12x12汉字
	  入口数据：x,y显示坐标
				*s 要显示的汉字
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode:  0非叠加模式  1叠加模式
	  返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t i, j, m = 0;
	uint16_t k;
	uint16_t HZnum;		  // 汉字数目
	uint16_t TypefaceNum; // 一个字符所占字节大小
	uint16_t x0 = x;
	TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

	HZnum = sizeof(tfont12) / sizeof(typFNT_GB12); // 统计汉字数目
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1)))
		{
			LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizey - 1, y + sizey - 1);
			for (i = 0; i < TypefaceNum; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) // 非叠加方式
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
					else // 叠加方式
					{
						if (tfont12[k].Msk[i] & (0x01 << j))
							LCD_DrawPoint(LCD_bus, x, y, fc); // 画一个点
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
		continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

/******************************************************************************
	  函数说明：显示单个16x16汉字
	  入口数据：x,y显示坐标
				*s 要显示的汉字
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode:  0非叠加模式  1叠加模式
	  返回值：  无
******************************************************************************/
void LCD_ShowChinese16x16(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t i, j, m = 0;
	uint16_t k;
	uint16_t HZnum;		  // 汉字数目
	uint16_t TypefaceNum; // 一个字符所占字节大小
	uint16_t x0 = x;
	TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
	HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); // 统计汉字数目
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
		{
			LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizey - 1, y + sizey - 1);
			for (i = 0; i < TypefaceNum; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) // 非叠加方式
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
					else // 叠加方式
					{
						if (tfont16[k].Msk[i] & (0x01 << j))
							LCD_DrawPoint(LCD_bus, x, y, fc); // 画一个点
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
		continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

/******************************************************************************
	  函数说明：显示单个24x24汉字
	  入口数据：x,y显示坐标
				*s 要显示的汉字
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode:  0非叠加模式  1叠加模式
	  返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t i, j, m = 0;
	uint16_t k;
	uint16_t HZnum;		  // 汉字数目
	uint16_t TypefaceNum; // 一个字符所占字节大小
	uint16_t x0 = x;
	TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
	HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); // 统计汉字数目
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
		{
			LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizey - 1, y + sizey - 1);
			for (i = 0; i < TypefaceNum; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) // 非叠加方式
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
					else // 叠加方式
					{
						if (tfont24[k].Msk[i] & (0x01 << j))
							LCD_DrawPoint(LCD_bus, x, y, fc); // 画一个点
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
		continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

/******************************************************************************
	  函数说明：显示单个32x32汉字
	  入口数据：x,y显示坐标
				*s 要显示的汉字
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode:  0非叠加模式  1叠加模式
	  返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t i, j, m = 0;
	uint16_t k;
	uint16_t HZnum;		  // 汉字数目
	uint16_t TypefaceNum; // 一个字符所占字节大小
	uint16_t x0 = x;
	TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
	HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); // 统计汉字数目
	for (k = 0; k < HZnum; k++)
	{
		if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
		{
			LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizey - 1, y + sizey - 1);
			for (i = 0; i < TypefaceNum; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (!mode) // 非叠加方式
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
					else // 叠加方式
					{
						if (tfont32[k].Msk[i] & (0x01 << j))
							LCD_DrawPoint(LCD_bus, x, y, fc); // 画一个点
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
		continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

/******************************************************************************
	  函数说明：显示单个字符
	  入口数据：x,y显示坐标
				num 要显示的字符
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode:  0非叠加模式  1叠加模式
	  返回值：  无
******************************************************************************/
void LCD_ShowChar(LCD_line_t *LCD_bus, uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	LCD_ctx.handle = LCD_bus->handle;
	uint8_t temp, sizex, t, m = 0;
	uint16_t i, TypefaceNum; // 一个字符所占字节大小
	uint16_t x0 = x;
	sizex = sizey / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
	num = num - ' ';														// 得到偏移后的值
	LCD_Address_Set(&LCD_ctx, LCD_bus, x, y, x + sizex - 1, y + sizey - 1); // 设置光标位置
	for (i = 0; i < TypefaceNum; i++)
	{
		if (sizey == 12)
			temp = ascii_1206[num][i]; // 调用6x12字体
		else if (sizey == 16)
			temp = ascii_1608[num][i]; // 调用8x16字体
		else if (sizey == 24)
			temp = ascii_2412[num][i]; // 调用12x24字体
		else if (sizey == 32)
			temp = ascii_3216[num][i]; // 调用16x32字体
		else
			return;
		for (t = 0; t < 8; t++)
		{
			if (!mode) // 非叠加模式
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
			else // 叠加模式
			{
				if (temp & (0x01 << t))
					LCD_DrawPoint(LCD_bus, x, y, fc); // 画一个点
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
	  函数说明：显示字符串
	  入口数据：x,y显示坐标
				*p 要显示的字符串
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode:  0非叠加模式  1叠加模式
	  返回值：  无
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
	  函数说明：显示数字
	  入口数据：m底数，n指数
	  返回值：  无
******************************************************************************/
uint32_t mypow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}

/******************************************************************************
	  函数说明：显示整数变量
	  入口数据：x,y显示坐标
				num 要显示整数变量
				len 要显示的位数
				fc 字的颜色
				bc 字的背景色
				sizey 字号
	  返回值：  无
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
	  函数说明：显示两位小数变量
	  入口数据：x,y显示坐标
				num 要显示小数变量
				len 要显示的位数
				fc 字的颜色
				bc 字的背景色
				sizey 字号
	  返回值：  无
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
	  函数说明：显示图片
	  入口数据：x,y起点坐标
				length 图片长度
				width  图片宽度
				pic[]  图片数组
	  返回值：  无
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
	// 12字号的四个英文字母占了24个横坐标单位
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