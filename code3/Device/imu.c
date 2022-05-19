#include "imu.h"
#include "string.h"

jy62_t jy62;

__IO uint8_t sumCheck;

void IMU_Calc(jy62_t * jy62t, uint8_t* buffer)
{
	for(int x = 0; x < 4; ++x)
	{
		sumCheck = 0;
		for(int y = 0; y < 10; ++y)
			sumCheck += buffer[ y + x * 11 ];
		if (sumCheck == buffer[ x * 11 + 10 ])
		{
			if(buffer[ x * 11 + 1 ] == 0x51)
				set_acc(jy62t, buffer, x * 11 + 1);
			else if (buffer[ x * 11 + 1 ] == 0x52)
				set_gyro(jy62t, buffer, x * 11 + 1);
			else if (buffer[ x * 11 + 1 ] == 0x53)
				set_angle(jy62t, buffer, x * 11 + 1);
			else if (buffer[ x * 11 + 1 ] == 0x59)
				set_quad(jy62t, buffer, x * 11 + 1);
		}
	}
}
 
__IO uint8_t thisTimeRx = 0;

void IMU_set_Z_Axis(UART_HandleTypeDef * huart, jy62_t * jy62t)
{
	// 1. Unlock
	uint8_t Unlock[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
	uint8_t Data[5] = {0xFF, 0xAA, 0x01, 0x04, 0x00};
	uint8_t Save[5] = {0xFF, 0xAA, 0x00, 0x00, 0x00};
	
	HAL_UART_Transmit(huart, Unlock, 5, 0xFFFF);
	HAL_Delay(1000);
	
	// 2. Set
	HAL_UART_Transmit(huart, Data, 5, 0xFFFF);
	HAL_Delay(1000);
	// 3. Store
	
	HAL_UART_Transmit(huart, Save, 5, 0xFFFF);
	HAL_Delay(1000);
}

void IMU_set_OffsetEular(UART_HandleTypeDef * huart, jy62_t * jy62t)
{
	// 1. Unlock
	uint8_t Unlock[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
	uint8_t Data[5] = {0xFF, 0xAA, 0x01, 0x08, 0x00};
	uint8_t Save[5] = {0xFF, 0xAA, 0x00, 0x00, 0x00};
	
	HAL_UART_Transmit(huart, Unlock, 5, 0xFFFF);
	HAL_Delay(1000);
	
	// 2. Set
	HAL_UART_Transmit(huart, Data, 5, 0xFFFF);
	HAL_Delay(1000);
	// 3. Store
	
	HAL_UART_Transmit(huart, Save, 5, 0xFFFF);
	HAL_Delay(1000);
}

static inline void set_acc(jy62_t * jy62, uint8_t * buffer, uint8_t index)
{
	memcpy(jy62->Data, &(buffer[index+1]), 6);
}
static inline void set_gyro(jy62_t * jy62, uint8_t * buffer, uint8_t index)
{
	memcpy(&(jy62->Data[6]), &(buffer[index+1]), 8);
}

static inline void set_angle(jy62_t * jy62, uint8_t * buffer, uint8_t index)
{
	memcpy(&(jy62->Data[14]), &(buffer[index+1]), 6);

}
static inline void set_quad(jy62_t * jy62, uint8_t * buffer, uint8_t index)
{
	memcpy(&(jy62->Data[20]), &(buffer[index+1]), 8);
}
