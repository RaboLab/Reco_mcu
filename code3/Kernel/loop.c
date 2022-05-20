#include "loop.h"
#include "loop_it.h"

#include "imu.h"
#include "imu_buff.h"
#include "gps.h"
#include "ring.h"

#include "key.h"
#include "gpioo.h"

#include "vofa.h"

uint16_t RAB_Clk = 0;
uint8_t time_1ms = 0;		//1K	Hz
uint8_t time_10ms = 0;	//100	Hz
uint8_t time_20ms = 0;	//50	Hz
uint8_t time_100ms = 0;	//10	Hz
uint8_t time_1s = 0;		//1		Hz

typedef enum WrokState_enum
{
	zero,
	inDemo,
	waitOnline,
  lookVideo,
	antiTheif,
	log,
}WorkState_t;

WorkState_t workState = zero;
uint8_t buz_time;
uint8_t IMU_Heavy;
uint8_t isSave;
uint8_t isLook;
uint8_t rbpOnline;
uint8_t rbpRx;
uint8_t rbpACK; 
void loop(void)
{

	
	//Communication Init//
	vofa.init();

	//Device Init//
	GPS_Init();
	IMU_Init();
	HAL_UART_Receive_IT(&RBP_UART, &rbpRx, 1);
	// Utils //
	
	RAB_GPIO_Read(KEY_GPIO_Port);
	RAB_GPIO_Read(KEY_GPIO_Port);
	
	if(RAB_GPIO_Cnt(KEY_GPIO_Port) & KEY_Pin)	// enter demo mode
	{
		workState = inDemo;
		Buz_ON();
		HAL_Delay(1000);
		Buz_OFF();
		goto demo;
	}
	
	workState = waitOnline;
	while( gps.isOK == 0 || jy62.isOK == 0)		//wait until those online
	{
		if(IMU_Ready)
		{
			IMU_Calc(&jy62, IMU_Data);
			IMU_Ready = 0;
		}
		if(GPS_Ready)
		{
			GPS_GetRawData();
			GPS_Ready = 0;
		}
	}
	
	Buz_ON();
	HAL_Delay(100);  //
	Buz_OFF();
	
	workState = antiTheif;		//power up mode
	while (1)			////////////////////////////////////		// normal while 1
	{
		if(IMU_Ready)
		{
			IMU_Calc(&jy62, IMU_Data);
			IMU_Ready = 0;
			pack_load(&jy62, IMU_size);
		}
		if(GPS_Ready)
		{
			GPS_GetRawData();
			GPS_Ready = 0;
			pack_load(&gps, GPS_size);
		}
		TaskCheck();
		
	}
demo:
	while (1)
	{
		if(IMU_Ready)
		{
			IMU_Calc(&jy62, IMU_Data);
			IMU_Ready = 0;
		}
		if(GPS_Ready)
		{
			GPS_GetRawData();
			GPS_Ready = 0;
		}
		//TaskCheck();
	}
}

pack_t* open;
void Task_1ms()
{
	if(jy62.mod_a >= 3)
	{
		IMU_Heavy = 1;
	}
	
	open = pack_open();
	if(open == NULL)
		return;
	HAL_UART_Transmit(&RBP_UART, open->load, open->lens, 0xFF);
}
void Task_10ms()
{
	
}

void Task_20ms()	
{
	static uint32_t LONG_PRESS = 0;
	static uint32_t PRESS_CNT = 0;
	RAB_GPIO_Read(KEY_GPIO_Port);
	if(RAB_GPIO_Trg(KEY_GPIO_Port) & KEY_Pin) 		//Key Trigerd, Start to count time while press
	{
		PRESS_CNT = 1;
	}
	if((RAB_GPIO_Cnt(KEY_GPIO_Port) & KEY_Pin)) 		//Key Pressed
	{
		LONG_PRESS++;
	 (LONG_PRESS == 1)   ? buz_time = 1					//short press   //save
	:(LONG_PRESS == 50)  ? buz_time = 1					//1s - 2s	      //look
	:(LONG_PRESS == 100) ? buz_time = 1					//2s - 3s				//anti
	:                      0;
	}	
	else		// KEY UP
	{
		if(PRESS_CNT)			//indicates press is over
		{
			 (LONG_PRESS <= 50)  ? PRESS_CNT = 1   //SAVE		//0s - 1s
			:(LONG_PRESS <= 100) ? PRESS_CNT = 2   //LOOK		//1s - 2s
			:											(PRESS_CNT = 3); //Anti   //2s - 3s
			
			if(workState == log)
			{
				if(PRESS_CNT == 1)
					isSave = 1;
				else if(PRESS_CNT == 2)
					workState = lookVideo;
				else if(PRESS_CNT == 3)
					workState = antiTheif;
			}
			
			else if(workState == antiTheif)
			{
				workState = log;
				RPW_ON();
			}
			
			PRESS_CNT = 0;
			LONG_PRESS = 0;
		}
		else							//nothing happened
		{
			
		}
	}
}
void Task_100ms()			//IO
{
	if(IMU_Heavy)
	{
		LED_ON();
	}
	else
	{
		LED_OFF();
	}
	
	if(buz_time)
	{
		Buz_ON();
		buz_time--;
	}
	else
	{
		buz_time = 0;
		Buz_OFF();
	}
	if(isSave)
	{
		
	}
	else
	{
		
	}
}
void Task_1s()
{
	static uint8_t IMU_Heavy_CleanCnt = 0;
	static uint8_t RBP_PW_DOWN_CNT = 10;
	static WorkState_t last;
	
	if(IMU_Heavy_CleanCnt++ > 5)
	{
		IMU_Heavy_CleanCnt = 0;
		IMU_Heavy = 0;
	}
	
	if(last == log && workState == antiTheif)
	{
		RBP_PW_DOWN_CNT=10;
	}
	if(RBP_PW_DOWN_CNT>0)
	{
		if(--RBP_PW_DOWN_CNT == 0)
			RPW_OFF();
	}
	
	last = workState;
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

void RBP_RxCallback(void)
{
	static uint8_t rxCnt = 0;
	if(rbpRx == '\n')
	{
		rxCnt++;
	}
	else
	{
		rxCnt = 0;
	}
	if(rxCnt == 3)
		rbpACK = 1;
	HAL_UART_Receive_IT(&RBP_UART, &rbpRx, 1);
}
