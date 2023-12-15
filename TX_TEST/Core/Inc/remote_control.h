#ifndef __REMOTE_CONTROL_H
#define __REMOTE_CONTROL_H
#include <stdbool.h>
#include <main.h>
#include "drv_RF24L01.h"

#define MAXJOY 65535


typedef struct 
{
    //side top button
    bool btnA;
    bool btnB;
    bool btnX;
    bool btnY;
    bool btnShare;
    bool btnStart;
    bool btnSelect;
    bool btnXbox;
    bool btnLB;
    bool btnRB;
    bool menu;
    //button on joy stick
  
    bool btnLS;
    bool btnRS;
    bool btnDirup;
    bool btnDirLeft;
    bool btnDirRight;
    bool btnDirDown;

    uint16_t joyLHori;
    uint16_t joyLVert;
    uint16_t joyRHori;
    uint16_t joyRVert;
    uint16_t trigLT;
    uint16_t trigRT;
} remote_control;
extern uint8_t _buf[18];
extern remote_control ctrl;
void Send_control_message(NRF24L01_rtx_t NRF24L01_rtx, NRF24L01_bus_t NRF24L01_bus);
#endif

