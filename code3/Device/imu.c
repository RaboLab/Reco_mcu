#include "imu.h"
#include "string.h"

jy62_t jy62;

__IO uint8_t sumCheck;

uint8_t IMU_size = sizeof(jy62_t);
void IMU_Calc(jy62_t * jy62t, uint8_t* buffer)
{
	for(uint8_t x = 0; x < 4; x++) 		//  51 52 53 59
	{
		sumCheck = 0;
		for(uint8_t y = 0; y < 10; ++y)
		{
			sumCheck +=   buffer[ x * 11 + y ];
		}
		if (sumCheck == buffer[ x * 11 + 10 ])			//sumCheck pass
		{			
			if(x == 0)
			{
				jy62t->ax = ((int16_t)((buffer[x * 11 + 3]<<8)|buffer[x * 11 + 2])) / 32768.0 * 16.0; //g
				jy62t->ay = ((int16_t)((buffer[x * 11 + 5]<<8)|buffer[x * 11 + 4])) / 32768.0 * 16.0;
				jy62t->az = ((int16_t)((buffer[x * 11 + 7]<<8)|buffer[x * 11 + 6])) / 32768.0 * 16.0;
				jy62t->mod_a = (jy62t->az)*(jy62t->az) + (jy62t->ay)*(jy62t->ay) + (jy62t->ax)*(jy62t->ax);
			}
			else if(x == 2)
			{
				jy62t->roll =  ((int16_t)((buffer[x * 11 + 3]<<8) | buffer[x * 11 + 2]))/ 32768.0 * 180.0; 
				jy62t->pitch = ((int16_t)((buffer[x * 11 + 5]<<8) | buffer[x * 11 + 4]))/ 32768.0 * 180.0; 
				jy62t->yaw =   ((int16_t)((buffer[x * 11 + 7]<<8) | buffer[x * 11 + 6]))/ 32768.0 * 180.0; 
			}
			else if(x == 3)
			{
				jy62t->q0 = ((int16_t)((buffer[x * 11 + 3]<<8) | buffer[x * 11 + 2]))/ 32768.0;
				jy62t->q1 = ((int16_t)((buffer[x * 11 + 5]<<8) | buffer[x * 11 + 4]))/ 32768.0;
				jy62t->q2 = ((int16_t)((buffer[x * 11 + 7]<<8) | buffer[x * 11 + 6]))/ 32768.0;
				jy62t->q3 = ((int16_t)((buffer[x * 11 + 9]<<8) | buffer[x * 11 + 8]))/ 32768.0;
			}
			jy62t->isOK = 1;
		}
		else 
		{
			jy62t->isOK = 0;
			return;
		}
	}
}

void IMU_set_OffsetEular(UART_HandleTypeDef * huart)
{
	// 1. Unlock
	uint8_t Unlock[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
	uint8_t Data[5] = {0xFF, 0xAA, 0x01, 0x08, 0x00};
	uint8_t Save[5] = {0xFF, 0xAA, 0x00, 0x00, 0x00};
	
	HAL_UART_Transmit(huart, Unlock, 5, 0xFFFF);
	HAL_Delay(100);
	
	// 2. Set
	HAL_UART_Transmit(huart, Data, 5, 0xFFFF);
	HAL_Delay(100);
	// 3. Store
	
	HAL_UART_Transmit(huart, Save, 5, 0xFFFF);
	HAL_Delay(100);
}

