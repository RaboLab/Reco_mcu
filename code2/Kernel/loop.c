#include "loop.h"
#include "loop_it.h"

#include "imu.h"

#include "vofa.h"

uint16_t time_1ms_flag = 0;
uint8_t time_10ms_flag = 0;
uint8_t time_20ms_flag = 0;
uint8_t time_50ms_flag = 0;
uint8_t time_100ms_flag = 0;

void loop(void)
{
	HAL_Delay(100);  //

	//Device Init//
	//IMU_Fetch_Init();
	
	//Communication Init//
	vofa.init();

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);		//LED ON
	HAL_GPIO_WritePin(RPW_GPIO_Port, RPW_Pin, GPIO_PIN_SET);			//PWR ON

		HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_RESET);
	while (1)
	{
		HAL_GPIO_TogglePin(BUZZER_GPIO_Port,BUZZER_Pin);
		HAL_Delay(500);
		//TaskCheck();
	}
}


void TaskCheck(void)
{
	if(time_10ms_flag)		{time_10ms_flag--;		time_10ms_flag_func();		}
	if(time_20ms_flag)		{time_20ms_flag--;		time_20ms_flag_func();		}
	if(time_50ms_flag)		{time_50ms_flag--;		time_50ms_flag_func();		}
	if(time_100ms_flag)		{time_100ms_flag--;		time_100ms_flag_func();	}
}

void time_systick_flag_func()
{
	
}
void time_1ms_flag_func(void)
{
	
//	vofa.fdata.set_data(0,tad2141.angle_data);
//	vofa.fdata.set_data(1,tad2141.rectify_angle);
//	vofa.fdata.set_data(2,motor_control.real_location);
//	vofa.fdata.set_data(3,motor_control.est_location);
//	vofa.fdata.set_data(4,motor_control.goal_location);
//	vofa.fdata.set_data(5,((double)(motor_control.goal_location - motor_control.real_location))/65535.0*360);
//	vofa.fdata.send(6);
}

void time_10ms_flag_func(void)
{
	
}

void time_20ms_flag_func(void)
{

}

void time_50ms_flag_func(void)
{
	
}

void time_100ms_flag_func(void)
{}

void time_1ms_clock(void)
{
	time_1ms_flag++;
	time_1ms_flag_func();
	if(!(time_1ms_flag % 10))		{time_10ms_flag++;		}
	if(!(time_1ms_flag % 20))		{time_20ms_flag++;		}
	if(!(time_1ms_flag % 50))		{time_50ms_flag++;		}
	if(!(time_1ms_flag % 100))	{time_100ms_flag++;		}
	if(!(time_1ms_flag % 1000))	{time_1ms_flag = 0;		}
}

