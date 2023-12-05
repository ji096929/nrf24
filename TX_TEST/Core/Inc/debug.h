#include "main.h"

#define NAME_LENGTH 13
#define VALUE_MAX 16
#define NAME_MAX 16
#define INIT_ID 0x10
#define PACKET_MAX 32
#define PARA_MAX 16
#define F_TYPE 3
#define UI_TYPE 2
#define I_TYPE 1

#define ERROR 0
#define OK 1
#define ERROR 0

typedef int Status;

enum PacketType
{
    ValueType = 0x0A,
    NameType = 0x0B
};

enum DebugStatus
{
    Disconnected = 0,
    Connected = 1
};

typedef __packed struct 
{
    uint8_t id;     // 值对应变量的id
    uint16_t value; // 变量值
    uint8_t none;   // 保留位 存放变量类型以及其他信息
} Value_t;

typedef __packed struct 
{
    uint8_t id;                // 名字对应变量的id
    uint8_t name[NAME_LENGTH]; // 变量名数组
    uint8_t instruct;          // 附加指令
} Name_t;

typedef __packed struct 
{
    enum PacketType type;
    Value_t value[VALUE_MAX];
} ValuePacket_t;

typedef __packed struct 
{
    enum PacketType type;
    Name_t name[NAME_MAX]; // 最多存放16个变量名
} NamePacket_t;

typedef __packed struct 
{
    ValuePacket_t value_list;
    NamePacket_t name_list;
    uint8_t parameter_num;
    void *parameter_locate[VALUE_MAX];
} ReceivePacket_t;


typedef __packed struct 
{
    uint8_t id;     // 值对应变量的id
    uint16_t value; // 变量值
	uint8_t name[NAME_LENGTH];
} Value_True_u16_t;

typedef __packed struct 
{
    uint8_t id;     // 值对应变量的id
    int16_t value; // 变量值
	uint8_t name[NAME_LENGTH];
} Value_True_16_t;

typedef __packed struct 
{
    uint8_t id;     // 值对应变量的id
    float value; // 变量值
	uint8_t name[NAME_LENGTH];
} Value_True_32_t;


typedef __packed struct 
{
    Value_True_u16_t Value_True_u16[PARA_MAX];
    Value_True_16_t Value_True_16[PARA_MAX];
	Value_True_32_t Value_True_32[PARA_MAX];
	uint8_t num_u16;
	uint8_t num_16;
	uint8_t num_32;
} Value_True_t;

extern Value_True_t Value_True;
extern ReceivePacket_t Receive_packet;

void InitPacket(void);

Status ReceivePacket_fn(Value_True_t* Value_True);

