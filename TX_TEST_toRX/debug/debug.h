#ifndef __DEBUG__
#define __DEBUG__

#include "main.h"
#include "drv_RF24L01.h"
#include "string.h"

#define NAME_LENGTH   13
#define VALUE_MAX     16
#define NAME_MAX     16
#define INIT_ID      0x10
#define PACKET_MAX   32

#define F_TYPE     3
#define UI_TYPE    2
#define I_TYPE     1

#define ERROR         0	
#define OK            1
#define ERROR         0

typedef  uint8_t  Status;

enum  PacketType{
    ValueType=0x0A,NameType=0x0B
};

typedef __packed struct   Value
{
    uint8_t id;  //值对应变量的id
    uint16_t value;  //变量值
    uint8_t none;  //保留位 存放变量类型以及其他信息
}Value;

typedef __packed struct  Name
{
    uint8_t id;  //名字对应变量的id
    char name[NAME_LENGTH];  //变量名数组
    uint8_t instruct;  //附加指令
}Name;

typedef __packed struct  ValuePacket
{
    enum PacketType type;
    Value value[VALUE_MAX]; 
}ValuePacket;

typedef __packed struct  NamePacket
{
    enum PacketType type;
    Name name[NAME_MAX];   //最多存放16个变量名
}NamePacket;

typedef __packed struct  TransmitPacket
{
    ValuePacket value_list;
    NamePacket  name_lit;
    uint8_t  parameter_num;
    void* parameter_locate[VALUE_MAX];
}TransmitPacket;

typedef  __packed struct  ReceivePacket
{
    char rec_data[2];
}ReceivePacket;

Status TransmitName(TransmitPacket* transmit);
Status TransmitValue(TransmitPacket* transmit);
Status AddParameter_i(const char name[],int16_t* parameter,TransmitPacket* transmit);
Status AddParameter_f(const char name[],float* parameter,TransmitPacket* transmit);
Status AddParameter_ui(const char name[],uint16_t* parameter,TransmitPacket* transmit);
void InitPacket(void);
Status TransmitParameter(void);


#endif 