#include "debug.h"
#include "string.h"
#include "drv_RF24L01.h"


uint8_t transmit_flag=0;
TransmitPacket transmit_packet;
ReceivePacket  receive_packet;



void InitPacket(void){
    memset(&transmit_packet,0,sizeof(transmit_packet));
    memset(&receive_packet,0,sizeof(ReceivePacket));
    for (int i = 0; i < VALUE_MAX; i++) {
        transmit_packet.parameter_locate[i]=NULL;
    }
    transmit_packet.value_list.type=ValueType;
    transmit_packet.name_lit.type=NameType;
    transmit_packet.parameter_num=0;
}

/*
* 简介：添加int16类型的变量
* 参数列表：const char name[]  变量名，0-13字符
*/
Status AddParameter_i(const char name[],int16_t* parameter,TransmitPacket* transmit){
    if(transmit->parameter_num<=VALUE_MAX){
        if(strlen(name)>NAME_LENGTH) return ERROR;
        else{
            int id=INIT_ID+transmit->parameter_num;  //计算变量id
            transmit->name_lit.name[transmit->parameter_num].id=id;  
            transmit->value_list.value[transmit->parameter_num].id=id;
            transmit->name_lit.name[transmit->parameter_num].instruct=I_TYPE;  //标注为int16类型
            strcpy(transmit->name_lit.name[transmit->parameter_num].name,name);  //存放变量名字
            transmit->parameter_locate[transmit->parameter_num]=parameter;  //存放变量地址
            transmit->parameter_num++;
            return OK;
            }
        }
    else return ERROR;
}

Status AddParameter_ui(const char name[],uint16_t* parameter,TransmitPacket* transmit){
    if(transmit->parameter_num<=VALUE_MAX){
        if(strlen(name)>NAME_LENGTH) return ERROR;
        else{
            int id=INIT_ID+transmit->parameter_num;  //计算变量id
            transmit->name_lit.name[transmit->parameter_num].id=id;  
            transmit->value_list.value[transmit->parameter_num].id=id;
            transmit->name_lit.name[transmit->parameter_num].instruct=UI_TYPE;   //标注为uint6类型
            strcpy(transmit->name_lit.name[transmit->parameter_num].name,name);  //存放变量名字
            transmit->parameter_locate[transmit->parameter_num]=parameter;  //存放变量地址
            transmit->parameter_num++;
            return OK;
            }
        }
    else return ERROR;
}

Status AddParameter_f(const char name[],float* parameter,TransmitPacket* transmit){
    if(transmit->parameter_num<=VALUE_MAX){
        if(strlen(name)>NAME_LENGTH) return ERROR;
        else{
            int id=INIT_ID+transmit->parameter_num;  //计算变量id
            transmit->name_lit.name[transmit->parameter_num].id=id;  
            transmit->value_list.value[transmit->parameter_num].id=id;
            transmit->name_lit.name[transmit->parameter_num].instruct=F_TYPE;  //标注为float类型
            strcpy(transmit->name_lit.name[transmit->parameter_num].name,name);  //存放变量名字
            transmit->parameter_locate[transmit->parameter_num]=parameter;  //存放变量地址
            transmit->parameter_num++;
            return OK;
            }
        }
    else return ERROR;
}
uint8_t value_tx_buffer[PACKET_MAX]={0};
Status TransmitValue(TransmitPacket* transmit){

    uint8_t offset=0;
    uint8_t parameter_cnt=0;
    uint8_t len=sizeof(Value); 
      
	
    while(parameter_cnt<transmit->parameter_num){
			
        memset(value_tx_buffer,0,PACKET_MAX);
        value_tx_buffer[offset++]=transmit->value_list.type;
			  value_tx_buffer[offset++]=transmit->parameter_num;
			
        while((offset+len)<=PACKET_MAX){
            //根据变量类型更新变量的值
            switch (transmit->name_lit.name[parameter_cnt].instruct)
            {
                case I_TYPE:
                case UI_TYPE:
                transmit->value_list.value[parameter_cnt].value=*(uint16_t*)transmit->parameter_locate[parameter_cnt];
								//memcpy(&transmit->value_list.value[parameter_cnt].value,transmit->parameter_locate[parameter_cnt],sizeof(uint16_t));
                break;
                case F_TYPE:
                transmit->value_list.value[parameter_cnt].value=(uint16_t)((*(float*)transmit->parameter_locate[parameter_cnt])*100);
                break;
            }
            memcpy((value_tx_buffer+offset),(transmit->value_list.value+parameter_cnt++),len);
            offset+=len;
            if(parameter_cnt==transmit->parameter_num) break;						
        }
				memcpy(NRF24L01_bus.TxPacket.Txbuffer,value_tx_buffer,PACKET_MAX);
        NRF24L01_TxPacket(&NRF24L01_rtx,&NRF24L01_bus);  //发送这一包数据
        offset=0;
        HAL_Delay(10);        
    }
    return OK;
}

uint8_t tx_buffer[PACKET_MAX]={0};
Status TransmitName(TransmitPacket* transmit){

    uint8_t offset=0;
    uint8_t parameter_cnt=0;
    uint8_t len=sizeof(Name); 
	  
    while(parameter_cnt<transmit->parameter_num){
			   
        memset(tx_buffer,0,PACKET_MAX);
        tx_buffer[offset++]=transmit->name_lit.type;
        tx_buffer[offset++]=transmit->parameter_num;
			
        while((offset+len)<=PACKET_MAX){
            //更新变量的值
            memcpy((tx_buffer+offset),(transmit->name_lit.name+parameter_cnt++),len);
            offset+=len;  
             if(parameter_cnt==transmit->parameter_num) break;					
        } 
				memcpy(NRF24L01_bus.TxPacket.Txbuffer,tx_buffer,PACKET_MAX);
        NRF24L01_TxPacket(&NRF24L01_rtx,&NRF24L01_bus);  //发送这一包数据
        offset=0;        
				HAL_Delay(10);
    }
    return OK;
}


Status TransmitParameter(void){
    if(transmit_flag<50){
        TransmitName(&transmit_packet);
        transmit_flag++;
    }
		else
		{
			TransmitValue(&transmit_packet);
		}
    return OK;
}

