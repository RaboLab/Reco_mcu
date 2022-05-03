#ifndef __JY62_IMU_H__
#define __JY62_IMU_H__

#include "usart.h"

typedef struct JY62
{
	/*
	float q0;
	float q1;
	float q2;
	float q3;

	float ax;
	float ay;
	float az;

	float wx;
	float wy;
	float wz;

	float temp;
	*/
	uint8_t Number;			// face  helmet  camera
	uint8_t next;           
	uint8_t Data[28];		//acc 6  gyro 6  temp 2  angle 6  quad 8 
	uint8_t end;
} jy62_t;


void IMU_fetch_IDLE_IRQHandle(UART_HandleTypeDef * huart);

static void IMU_Uart_with_DMA_Init(void);
void IMU_set_Z_Axis(UART_HandleTypeDef * huart, jy62_t * jy62t);
void IMU_set_OffsetEular(UART_HandleTypeDef * huart, jy62_t * jy62t);
void IMU_Fetch_Init(void);
void IMU_Fetch_Test_IT(void);


#endif
