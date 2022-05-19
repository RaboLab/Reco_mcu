#include "loop.h"
#include "loop_it.h"

#include "imu.h"
#include "imu_buff.h"
#include "gps.h"

#include "key.h"
#include "gpioo.h"

#include "vofa.h"

uint16_t RAB_Clk = 0;
uint8_t time_1ms = 0;		//1K	Hz
uint8_t time_10ms = 0;	//100	Hz
uint8_t time_20ms = 0;	//50	Hz
uint8_t time_100ms = 0;	//10	Hz
uint8_t time_1s = 0;		//1		Hz


void loop(void)
{
	HAL_Delay(100);  //
	Buz_ON();
	HAL_Delay(100);  //
	Buz_OFF();
	//Device Init//
	//IMU_Fetch_Init();
	
	//Communication Init//
	vofa.init();

	// Utils //
	GPS_Init();
	IMU_Init();
	while (1)
	{
		if(IMU_Ready)
		{
			IMU_Ready = 0;
			IMU_Calc(&jy62, IMU_Data);
		}
		if(GPS_Ready)
		{
			GPS_Ready = 0;
			IMU_Calc(&jy62, IMU_Data);
		}
		TaskCheck();
	}
}

void Task_1ms()
{
	
}
void Task_10ms()
{
}
void Task_20ms()
{
}
void Task_100ms()
{
}
void Task_1s()
{
	
}

void TaskCheck(void)
{
	if(time_1ms)		{time_1ms--;		Task_1ms();		}
	if(time_10ms)		{time_10ms--;		Task_10ms();	}
	if(time_20ms)		{time_20ms--;		Task_20ms();	}
	if(time_100ms)	{time_100ms--;	Task_100ms();	}
	if(time_1s)			{time_1s--;			Task_1s();		}
}

void RAB_Clock(void)
{
	RAB_Clk++;
	time_1ms = 1;
	if(!(RAB_Clk % 10))		{time_10ms++;			}
	if(!(RAB_Clk % 20))		{time_20ms++;			}
	if(!(RAB_Clk % 100))	{time_100ms++;		}
	if(!(RAB_Clk % 1000))	{time_1s++;			RAB_Clk = 0;	}
}

