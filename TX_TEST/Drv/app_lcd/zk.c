//#include "lcd_init.h" 
//#include "lcd.h"
//extern SPI_HandleTypeDef hspi1;

//uint8_t SPI1_ReadWriteByte(uint8_t TxData);
//extern LCD_ctx_t LCD_ctx;
//uint8_t FontBuf[130];//�ֿ⻺��	
///******************************************************************************
//      ����˵�������ֿ�д������
//      ������ݣ�dat  Ҫд�������
//      ����ֵ��  ��
//******************************************************************************/
//void ZK_command(LCD_line_t *LCD_bus,uint8_t dat)
//{
//	SPI1_ReadWriteByte(dat);
// }

///******************************************************************************
//      ����˵�������ֿ��ȡ����
//      ������ݣ���
//      ����ֵ��  ret_data ��ȡ������
//******************************************************************************/
//uint8_t get_data_from_ROM(void)
//{
//	return SPI1_ReadWriteByte(0xff);    //���ض�����һ���ֽ�
//}
// 
///******************************************************************************
//      ����˵������ȡN������
//      ������ݣ�AddrHigh  д��ַ���ֽ�
//                AddrMid   д��ַ���ֽ�
//                AddrLow   д��ַ���ֽ�
//                *pBuff    ��ȡ������
//                DataLen   ��ȡ���ݵĳ���
//      ����ֵ��  ��
//******************************************************************************/
//void get_n_bytes_data_from_ROM(LCD_line_t *LCD_bus,uint8_t AddrHigh,uint8_t AddrMid,uint8_t AddrLow,uint8_t *pBuff,uint8_t DataLen)
//{
// 	uint8_t i;
//	ZK_CS_Clr(); //�ֿ�Ƭѡ
//	ZK_command(LCD_bus,0x03);//дָ��
//	ZK_command(LCD_bus,AddrHigh);//д��ַ���ֽ�
//	ZK_command(LCD_bus,AddrMid);//д��ַ���ֽ�
//	ZK_command(LCD_bus,AddrLow);//д��ַ���ֽ�
//	for(i=0;i<DataLen;i++)
//	{
//	   *(pBuff+i) =get_data_from_ROM();//��һ���ֽ�����
//	}

//	ZK_CS_Set();//ȡ���ֿ�Ƭѡ
//}

///******************************************************************************
//      ����˵������ʾ����
//      ������ݣ�x,y      д�������
//                zk_num   1:12*12,  2:15*16,  3:24*24,  4:32*32
//                fc ������ɫ
//                bc ������ɫ
//      ����ֵ��  ��
//******************************************************************************/
//void Display_GB2312(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint16_t fc,uint16_t bc)
//{
//	LCD_ctx.handle=LCD_bus->handle;
//  uint8_t i,k,n,d,m=0;
//	switch(zk_num)
//	{
//		// n:�ַ���ռ�ֽ���  d���ּ��
//		case 12 :  n=24;  d=12; break;   // 12*12
//		case 16 :  n=32;  d=16; break;   // 15*16
//		case 24 :  n=72;  d=24; break;   // 24*24
//		case 32 :  n=128; d=32; break;   // 32*32
//	}
//	LCD_Address_Set(&LCD_ctx,LCD_bus,x,y,x+d-1,y+d-1);
//	for(i=0;i<n;i++)
//	{
//		for(k=0;k<8;k++)
//		{
//			if((FontBuf[i]&(0x80>>k)))
//			{
//			  LCD_WR_DATA(&LCD_ctx,LCD_bus,fc);
//			}
//			else
//			{
//			  LCD_WR_DATA(&LCD_ctx,LCD_bus,bc);
//			}
//			m++;
//			if(m%zk_num==0)
//			{
//				m=0;
//				break;
//			}
//		}
//	}
//}

// 
///******************************************************************************
//      ����˵������ʾ����
//      ������ݣ�zk_num    1:12*12,  2:15*16,  3:24*24,  4:32*32
//                x,y       ����
//                text[]    Ҫ��ʾ�ĺ���
//                fc ������ɫ
//                bc ������ɫ
//      ����ֵ��  ��
//******************************************************************************/
//void Display_GB2312_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc)
//{
//	LCD_ctx.handle=LCD_bus->handle;
//	uint8_t i= 0;
//	uint8_t AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
//	uint32_t FontAddr=0; //�ֵ�ַ
//	uint32_t BaseAdd=0; //�ֿ����ַ	
//	uint8_t n,d;// ��ͬ�����ֿ�ļ������
//	switch(zk_num)
//	{
//		// n:�ַ���ռ�ֽ���  d���ּ��
//		case 12 :  BaseAdd=0x00;    n=24;  d=12; break;   // 12*12  
//		case 16 :  BaseAdd=0x2C9D0; n=32;  d=16; break;   // 15*16  
//		case 24 :  BaseAdd=0x68190; n=72;  d=24; break;   // 24*24  
//		case 32 :  BaseAdd=0xEDF00; n=128; d=32; break;   // 32*32  
//	}
//	while((text[i]>0x00))
//	{
//		if(((text[i]>=0xA1)&&(text[i]<=0xA9))&&(text[i+1]>=0xA1))
//		{						
//			//������壨GB2312�������� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺//
//			//Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; ������ȡ��ַ///
//			FontAddr = (text[i]- 0xA1)*94; 
//			FontAddr += (text[i+1]-0xA1);
//			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
//			
//			AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ//
//			AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ//
//			AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ//
//			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[]"
//			Display_GB2312(LCD_bus,x,y,zk_num,fc,bc);//��ʾһ�����ֵ�LCD��/ 
//		}
//		else if(((text[i]>=0xB0) &&(text[i]<=0xF7))&&(text[i+1]>=0xA1))
//		{
//			//������壨GB2312�� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺//
//			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; ������ȡ��ַ//
//			FontAddr = (text[i]- 0xB0)*94; 
//			FontAddr += (text[i+1]-0xA1)+846;
//			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
//			
//			AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ//
//			AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ//
//			AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ//
//			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[ ]"
//			Display_GB2312(LCD_bus,x,y,zk_num,fc,bc);//��ʾһ�����ֵ�LCD��/
//		}
//		x+=d; //��һ��������
//		i+=2;  //�¸��ַ�
//	}
//} 
// 
///******************************************************************************
//      ����˵������ʾASCII��
//      ������ݣ�x,y      д�������
//                zk_num   1:5*7   2:7*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
//                fc ������ɫ
//                bc ������ɫ
//      ����ֵ��  ��
//******************************************************************************/
//void Display_Asc(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint16_t fc,uint16_t bc)
//{
//	LCD_ctx.handle=LCD_bus->handle;
//  uint8_t i,k,n,x0,y0,m=0;
//	switch(zk_num)
//	{
//		// n:�ַ���ռ�ֽ���  d���ּ��
//		case 7:   n=7;  x0=5;  y0=7;  break;	 //	  5x7 ASCII
//		case 8:   n=8;  x0=7;  y0=8;  break;	 //   7x8 ASCII
//		case 12:  n=12; x0=6;  y0=12; break;	 //  6x12 ASCII
//	  case 16:  n=16; x0=8;  y0=16; break;	 //  8x16 ASCII	
//	  case 24:  n=48; x0=12; y0=24; break;	 //  12x24 ASCII
//	 	case 32:  n=64; x0=16; y0=32; break;	 //  16x32 ASCII
//	}
//	LCD_Address_Set(&LCD_ctx,LCD_bus,x,y,x+x0-1,y+y0-1);
//	for(i=0;i<n;i++)
//	{
//		for(k=0;k<8;k++)
//		{
//			if((FontBuf[i]&(0x80>>k)))
//			{
//			  LCD_WR_DATA(&LCD_ctx,LCD_bus,fc);
//			}
//			else
//			{
//			  LCD_WR_DATA(&LCD_ctx,LCD_bus,bc);
//			}
//			m++;
//			if(m%x0==0)
//			{
//				m=0;
//				break;
//			}
//		}
//	}
//}

///******************************************************************************
//      ����˵������ʾASCII��
//      ������ݣ�x,y      д�������
//                zk_num   1:5*7   2:7*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
//                text[]   Ҫ��ʾ���ַ���
//                fc ������ɫ
//                bc ������ɫ
//      ����ֵ��  ��
//******************************************************************************/
//void Display_Asc_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc)
//{
//	LCD_ctx.handle=LCD_bus->handle;
//	uint8_t i= 0;
//	uint8_t AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
//	uint32_t FontAddr=0; //�ֵ�ַ
//	uint32_t BaseAdd=0; //�ֿ����ַ	
//  uint8_t n,d;// ��ͬ�����ֿ�ļ������
//	switch(zk_num)
//	{
//		//n������d:�ּ��
//		case 7:   BaseAdd=0x1DDF80; n=8;  d=6;  break;	 //	  5x7 ASCII
//		case 8:   BaseAdd=0x1DE280; n=8;  d=7;  break;	 //   7x8 ASCII
//		case 12:  BaseAdd=0x1DBE00; n=12; d=6;  break;	 //  6x12 ASCII
//	  case 16:  BaseAdd=0x1DD780; n=16; d=8;  break;	 //  8x16 ASCII	
//	  case 24:  BaseAdd=0x1DFF00; n=48; d=12; break;	 //  12x24 ASCII
//	 	case 32:  BaseAdd=0x1E5A50; n=64; d=16; break;	 //  16x32 ASCII
//	}
//	while((text[i]>0x00))
//	{	
//	  if((text[i] >= 0x20) &&(text[i] <= 0x7E))
//		{						
//		  FontAddr = 	text[i]-0x20;
//			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
//			
//			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
//			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
//			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
//			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
//			Display_Asc(LCD_bus,x,y,zk_num,fc,bc);/*��ʾһ��ascii��LCD�� */
//		}
//    i++;  //�¸�����
//		x+=d;//��һ�������� 
//	}
//}


///******************************************************************************
//      ����˵������ʾASCII��(Arial&Times New Roman)
//      ������ݣ�x,y      д�������
//                zk_num   1:8*12,  2:12*16,  3:16*24,  4:24*32
//                fc ������ɫ
//                bc ������ɫ
//      ����ֵ��  ��
//******************************************************************************/
//void Display_Arial_TimesNewRoman(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint8_t zk_num,uint16_t fc,uint16_t bc)
//{
//	LCD_ctx.handle=LCD_bus->handle;
//  uint8_t i,k,n,x0,y0,m=0;
//	switch(zk_num)
//	{
//		// n:�ַ���ռ�ֽ���  d:�ּ��
//		case 12:  n=26;  x0=10; y0=12; break;	 //  8x12 ASCII
//	  case 16:  n=34;  x0=12; y0=16; break;	 //  12x16 ASCII
//	  case 24:  n=74;  x0=20; y0=24; break;	 //  16x24 ASCII
//	 	case 32:  n=130; x0=25; y0=32; break;	 //  24x32 ASCII
//	}
//	LCD_Address_Set(&LCD_ctx,LCD_bus,x,y,x+x0-1,y+y0-1);
//	for(i=2;i<n;i++)
//	{
//		for(k=0;k<8;k++)
//		{
//			if((FontBuf[i]&(0x80>>k)))
//			{
//			  LCD_WR_DATA(&LCD_ctx,LCD_bus,fc);
//			}
//			else
//			{
//			  LCD_WR_DATA(&LCD_ctx,LCD_bus,bc);
//			}
//			m++;
//			if(m%x0==0)
//			{
//				m=0;
//				break;
//			}
//		}
//	}
//}



///******************************************************************************
//      ����˵������ʾASCII(Arial����)
//      ������ݣ�x,y      д�������
//                zk_num   1:8*12,  2:12*16,  3:16*24,  4:24*32
//                text[]   Ҫ��ʾ���ַ���
//                fc ������ɫ
//                bc ������ɫ
//      ����ֵ��  ��
//******************************************************************************/
//void Display_Arial_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc)
//{
//	LCD_ctx.handle=LCD_bus->handle;
//	uint8_t i= 0;
//	uint8_t AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
//	uint32_t FontAddr=0; //�ֵ�ַ
//	uint32_t BaseAdd=0; //�ֿ����ַ	
//  uint8_t n,d;// ��ͬ�����ֿ�ļ������
//	switch(zk_num)
//	{
//		//n:������d:�ּ��
//		case 12:  BaseAdd=0x1DC400; n=26;  d=10; break;	 //  8x12 ASCII(Arial����)
//	  case 16:  BaseAdd=0x1DE580; n=34;  d=12; break;	 //  12x16 ASCII(Arial����)
//	  case 24:  BaseAdd=0x1E22D0; n=74;  d=20; break;	 //  16x24 ASCII(Arial����)
//	 	case 32:  BaseAdd=0x1E99D0; n=130; d=25; break;	 //  24x32 ASCII(Arial����)
//	}
//	while((text[i]>0x00))
//	{	
//	  if((text[i] >= 0x20) &&(text[i] <= 0x7E))
//		{						
//		  FontAddr = 	text[i]-0x20;
//			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
//			
//			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
//			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
//			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
//			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
//			Display_Arial_TimesNewRoman(LCD_bus,x,y,zk_num,fc,bc);/*��ʾһ��ascii��LCD�� */
//		}
//    i++;  //�¸�����
//		x+=d;//��һ�������� 
//	}
//}


///******************************************************************************
//      ����˵������ʾASCII(Arial����)
//      ������ݣ�x,y      д�������
//                zk_num   1:8*12,  2:12*16,  3:16*24,  4:24*32
//                text[]   Ҫ��ʾ���ַ���
//                fc ������ɫ
//                bc ������ɫ
//      ����ֵ��  ��
//******************************************************************************/
//void Display_TimesNewRoman_String(LCD_line_t *LCD_bus,uint16_t x,uint16_t y,uint16_t zk_num,uint8_t text[],uint16_t fc,uint16_t bc)
//{
//	LCD_ctx.handle=LCD_bus->handle;
//	uint8_t i= 0;
//	uint8_t AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
//	uint32_t FontAddr=0; //�ֵ�ַ
//	uint32_t BaseAdd=0; //�ֿ����ַ	
//  uint8_t n,d;// ��ͬ�����ֿ�ļ������
//	switch(zk_num)
//	{
//		//n:������d:�ּ��
//		case 12:  BaseAdd=0x1DCDC0; n=26;  d=10; break;	 //  8x12 ASCII(TimesNewRoman����)
//	  case 16:  BaseAdd=0x1DF240; n=34;  d=12; break;	 //  12x16 ASCII(TimesNewRoman����)
//	  case 24:  BaseAdd=0x1E3E90; n=74;  d=20; break;	 //  16x24 ASCII(TimesNewRoman����)
//	 	case 32:  BaseAdd=0x1ECA90; n=130; d=25; break;	 //  24x32 ASCII(TimesNewRoman����)
//	}
//	while((text[i]>0x00))
//	{	
//	  if((text[i] >= 0x20) &&(text[i] <= 0x7E))
//		{						
//		  FontAddr = 	text[i]-0x20;
//			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
//			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
//			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
//			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
//			get_n_bytes_data_from_ROM(LCD_bus,AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
//			Display_Arial_TimesNewRoman(LCD_bus,x,y,zk_num,fc,bc);/*��ʾһ��ascii��LCD�� */
//		}
//    i++;  //�¸�����
//		x+=d;//��һ�������� 
//	}
//}
//uint16_t SPI_I2S_ReceiveData(SPI_HandleTypeDef *hspi)
//{
//  /* Check the parameters */
//  assert_param(IS_SPI_ALL_PERIPH(hspi));
//  
//  /* Return the data in the DR register */
//  return hspi->Instance->DR;
//}
////SPIx ��дһ���ֽ�
////TxData:Ҫд����ֽ�
////����ֵ:��ȡ�����ֽ�
//uint8_t SPI1_ReadWriteByte(uint8_t TxData)
//{		
//	uint8_t retry=0;		
//	uint8_t rxdata;	
//	while (HAL_SPI_GetState(&hspi1) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
//		{
//		retry++;
//		if(retry>200)return 0;
//		}		
//	HAL_SPI_Transmit(&hspi1,&TxData,sizeof(TxData),0x1111);//ͨ������SPIx����һ������
//	retry=0;

//	while (HAL_SPI_GetState(&hspi1) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
//		{
//		retry++;
//		if(retry>200)return 0;
//		}	  						    
//	return SPI_I2S_ReceiveData(&hspi1);//HAL_SPI_Receive(&hspi1,&rxdata,sizeof(rxdata),0x1111); //����ͨ��SPIx������յ�����					    
//}
