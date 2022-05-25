#pragma once

#include "usart.h"

typedef __packed struct JY62
{
	float ax;	float ay; float az;
  float mod_a;
	
	float q0; float q1; float q2; float q3;
	float yaw; float pitch; float roll;
	uint8_t isOK;
	uint8_t end[2];
} jy62_t;

extern jy62_t jy62;
extern uint8_t IMU_size;

void IMU_Calc(jy62_t * jy62t, uint8_t* buffer);
static void IMU_Uart_with_DMA_Init(void);
void IMU_set_Z_Axis(UART_HandleTypeDef * huart, jy62_t * jy62t);
void IMU_set_OffsetEular(UART_HandleTypeDef * huart);
void IMU_Fetch_Test_IT(void);

static void set_acc(jy62_t * jy62, uint8_t * buffer, uint8_t index);
static void set_gyro(jy62_t * jy62, uint8_t * buffer, uint8_t index);
static void set_angle(jy62_t * jy62, uint8_t * buffer, uint8_t index);
static void set_quad(jy62_t * jy62, uint8_t * buffer, uint8_t index);
