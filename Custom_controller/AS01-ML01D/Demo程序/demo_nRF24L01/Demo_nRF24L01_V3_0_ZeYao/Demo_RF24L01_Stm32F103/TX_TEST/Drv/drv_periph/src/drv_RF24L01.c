/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   NRF24L01配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */
  
  
  
#include "drv_RF24L01.h"
#include "spi.h"
const char *g_ErrorString = "RF24L01 is not find !...";
NRF24L01_bus_t  NRF24L01_bus;
NRF24L01_rtx_t  NRF24L01_rtx;

/**
  * @brief :NRF24L01读寄存器
  * @param :
           @Addr:寄存器地址
  * @note  :地址在设备中有效
  * @retval:读取的数据
  */
uint8_t NRF24L01_Read_Reg(NRF24L01_rtx_t *NRF24L01_rtx, uint8_t RegAddr )
{
    uint8_t btmp;
	RF24L01_SET_CS_LOW( );		//片选
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,NRF_READ_REG | RegAddr );	//读命令 地址
    btmp = NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,0xFF );				//读数据
	 RF24L01_SET_CS_HIGH( );		//取消片选
    return btmp;
}

/**
  * @brief :NRF24L01读指定长度的数据
  * @param :
  *			@reg:地址
  *			@pBuf:数据存放地址
  *			@len:数据长度
  * @note  :数据长度不超过255，地址在设备中有效
  * @retval:读取状态
  */
void NRF24L01_Read_Buf(NRF24L01_rtx_t *NRF24L01_rtx,uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t btmp;
	
    RF24L01_SET_CS_LOW( );			//片选
	
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,NRF_READ_REG | RegAddr );	//读命令 地址
    for( btmp = 0; btmp < len; btmp ++ )
    {
        *( pBuf + btmp ) = NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,0xFF );	//读数据
    }
    RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :NRF24L01写寄存器
  * @param :无
  * @note  :地址在设备中有效
  * @retval:读写状态
  */
void NRF24L01_Write_Reg(NRF24L01_rtx_t *NRF24L01_rtx ,uint8_t RegAddr, uint8_t Value )
{
    RF24L01_SET_CS_LOW( );		//片选
	
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,NRF_WRITE_REG | RegAddr );	//写命令 地址
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,Value );			//写数据
	  
    RF24L01_SET_CS_HIGH( );		//取消片选
	  
}

/**
  * @brief :NRF24L01写指定长度的数据
  * @param :
  *			@reg:地址
  *			@pBuf:写入的数据地址
  *			@len:数据长度
  * @note  :数据长度不超过255，地址在设备中有效
  * @retval:写状态
  */
void NRF24L01_Write_Buf(NRF24L01_rtx_t *NRF24L01_rtx ,uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t i;
    RF24L01_SET_CS_LOW( );		//片选
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,NRF_WRITE_REG | RegAddr );	//写命令 地址
    for( i = 0; i < len; i ++ )
    {
        NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,*( pBuf + i ) );		//写数据
    }
		RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :清空TX缓冲区
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Flush_Tx_Fifo ( NRF24L01_rtx_t *NRF24L01_rtx )
{
	RF24L01_SET_CS_LOW( );		//片选
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,FLUSH_TX );	//清TX FIFO命令
	RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :清空RX缓冲区
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Flush_Rx_Fifo( NRF24L01_rtx_t *NRF24L01_rtx )
{
	    RF24L01_SET_CS_LOW( );		//片选
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,FLUSH_RX );	//清RX FIFO命令
	RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :重新使用上一包数据
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Reuse_Tx_Payload( NRF24L01_rtx_t *NRF24L01_rtx )
{
	  RF24L01_SET_CS_LOW( );		//片选
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,REUSE_TX_PL );		//重新使用上一包命令
	  RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :NRF24L01空操作
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Nop(NRF24L01_rtx_t *NRF24L01_rtx)
{
	  RF24L01_SET_CS_LOW( );		//取消片选
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,NOP );		//空操作命令
	  RF24L01_SET_CS_HIGH( );		//取消片选
}


/**
  * @brief :NRF24L01读状态寄存器
  * @param :无
  * @note  :无
  * @retval:RF24L01状态
  */
uint8_t NRF24L01_Read_Status_Register(NRF24L01_rtx_t *NRF24L01_rtx )
{
    uint8_t Status;
	 RF24L01_SET_CS_LOW( );		//片选
	 //HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
    Status = NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle, NRF_READ_REG + STATUS );	//读状态寄存器
	  RF24L01_SET_CS_HIGH( );		//取消片选
    return Status;
}

/**
  * @brief :NRF24L01清中断
  * @param :
           @IRQ_Source:中断源
  * @note  :无
  * @retval:清除后状态寄存器的值
  */
uint8_t NRF24L01_Clear_IRQ_Flag(NRF24L01_rtx_t *NRF24L01_rtx, uint8_t IRQ_Source )
{
    uint8_t btmp = 0;

    IRQ_Source &= ( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT );	//中断标志处理
    btmp = NRF24L01_Read_Status_Register( NRF24L01_rtx);			//读状态寄存器
		
	  RF24L01_SET_CS_LOW( );		//片选
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,NRF_WRITE_REG + STATUS );	//写状态寄存器命令
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,IRQ_Source | btmp );		//清相应中断标志
	  RF24L01_SET_CS_HIGH( );		//取消片选
	
    return ( NRF24L01_Read_Status_Register(NRF24L01_rtx));			//返回状态寄存器状态
}

/**
  * @brief :读RF24L01中断状态
  * @param :无
  * @note  :无
  * @retval:中断状态
  */
uint8_t RF24L01_Read_IRQ_Status( NRF24L01_rtx_t *NRF24L01_rtx )
{
    return ( NRF24L01_Read_Status_Register( NRF24L01_rtx) & (( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT )));	//返回中断状态
}
 
 /**
  * @brief :读FIFO中数据宽度
  * @param :无
  * @note  :无
  * @retval:数据宽度
  */
uint8_t NRF24L01_Read_Top_Fifo_Width( NRF24L01_rtx_t *NRF24L01_rtx  )
{
    uint8_t btmp;
	
    RF24L01_SET_CS_LOW( );		//片选
	
    NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle, R_RX_PL_WID );	//读FIFO中数据宽度命令
    btmp = NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle, 0xFF );	//读数据
	
    RF24L01_SET_CS_HIGH( );		//取消片选
	
    return btmp;
}

 /**
  * @brief :读接收到的数据
  * @param :无
  * @note  :无
  * @retval:
           @pRxBuf:数据存放地址首地址
  */
uint8_t NRF24L01_Read_Rx_Payload(NRF24L01_rtx_t *NRF24L01_rtx, NRF24L01_bus_t *NRF24L01_bus )
{
    uint8_t Width, PipeNum;
    PipeNum = ( NRF24L01_Read_Reg(NRF24L01_rtx, STATUS ) >> 1 ) & 0x07;	//读接收状态
    Width = NRF24L01_Read_Top_Fifo_Width( NRF24L01_rtx);		//读接收数据个数

    RF24L01_SET_CS_LOW( );		//片选
    NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,RD_RX_PLOAD );			//读有效数据命令
	
    for( PipeNum = 0; PipeNum < Width; PipeNum ++ )
    {
        *( NRF24L01_bus->RxPacket.Rxbuffer + PipeNum ) = NRF24L01_rtx->rtx_cmd( NRF24L01_rtx->handle,0xFF );		//读数据
    }
    RF24L01_SET_CS_HIGH( );		//取消片选
    NRF24L01_Flush_Rx_Fifo(NRF24L01_rtx );	//清空RX FIFO
	
    return Width;
}

 /**
  * @brief :发送数据（带应答）
  * @param :
  *			@pTxBuf:发送数据地址
  *			@len:长度
  * @note  :一次不超过32个字节
  * @retval:无
  */
void NRF24L01_Write_Tx_Payload_Ack(NRF24L01_rtx_t *NRF24L01_rtx,  NRF24L01_bus_t *NRF24L01_bus)
{
    uint8_t btmp;
	  uint8_t len=NRF24L01_bus->TxPacket.Txlength;
    uint8_t length = ( len > 32 ) ? 32 : len;		//数据长达大约32 则只发送32个

    NRF24L01_Flush_Tx_Fifo(NRF24L01_rtx);		//清TX FIFO
	
    RF24L01_SET_CS_LOW( );			//片选
    NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle, WR_TX_PLOAD );	//发送命令
	
    for( btmp = 0; btmp < length; btmp ++ )
    {
        NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle, *( NRF24L01_bus->TxPacket.Txbuffer + btmp ) );	//发送数据
    }
    RF24L01_SET_CS_HIGH( );			//取消片选
}

 /**
  * @brief :发送数据（不带应答）
  * @param :
  *			@pTxBuf:发送数据地址
  *			@len:长度
  * @note  :一次不超过32个字节
  * @retval:无
  */
void NRF24L01_Write_Tx_Payload_NoAck( NRF24L01_rtx_t *NRF24L01_rtx, NRF24L01_bus_t *NRF24L01_bus )
{
	  uint8_t btmp;
	  uint8_t len=NRF24L01_bus->TxPacket.Txlength;
    if( len > 32 || len == 0 )
    {
        return ;		//数据长度大于32 或者等于0 不执行
    }
	
    RF24L01_SET_CS_LOW( );	//片选
    NRF24L01_rtx->rtx_cmd(  NRF24L01_rtx->handle,WR_TX_PLOAD_NACK );	//发送命令
		for( btmp = 0; btmp < len; btmp ++ )
    {
        NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle, *( NRF24L01_bus->TxPacket.Txbuffer + btmp ) );	//写数据
    }
    RF24L01_SET_CS_HIGH( );		//取消片选
}

 /**
  * @brief :在接收模式下向TX FIFO写数据(带ACK)
  * @param :
  *			@pData:数据地址
  *			@len:长度
  * @note  :一次不超过32个字节
  * @retval:无
  */
void NRF24L01_Write_Tx_Payload_InAck(NRF24L01_rtx_t *NRF24L01_rtx, NRF24L01_bus_t *NRF24L01_bus )
{
    uint8_t btmp;
	  uint8_t len=NRF24L01_bus->TxPacket.Txlength;
	
  	len = ( len > 32 ) ? 32 : len;		//数据长度大于32个则只写32个字节

    RF24L01_SET_CS_LOW( );			//片选
    NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle , W_ACK_PLOAD );		//命令
    for( btmp = 0; btmp < len; btmp ++ )
    {
        NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle, *( NRF24L01_bus->TxPacket.Txbuffer + btmp ) );	//写数据
    }
    RF24L01_SET_CS_HIGH( );			//取消片选
}

 /**
  * @brief :设置发送地址
  * @param :
  *			@pAddr:地址存放地址
  *			@len:长度
  * @note  :无
  * @retval:无
  */
void NRF24L01_Set_TxAddr(NRF24L01_rtx_t *NRF24L01_rtx, uint8_t *pAddr, uint8_t len )
{
	len = ( len > 5 ) ? 5 : len;					//地址不能大于5个字节
    NRF24L01_Write_Buf(NRF24L01_rtx, TX_ADDR, pAddr, len );	//写地址
}

 /**
  * @brief :设置接收通道地址
  * @param :
  *			@PipeNum:通道
  *			@pAddr:地址存肥着地址
  *			@Len:长度
  * @note  :通道不大于5 地址长度不大于5个字节
  * @retval:无
  */
void NRF24L01_Set_RxAddr(NRF24L01_rtx_t *NRF24L01_rtx, uint8_t PipeNum, uint8_t *pAddr, uint8_t Len )
{
    Len = ( Len > 5 ) ? 5 : Len;
    PipeNum = ( PipeNum > 5 ) ? 5 : PipeNum;		//通道不大于5 地址长度不大于5个字节

    NRF24L01_Write_Buf(NRF24L01_rtx, RX_ADDR_P0 + PipeNum, pAddr, Len );	//写入地址
}

 /**
  * @brief :设置通信速度
  * @param :
  *			@Speed:速度
  * @note  :无
  * @retval:无
  */
void NRF24L01_Set_Speed(NRF24L01_rtx_t *NRF24L01_rtx, nRf24l01SpeedType Speed )
{
	uint8_t btmp = 0;
	
	btmp = NRF24L01_Read_Reg(NRF24L01_rtx, RF_SETUP );
	btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	
	if( Speed == SPEED_250K )		//250K
	{
		btmp |= ( 1<<5 );
	}
	else if( Speed == SPEED_1M )   //1M
	{
   		btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	}
	else if( Speed == SPEED_2M )   //2M
	{
		btmp |= ( 1<<3 );
	}

	NRF24L01_Write_Reg(NRF24L01_rtx, RF_SETUP, btmp );
}

 /**
  * @brief :设置功率
  * @param :
  *			@Speed:速度
  * @note  :无
  * @retval:无
  */
void NRF24L01_Set_Power(NRF24L01_rtx_t *NRF24L01_rtx, nRf24l01PowerType Power )
{
    uint8_t btmp;
	
	btmp = NRF24L01_Read_Reg(NRF24L01_rtx, RF_SETUP ) & ~0x07;
    switch( Power )
    {
        case POWER_F18DBM:
            btmp |= PWR_18DB;
            break;
        case POWER_F12DBM:
            btmp |= PWR_12DB;
            break;
        case POWER_F6DBM:
            btmp |= PWR_6DB;
            break;
        case POWER_0DBM:
            btmp |= PWR_0DB;
            break;
        default:
            break;
    }
    NRF24L01_Write_Reg(NRF24L01_rtx, RF_SETUP, btmp );
}

 /**
  * @brief :设置频率
  * @param :
  *			@FreqPoint:频率设置参数
  * @note  :值不大于127
  * @retval:无
  */
void RF24LL01_Write_Hopping_Point(NRF24L01_rtx_t *NRF24L01_rtx, uint8_t FreqPoint )
{
    NRF24L01_Write_Reg( NRF24L01_rtx, RF_CH, FreqPoint & 0x7F );
}

/**
  * @brief :NRF24L01检测
  * @param :无
  * @note  :无
  * @retval:无
  */ 
void NRF24L01_check( NRF24L01_rtx_t *NRF24L01_rtx )
{
	uint8_t i;
	uint8_t buf[5]={ 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	uint8_t read_buf[ 5 ] = { 0 };
	 
	while( 1 )
	{
		NRF24L01_Write_Buf(NRF24L01_rtx, TX_ADDR, buf, 5 );			//写入5个字节的地址
		NRF24L01_Read_Buf(NRF24L01_rtx, TX_ADDR, read_buf, 5 );		//读出写入的地址  
		for( i = 0; i < 5; i++ )
		{
			if( buf[ i ] != read_buf[ i ] )
			{
				break;
			}	
		} 
		
		if( 5 == i )
		{
			break;
		}
		else
		{
			//drv_uart_tx_bytes( (uint8_t *)g_ErrorString, 26 );
		}
		HAL_Delay( 20);
	}
}

 /**
  * @brief :设置模式
  * @param :
  *			@Mode:模式发送模式或接收模式
  * @note  :无
  * @retval:无
  */
void RF24L01_Set_Mode(NRF24L01_rtx_t *NRF24L01_rtx , nRf24l01ModeType Mode )
{
    uint8_t controlreg = 0;
	controlreg = NRF24L01_Read_Reg(NRF24L01_rtx, CONFIG );
	RF24L01_SET_CE_LOW( );
    if( Mode == MODE_TX )       
	{
		//controlreg &= ~( 1<< PRIM_RX );
		 NRF24L01_Write_Reg(NRF24L01_rtx, CONFIG, 0x0e );
	}
    else 
	{
		if( Mode == MODE_RX )  
		{ 
			//controlreg |= ( 1<< PRIM_RX ); 
			 NRF24L01_Write_Reg(NRF24L01_rtx, CONFIG, 0x0f );
		}
	}

    //NRF24L01_Write_Reg(NRF24L01_rtx, CONFIG, controlreg );
	RF24L01_SET_CE_HIGH( );
}

/**
  * @brief :NRF24L01发送一次数据
  * @param :
  *			@txbuf:待发送数据首地址
  *			@Length:发送数据长度
  * @note  :无
  * @retval:
  *			MAX_TX：达到最大重发次数
  *			TX_OK：发送完成
  *			0xFF:其他原因
  */ 
uint8_t NRF24L01_TxPacket(NRF24L01_rtx_t *NRF24L01_rtx,NRF24L01_bus_t *NRF24L01_bus )
{
	uint8_t l_Status = 0;
	uint16_t l_MsTimes = 0;
	
	RF24L01_SET_CS_LOW( );		//片选
	NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle, FLUSH_TX );
	RF24L01_SET_CS_HIGH( );
	
	RF24L01_SET_CE_LOW( );
  
  NRF24L01_Write_Buf(NRF24L01_rtx, WR_TX_PLOAD,NRF24L01_bus->TxPacket.Txbuffer, NRF24L01_bus->TxPacket.Txlength);	//写数据到TX BUF 32字节  TX_PLOAD_WIDTH
	RF24L01_SET_CE_HIGH( );			//启动发送
	while( 0 != RF24L01_GET_IRQ_STATUS())
	{
		HAL_Delay(1);
		if( 500 == l_MsTimes++ )						//500ms还没有发送成功，重新初始化设备
		{
			RF24L01_Init(NRF24L01_rtx );
			RF24L01_Set_Mode(NRF24L01_rtx, MODE_TX );
			break;
		}
	}
	l_Status = NRF24L01_Read_Reg(NRF24L01_rtx,STATUS);						//读状态寄存器
	//NRF24L01_Write_Reg(NRF24L01_rtx, STATUS, l_Status );						//清除TX_DS或MAX_RT中断标志
	
	if( l_Status & MAX_TX )	//达到最大重发次数
	{
		NRF24L01_Write_Reg(NRF24L01_rtx, FLUSH_TX,0xff );	//清除TX FIFO寄存器
		return MAX_TX; 
	}
	if( l_Status & TX_OK )	//发送完成
	{
		return TX_OK;
	}
  
  return 0xFF;	//其他原因发送失败
}

/**
  * @brief :NRF24L01接收数据
  * @param :
  *			@rxbuf:接收数据存放地址
  * @note  :无
  * @retval:接收的数据个数
  */ 
uint8_t NRF24L01_RxPacket(NRF24L01_rtx_t *NRF24L01_rtx, NRF24L01_bus_t *NRF24L01_bus )
{
	uint8_t l_Status = 0, l_RxLength = 0, l_100MsTimes = 0;
	
	RF24L01_SET_CS_LOW( );		//片选
	NRF24L01_rtx->rtx_cmd(NRF24L01_rtx->handle, FLUSH_RX );
	RF24L01_SET_CS_HIGH( );
	
	while( 0 != RF24L01_GET_IRQ_STATUS())
	{
		HAL_Delay( 100 );
		
		if( 30 == l_100MsTimes++ )		//3s没接收过数据，重新初始化模块
		{
			RF24L01_Init(NRF24L01_rtx );
			RF24L01_Set_Mode(NRF24L01_rtx, MODE_RX );
			break;
		}
	}
	
	l_Status = NRF24L01_Read_Reg(NRF24L01_rtx, STATUS );		//读状态寄存器
	NRF24L01_Write_Reg(NRF24L01_rtx, STATUS,l_Status );		//清中断标志
	if( l_Status & RX_OK)	//接收到数据
	{
		l_RxLength = NRF24L01_Read_Reg(NRF24L01_rtx, R_RX_PL_WID );		//读取接收到的数据个数
		NRF24L01_Read_Buf(NRF24L01_rtx, RD_RX_PLOAD,NRF24L01_bus->RxPacket.Rxbuffer,l_RxLength );	//接收到数据 
		NRF24L01_Write_Reg(NRF24L01_rtx, FLUSH_RX,0xff );				//清除RX FIFO
		return l_RxLength; 
	}	
	
	return 0;				//没有收到数据	
}


 /**
  * @brief :RF24L01模块初始化
  * @param :无
  * @note  :无
  * @retval:无
  */
void RF24L01_Init( NRF24L01_rtx_t *NRF24L01_rtx )
{
    uint8_t addr[5] = {INIT_ADDR};

    RF24L01_SET_CE_LOW( );//genggai
       NRF24L01_Clear_IRQ_Flag( NRF24L01_rtx,IRQ_ALL );
#if DYNAMIC_PACKET == 1

    NRF24L01_Write_Reg(NRF24L01_rtx, DYNPD, ( 1 << 0 ) ); 	//使能通道1动态数据长度
    NRF24L01_Write_Reg(NRF24L01_rtx, FEATRUE, 0x07 );
    NRF24L01_Read_Reg(NRF24L01_rtx, DYNPD );
    NRF24L01_Read_Reg(NRF24L01_rtx, FEATRUE );
	
#elif DYNAMIC_PACKET == 0
    
    L01_WriteSingleReg( L01REG_RX_PW_P0, FIXED_PACKET_LEN );	//固定数据长度
	
#endif	//DYNAMIC_PACKET

    NRF24L01_Write_Reg(NRF24L01_rtx, CONFIG, /*( 1<<MASK_RX_DR ) |*/		//接收中断
                                      ( 1 << EN_CRC ) |     //使能CRC 1个字节
                                      ( 1 << PWR_UP ) );    //开启设备
    NRF24L01_Write_Reg(NRF24L01_rtx, EN_AA, 0x00 );   		//通道0自动应答
    NRF24L01_Write_Reg(NRF24L01_rtx, EN_RXADDR, 0x00 );		//通道0接收
    NRF24L01_Write_Reg(NRF24L01_rtx, SETUP_AW, AW_5BYTES );     			//地址宽度 5个字节
    NRF24L01_Write_Reg(NRF24L01_rtx, SETUP_RETR, 0x00 );         	//重复等待时间 250us
    NRF24L01_Write_Reg(NRF24L01_rtx, RF_CH, 0 );             			//初始化通道
    NRF24L01_Write_Reg(NRF24L01_rtx, RF_SETUP, 0x26 );

    NRF24L01_Set_TxAddr(NRF24L01_rtx, &addr[0], 5 );                      //设置TX地址
    NRF24L01_Set_RxAddr(NRF24L01_rtx, 0, &addr[0], 5 );                   //设置RX地址
    
     RF24L01_SET_CE_HIGH( );//genggai
}

static uint8_t NRF24L01_RT(void* handle,uint8_t TxByte)
{
	return drv_spi_read_write_byte(handle,TxByte);
}

 /**
  * @brief :SPI接口初始化
  * @param :无
  * @note  :无
  * @retval:无
  */
void RF24L01_Port_Init( void )
{
	NRF24L01_bus.handle=&SPI_PORT;
	NRF24L01_rtx.handle=&SPI_PORT;
	NRF24L01_rtx.rtx_cmd=NRF24L01_RT;
}
