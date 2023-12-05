#include "debug.h"
#include "drv_RF24L01.h"
#include <string.h>
ReceivePacket_t Receive_packet;
uint8_t receive_flag = 0;
Value_True_t Value_True;

Status IsAdd(uint8_t id,Value_True_t* Value_True);

void InitPacket(void)
{
    memset(&Receive_packet, 0, sizeof(Receive_packet));
    for (int i = 0; i < VALUE_MAX; i++)
    {
        Receive_packet.parameter_locate[i] = NULL;
    }
    Receive_packet.value_list.type = ValueType;
    Receive_packet.name_list.type = NameType;
    Receive_packet.parameter_num = 0;
}

Status ReceivePacket_fn(Value_True_t* Value_True)
{

    NRF24L01_RxPacket(&NRF24L01_rtx, &NRF24L01_bus);
	
    uint8_t num=NRF24L01_bus.RxPacket.Rxbuffer[1];    
	
    if (NRF24L01_bus.RxPacket.Rxbuffer[0] == 0x0a) // ����ֵ
    {
			  uint8_t offset = 0;
        int len = sizeof(Value_t);
			  uint8_t rec_buffer[PACKET_MAX]={0};
				memcpy(rec_buffer, (NRF24L01_bus.RxPacket.Rxbuffer + 2), PACKET_MAX-2);       
				while ((offset + len) <= PACKET_MAX)
				{
						for (int i = 0; i < Value_True->num_16;i++){
								if (Value_True->Value_True_16[i].id == rec_buffer[offset])
										Value_True->Value_True_16[i].value = (int16_t)((uint16_t)(rec_buffer[offset+2]<<8)|rec_buffer[offset+1]);
						}
						for (int i = 0; i < Value_True->num_u16; i++)
						{
								if (Value_True->Value_True_u16[i].id == rec_buffer[offset])
										Value_True->Value_True_u16[i].value = (uint16_t)(rec_buffer[offset+2]<<8)|rec_buffer[offset+1];
						}
						for (int i = 0; i < Value_True->num_32; i++)
						{
								if (Value_True->Value_True_32[i].id == rec_buffer[offset])
										Value_True->Value_True_32[i].value = (float)((uint16_t)(rec_buffer[offset+2]<<8)|rec_buffer[offset+1]) / 100.0f;
						}
						offset += len;
				}
				return OK;
    }
    else
    {
        uint8_t offset = 0;
        int len = sizeof(Name_t);
        uint8_t rec_buffer[PACKET_MAX]={0};
				memcpy(rec_buffer, (NRF24L01_bus.RxPacket.Rxbuffer + 2), PACKET_MAX-2);
				while ((offset + len) <= PACKET_MAX)
				{
						switch (rec_buffer[offset+NAME_LENGTH+1])
						{
							case I_TYPE:
								if(!IsAdd(rec_buffer[offset],Value_True)){
									Value_True->Value_True_16[Value_True->num_16].id=rec_buffer[offset];
									memcpy(Value_True->Value_True_16[Value_True->num_16++].name, rec_buffer+offset+1, NAME_LENGTH);
								}
								break;
							case UI_TYPE:
								if(!IsAdd(rec_buffer[offset],Value_True)){
									Value_True->Value_True_u16[Value_True->num_u16].id = rec_buffer[offset];
									memcpy(Value_True->Value_True_u16[Value_True->num_u16++].name, rec_buffer+offset+1, NAME_LENGTH);
								}
								break;
							case F_TYPE:
								if(!IsAdd(rec_buffer[offset],Value_True)){
									Value_True->Value_True_32[Value_True->num_32].id = rec_buffer[offset];
									memcpy(Value_True->Value_True_32[Value_True->num_32++].name, rec_buffer+offset+1, NAME_LENGTH);												
								}
								break;
						}
						offset += len;
				}
				return OK;
		}
}

Status IsAdd(uint8_t id,Value_True_t* Value_True){
	for (int i = 0; i < Value_True->num_16;i++){
			if (Value_True->Value_True_16[i].id == id)
					return OK;
	}
	for (int i = 0; i < Value_True->num_u16; i++)
	{
			if (Value_True->Value_True_u16[i].id == id)
					return OK;
	}
	for (int i = 0; i < Value_True->num_32; i++)
	{
			if (Value_True->Value_True_32[i].id == id)
					return OK;
	}
	return ERROR;
}

