#pragma once

#include "usart.h"

extern uint8_t IMU_Ready;
extern uint8_t IMU_Data[44];


void IMU_Init(void);
void IMU_RxCallback(void);
