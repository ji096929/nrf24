#include "remote_control.h"
#include "drv_RF24L01.h"
#include "adc.h"
remote_control ctrl;
uint8_t _buf[18] = {0};
static uint8_t state = 1;
static uint8_t _data_len = 16, _data_cnt = 0;
/**
 * @brief remote_control结构体复位
 *
 */
void Ctrl_Reset(void)
{
    // side top button
    ctrl.btnA = 0;
    ctrl.btnB = 0;
    ctrl.btnX = 0;
    ctrl.btnY = 0;
    ctrl.btnShare = 0;
    ctrl.btnStart = 0;
    ctrl.btnSelect = 0;
    ctrl.btnXbox = 0;
    ctrl.btnLB = 0;
    ctrl.btnRB = 0;
    ctrl.menu = 0;
    // button on joy stick

    ctrl.btnLS = 0;
    ctrl.btnRS = 0;
    ctrl.btnDirup = 0;
    ctrl.btnDirLeft = 0;
    ctrl.btnDirRight = 0;
    ctrl.btnDirDown = 0;

    ctrl.joyLHori = MAXJOY / 2;
    ctrl.joyLVert = MAXJOY / 2;
    ctrl.joyRHori = MAXJOY / 2;
    ctrl.joyRVert = MAXJOY / 2;
    ctrl.trigLT = 0;
    ctrl.trigRT = 0;
}

void Tx_Data_Prepare(NRF24L01_bus_t NRF24L01_bus)
{
    // 帧头
    NRF24L01_bus.TxPacket.Txbuffer[0] = 0xA5;

    // 控制器数据
    NRF24L01_bus.TxPacket.Txbuffer[1] = ctrl.joyLHori & 0xFF;        // 左摇杆水平行程低八位
    NRF24L01_bus.TxPacket.Txbuffer[2] = (ctrl.joyLHori >> 8) & 0xFF; // 左摇杆水平行程高八位
    NRF24L01_bus.TxPacket.Txbuffer[3] = ctrl.joyLVert & 0xFF;        // 左摇杆垂直行程低八位
    NRF24L01_bus.TxPacket.Txbuffer[4] = (ctrl.joyLVert >> 8) & 0xFF; // 左摇杆垂直行程高八位
    NRF24L01_bus.TxPacket.Txbuffer[5] = ctrl.joyRHori & 0xFF;        // 右摇杆水平行程低八位
    NRF24L01_bus.TxPacket.Txbuffer[6] = (ctrl.joyRHori >> 8) & 0xFF; // 右摇杆水平行程高八位
    NRF24L01_bus.TxPacket.Txbuffer[7] = ctrl.joyRVert & 0xFF;        // 右摇杆垂直行程低八位
    NRF24L01_bus.TxPacket.Txbuffer[8] = (ctrl.joyRVert >> 8) & 0xFF; // 右摇杆垂直行程高八位
    NRF24L01_bus.TxPacket.Txbuffer[9] = ctrl.trigLT & 0xFF;          // 左扳机行程低八位
    NRF24L01_bus.TxPacket.Txbuffer[10] = (ctrl.trigLT >> 8) & 0xFF;  // 左扳机行程高八位
    NRF24L01_bus.TxPacket.Txbuffer[11] = ctrl.trigRT & 0xFF;         // 右扳机行程低八位
    NRF24L01_bus.TxPacket.Txbuffer[12] = (ctrl.trigRT >> 8) & 0xFF;  // 右扳机行程高八位

    // 按键数据
    if (ctrl.btnDirup)
        NRF24L01_bus.TxPacket.Txbuffer[13] = 1;
    if (ctrl.btnDirup&&ctrl.btnDirRight)
        NRF24L01_bus.TxPacket.Txbuffer[13] = 2;
    if (ctrl.btnDirup&&ctrl.btnDirLeft)
        NRF24L01_bus.TxPacket.Txbuffer[13] = 8;
    if (ctrl.btnDirDown)
        NRF24L01_bus.TxPacket.Txbuffer[13] = 5;
    if (ctrl.btnDirDown&&ctrl.btnDirRight)
        NRF24L01_bus.TxPacket.Txbuffer[13] = 4;
    if (ctrl.btnDirDown&&ctrl.btnDirLeft)
        NRF24L01_bus.TxPacket.Txbuffer[13] = 6;
    if (ctrl.btnDirRight)
        NRF24L01_bus.TxPacket.Txbuffer[13] = 3;
    if (ctrl.btnDirLeft)
        NRF24L01_bus.TxPacket.Txbuffer[13] = 7;

    NRF24L01_bus.TxPacket.Txbuffer[14] = (ctrl.btnA ? (1 << 0) : 0) | (ctrl.btnB ? (1 << 1) : 0) | (ctrl.btnX ? (1 << 3) : 0) | (ctrl.btnY ? (1 << 4) : 0) | (ctrl.btnLB ? (1 << 6) : 0) | (ctrl.btnRB ? (1 << 7) : 0); // Byte13
    NRF24L01_bus.TxPacket.Txbuffer[15] = (ctrl.menu ? (1 << 2) : 0) | (ctrl.btnXbox ? (1 << 3) : 0) | (ctrl.btnLS ? (1 << 4) : 0) | (ctrl.btnRS ? (1 << 5) : 0);                                                        // Byte14
    NRF24L01_bus.TxPacket.Txbuffer[16] = (ctrl.btnShare ? 1 : 0);                                                                                                                                                       // Byte15

    // 帧尾
    NRF24L01_bus.TxPacket.Txbuffer[17] = 0xA6;
}

void Send_control_message(NRF24L01_rtx_t NRF24L01_rtx, NRF24L01_bus_t NRF24L01_bus)
{
	 ctrl.joyLHori=left_x;
	ctrl.joyLVert=left_y;
	ctrl.joyRHori=right_x;
	ctrl.joyRVert=right_y;
    Tx_Data_Prepare(NRF24L01_bus);
    NRF24L01_TxPacket(&NRF24L01_rtx, &NRF24L01_bus);
}
