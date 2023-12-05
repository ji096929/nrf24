#include "lcd_bsp.h" 
#include "lcd.h"
extern SPI_HandleTypeDef hspi1;

uint8_t SPI1_ReadWriteByte(uint8_t TxData);
extern LCD_ctx_t LCD_ctx;
uint8_t FontBuf[130];//字库缓存	
/******************************************************************************
      函数说明：向字库写入命令
      入口数据：dat  要写入的命令
      返回值：  无
******************************************************************************/
void ZK_command(LCD_line_t *LCD_bus,uint8_t dat)
{
	SPI1_ReadWriteByte(dat);
 }

/******************************************************************************
      函数说明：从字库读取数据
      入口数据：无
      返回值：  ret_data 读取的数据
******************************************************************************/
uint8_t get_data_from_ROM(void)
{
	return SPI1_ReadWriteByte(0xff);    //返回读出的一个字节
}
 
/******************************************************************************
      函数说明：读取N个数据
      入口数据：AddrHigh  写地址高字节
                AddrMid   写地址中字节
                AddrLow   写地址低字节
                *pBuff    读取的数据
                DataLen   读取数据的长度
      返回值：  无
******************************************************************************/
void get_n_bytes_data_from_ROM(LCD_line_t *LCD_bus,uint8_t AddrHigh,uint8_t AddrMid,uint8_t AddrLow,uint8_t *pBuff,uint8_t DataLen)
{
 	uint8_t i;
	ZK_CS_Clr(); //字库片选
	ZK_command(LCD_bus,0x03);//写指令
	ZK_command(LCD_bus,AddrHigh);//写地址高字节
	ZK_command(LCD_bus,AddrMid);//写地址中字节
	ZK_command(LCD_bus,AddrLow);//写地址低字节
	for(i=0;i<DataLen;i++)
	{
	   *(pBuff+i) =get_data_from_ROM();//读一个字节数据
	}

	ZK_CS_Set();//取消字库片选
}

/******************************************************************************
      函数说明：显示汉字
      入口数据：x,y      写入的坐标
                zk_num   1:12*12,  2:15*16,  3:24*24,  4:32*32
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_GB2312(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint16_t fc,uint16_t bc)
{
	LCD_ctx.handle=LCD_bus->handle;
  uint8_t i,k,n,d,m=0;
	switch(zk_num)
	{
		// n:字符所占字节数  d：字间距
		case 12 :  n=24;  d=12; break;   // 12*12
		case 16 :  n=32;  d=16; break;   // 15*16
		case 24 :  n=72;  d=24; break;   // 24*24
		case 32 :  n=128; d=32; break;   // 32*32
	}
	LCD_Address_Set(&LCD_ctx,LCD_bus,x,y,x+d-1,y+d-1);
	for(i=0;i<n;i++)
	{
		for(k=0;k<8;k++)
		{
			if((FontBuf[i]&(0x80>>k)))
			{
			  LCD_WR_DATA(&LCD_ctx,LCD_bus,fc);
			}
			else
			{
			  LCD_WR_DATA(&LCD_ctx,LCD_bus,bc);
			}
			m++;
			if(m%zk_num==0)
			{
				m=0;
				break;
			}
		}
	}
}

 
/******************************************************************************
      函数说明：显示汉字
      入口数据：zk_num    1:12*12,  2:15*16,  3:24*24,  4:32*32
                x,y       坐标
                text[]    要显示的汉字
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_GB2312_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc)
{
	LCD_ctx.handle=LCD_bus->handle;
	uint8_t i= 0;
	uint8_t AddrHigh,AddrMid,AddrLow ; //字高、中、低地址
	uint32_t FontAddr=0; //字地址
	uint32_t BaseAdd=0; //字库基地址	
	uint8_t n,d;// 不同点阵字库的计算变量
	switch(zk_num)
	{
		// n:字符所占字节数  d：字间距
		case 12 :  BaseAdd=0x00;    n=24;  d=12; break;   // 12*12  
		case 16 :  BaseAdd=0x2C9D0; n=32;  d=16; break;   // 15*16  
		case 24 :  BaseAdd=0x68190; n=72;  d=24; break;   // 24*24  
		case 32 :  BaseAdd=0xEDF00; n=128; d=32; break;   // 32*32  
	}
	while((text[i]>0x00))
	{
		if(((text[i]>=0xA1)&&(text[i]<=0xA9))&&(text[i+1]>=0xA1))
		{						
			//国标简体（GB2312）汉字在 字库IC中的地址由以下公式来计算：//
			//Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; 分三部取地址///
			FontAddr = (text[i]- 0xA1)*94; 
			FontAddr += (text[i+1]-0xA1);
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  //地址的高8位,共24位//
			AddrMid = (FontAddr&0xff00)>>8;      //地址的中8位,共24位//
			AddrLow = FontAddr&0xff;	     //地址的低8位,共24位//
			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );//取一个汉字的数据，存到"FontBuf[]"
			Display_GB2312(LCD_bus,x,y,zk_num,fc,bc);//显示一个汉字到LCD上/ 
		}
		else if(((text[i]>=0xB0) &&(text[i]<=0xF7))&&(text[i+1]>=0xA1))
		{
			//国标简体（GB2312） 字库IC中的地址由以下公式来计算：//
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; 分三部取地址//
			FontAddr = (text[i]- 0xB0)*94; 
			FontAddr += (text[i+1]-0xA1)+846;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  //地址的高8位,共24位//
			AddrMid = (FontAddr&0xff00)>>8;      //地址的中8位,共24位//
			AddrLow = FontAddr&0xff;	     //地址的低8位,共24位//
			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );//取一个汉字的数据，存到"FontBuf[ ]"
			Display_GB2312(LCD_bus,x,y,zk_num,fc,bc);//显示一个汉字到LCD上/
		}
		x+=d; //下一个字坐标
		i+=2;  //下个字符
	}
} 
 
/******************************************************************************
      函数说明：显示ASCII码
      入口数据：x,y      写入的坐标
                zk_num   1:5*7   2:7*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_Asc(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint16_t fc,uint16_t bc)
{
	LCD_ctx.handle=LCD_bus->handle;
  uint8_t i,k,n,x0,y0,m=0;
	switch(zk_num)
	{
		// n:字符所占字节数  d：字间距
		case 7:   n=7;  x0=5;  y0=7;  break;	 //	  5x7 ASCII
		case 8:   n=8;  x0=7;  y0=8;  break;	 //   7x8 ASCII
		case 12:  n=12; x0=6;  y0=12; break;	 //  6x12 ASCII
	  case 16:  n=16; x0=8;  y0=16; break;	 //  8x16 ASCII	
	  case 24:  n=48; x0=12; y0=24; break;	 //  12x24 ASCII
	 	case 32:  n=64; x0=16; y0=32; break;	 //  16x32 ASCII
	}
	LCD_Address_Set(&LCD_ctx,LCD_bus,x,y,x+x0-1,y+y0-1);
	for(i=0;i<n;i++)
	{
		for(k=0;k<8;k++)
		{
			if((FontBuf[i]&(0x80>>k)))
			{
			  LCD_WR_DATA(&LCD_ctx,LCD_bus,fc);
			}
			else
			{
			  LCD_WR_DATA(&LCD_ctx,LCD_bus,bc);
			}
			m++;
			if(m%x0==0)
			{
				m=0;
				break;
			}
		}
	}
}

/******************************************************************************
      函数说明：显示ASCII码
      入口数据：x,y      写入的坐标
                zk_num   1:5*7   2:7*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                text[]   要显示的字符串
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_Asc_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc)
{
	LCD_ctx.handle=LCD_bus->handle;
	uint8_t i= 0;
	uint8_t AddrHigh,AddrMid,AddrLow ; //字高、中、低地址
	uint32_t FontAddr=0; //字地址
	uint32_t BaseAdd=0; //字库基地址	
  uint8_t n,d;// 不同点阵字库的计算变量
	switch(zk_num)
	{
		//n个数，d:字间距
		case 7:   BaseAdd=0x1DDF80; n=8;  d=6;  break;	 //	  5x7 ASCII
		case 8:   BaseAdd=0x1DE280; n=8;  d=7;  break;	 //   7x8 ASCII
		case 12:  BaseAdd=0x1DBE00; n=12; d=6;  break;	 //  6x12 ASCII
	  case 16:  BaseAdd=0x1DD780; n=16; d=8;  break;	 //  8x16 ASCII	
	  case 24:  BaseAdd=0x1DFF00; n=48; d=12; break;	 //  12x24 ASCII
	 	case 32:  BaseAdd=0x1E5A50; n=64; d=16; break;	 //  16x32 ASCII
	}
	while((text[i]>0x00))
	{	
	  if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		  FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );/*取一个汉字的数据，存到"FontBuf[]"*/
			Display_Asc(LCD_bus,x,y,zk_num,fc,bc);/*显示一个ascii到LCD上 */
		}
    i++;  //下个数据
		x+=d;//下一个字坐标 
	}
}


/******************************************************************************
      函数说明：显示ASCII码(Arial&Times New Roman)
      入口数据：x,y      写入的坐标
                zk_num   1:8*12,  2:12*16,  3:16*24,  4:24*32
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_Arial_TimesNewRoman(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint16_t fc,uint16_t bc)
{
	LCD_ctx.handle=LCD_bus->handle;
  uint8_t i,k,n,x0,y0,m=0;
	switch(zk_num)
	{
		// n:字符所占字节数  d:字间距
		case 12:  n=26;  x0=10; y0=12; break;	 //  8x12 ASCII
	  case 16:  n=34;  x0=12; y0=16; break;	 //  12x16 ASCII
	  case 24:  n=74;  x0=20; y0=24; break;	 //  16x24 ASCII
	 	case 32:  n=130; x0=25; y0=32; break;	 //  24x32 ASCII
	}
	LCD_Address_Set(&LCD_ctx,LCD_bus,x,y,x+x0-1,y+y0-1);
	for(i=2;i<n;i++)
	{
		for(k=0;k<8;k++)
		{
			if((FontBuf[i]&(0x80>>k)))
			{
			  LCD_WR_DATA(&LCD_ctx,LCD_bus,fc);
			}
			else
			{
			  LCD_WR_DATA(&LCD_ctx,LCD_bus,bc);
			}
			m++;
			if(m%x0==0)
			{
				m=0;
				break;
			}
		}
	}
}



/******************************************************************************
      函数说明：显示ASCII(Arial类型)
      入口数据：x,y      写入的坐标
                zk_num   1:8*12,  2:12*16,  3:16*24,  4:24*32
                text[]   要显示的字符串
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_Arial_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc)
{
	LCD_ctx.handle=LCD_bus->handle;
	uint8_t i= 0;
	uint8_t AddrHigh,AddrMid,AddrLow ; //字高、中、低地址
	uint32_t FontAddr=0; //字地址
	uint32_t BaseAdd=0; //字库基地址	
  uint8_t n,d;// 不同点阵字库的计算变量
	switch(zk_num)
	{
		//n:个数，d:字间距
		case 12:  BaseAdd=0x1DC400; n=26;  d=10; break;	 //  8x12 ASCII(Arial类型)
	  case 16:  BaseAdd=0x1DE580; n=34;  d=12; break;	 //  12x16 ASCII(Arial类型)
	  case 24:  BaseAdd=0x1E22D0; n=74;  d=20; break;	 //  16x24 ASCII(Arial类型)
	 	case 32:  BaseAdd=0x1E99D0; n=130; d=25; break;	 //  24x32 ASCII(Arial类型)
	}
	while((text[i]>0x00))
	{	
	  if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		  FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );/*取一个汉字的数据，存到"FontBuf[]"*/
			Display_Arial_TimesNewRoman(LCD_bus,x,y,zk_num,fc,bc);/*显示一个ascii到LCD上 */
		}
    i++;  //下个数据
		x+=d;//下一个字坐标 
	}
}


/******************************************************************************
      函数说明：显示ASCII(Arial类型)
      入口数据：x,y      写入的坐标
                zk_num   1:8*12,  2:12*16,  3:16*24,  4:24*32
                text[]   要显示的字符串
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_TimesNewRoman_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc)
{
	LCD_ctx.handle=LCD_bus->handle;
	uint8_t i= 0;
	uint8_t AddrHigh,AddrMid,AddrLow ; //字高、中、低地址
	uint32_t FontAddr=0; //字地址
	uint32_t BaseAdd=0; //字库基地址	
  uint8_t n,d;// 不同点阵字库的计算变量
	switch(zk_num)
	{
		//n:个数，d:字间距
		case 12:  BaseAdd=0x1DCDC0; n=26;  d=10; break;	 //  8x12 ASCII(TimesNewRoman类型)
	  case 16:  BaseAdd=0x1DF240; n=34;  d=12; break;	 //  12x16 ASCII(TimesNewRoman类型)
	  case 24:  BaseAdd=0x1E3E90; n=74;  d=20; break;	 //  16x24 ASCII(TimesNewRoman类型)
	 	case 32:  BaseAdd=0x1ECA90; n=130; d=25; break;	 //  24x32 ASCII(TimesNewRoman类型)
	}
	while((text[i]>0x00))
	{	
	  if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		  FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );/*取一个汉字的数据，存到"FontBuf[]"*/
			Display_Arial_TimesNewRoman(LCD_bus,x,y,zk_num,fc,bc);/*显示一个ascii到LCD上 */
		}
    i++;  //下个数据
		x+=d;//下一个字坐标 
	}
}
uint16_t SPI_I2S_ReceiveData(SPI_HandleTypeDef *hspi)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(hspi));
  
  /* Return the data in the DR register */
  return hspi->Instance->DR;
}
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;		
	uint8_t rxdata;	
	while (HAL_SPI_GetState(&hspi1) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}		
	HAL_SPI_Transmit(&hspi1,&TxData,sizeof(TxData),0x1111);//通过外设SPIx发送一个数据
	retry=0;

	while (HAL_SPI_GetState(&hspi1) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(&hspi1);//HAL_SPI_Receive(&hspi1,&rxdata,sizeof(rxdata),0x1111); //返回通过SPIx最近接收的数据					    
}
