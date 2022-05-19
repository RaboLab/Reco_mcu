#pragma once

#include "usart.h"

typedef struct JY62
{
	float ax;		//only user accel
	float ay;
	float az;

	uint8_t Number;			// face  helmet  camera
	uint8_t next;           
	uint8_t Data[28];		//acc 6  gyro 6  temp 2  angle 6  quad 8 
	uint8_t end;
} jy62_t;

extern jy62_t jy62;

void IMU_Calc(jy62_t * jy62t, uint8_t* buffer);
static void IMU_Uart_with_DMA_Init(void);
void IMU_set_Z_Axis(UART_HandleTypeDef * huart, jy62_t * jy62t);
void IMU_set_OffsetEular(UART_HandleTypeDef * huart, jy62_t * jy62t);
void IMU_Fetch_Test_IT(void);

static void set_acc(jy62_t * jy62, uint8_t * buffer, uint8_t index);
static void set_gyro(jy62_t * jy62, uint8_t * buffer, uint8_t index);
static void set_angle(jy62_t * jy62, uint8_t * buffer, uint8_t index);
static void set_quad(jy62_t * jy62, uint8_t * buffer, uint8_t index);
