#include "loop.h"
#include "loop_it.h"
#include "adc.h"

#include "imu.h"
#include "imu_buff.h"
#include "gps.h"
#include "ring.h"
#include "gsm.h"

#include "key.h"
#include "gpioo.h"

#include "vofa.h"

uint16_t RAB_Clk = 0;
uint8_t time_1ms = 0;		//1K	Hz
uint8_t time_20ms = 0;	//50	Hz
uint8_t time_50ms = 0;	//20	Hz
uint8_t time_100ms = 0;	//10	Hz
uint8_t time_1s = 0;		//1		Hz

typedef enum WrokState_enum
{
	zero,
	
  lookVideo,
	antiTheif,
	log,
	power_down_pend,
	stopPy,
}WorkState_t;

WorkState_t workState = zero;
uint8_t buz_time;
uint8_t IMU_Heavy;
uint8_t isSave;
uint8_t isLook;
uint8_t rbpOnline;
uint8_t rbpRx;
uint8_t rbpSaveACK; 
uint8_t rbpLookACK;  
uint8_t rbpOFFACK;

uint8_t GSM_isTx;
uint8_t GSM_isClear;

uint8_t adc_mode;
uint16_t adc_value;
__IO float votage;

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
	
	workState = zero;
	IMU_set_OffsetEular(&GPS_UART);
	
	adc_mode = 1;
	if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0)
	{
		adc_mode = 0;
		Buz_ON();
	}

	while (HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0);		//keep press
	Buz_OFF();
	HAL_Delay(100);
	
	
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
		if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0)
		{
			break;
		}
	}
	
	Buz_ON();
	HAL_Delay(100);  //self check pass
	Buz_OFF();
	HAL_Delay(500);  //
	
	//GSM_Init();
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
	
	if(GSM_isTx)
	{
		GSM_Tx();
		GSM_isTx = 0;
		
	}
	if(GSM_isClear)
	{
		GSM_RxClear();
		GSM_isClear = 0;
	}
}

void Task_20ms()		//KEY AND STATUS function
{
	// ******************  KEY FUNCTION  ******************//
	// ******************  KEY FUNCTION  ******************//
	static uint32_t LONG_PRESS = 0;
	static uint32_t PRESS_CNT = 0;
	RAB_GPIO_Read(KEY_GPIO_Port);
	if(RAB_GPIO_Trg(KEY_GPIO_Port) & KEY_Pin) 		//Key Trigerd, Start to count time while press
	{
		PRESS_CNT = 1;
	}
	if((RAB_GPIO_Cnt(KEY_GPIO_Port) & KEY_Pin)) 		//while Key Pressed
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
			
			if(workState == log)		//different function
			{
				if(PRESS_CNT == 1)
					isSave = 2;		//Save two file
				else if(PRESS_CNT == 2)
					workState = lookVideo;
				else if(PRESS_CNT == 3)	//power_down
					workState = power_down_pend;
			}
			else if(workState == antiTheif)		//original state
			{
				if(PRESS_CNT == 3)
				{
					workState = lookVideo;		// long long press -> video
				}
				else
				{
					workState = log;		// 1 2 -> log
				}
			}
			else if(workState == stopPy)      //original state
			{
					workState = lookVideo;
			}
			else if(workState == lookVideo)       //original state
			{
				if(PRESS_CNT == 3)
					workState = power_down_pend;		//Save two file
				else if(PRESS_CNT == 1)
					workState = log;
				else
					workState = stopPy;
			}
			PRESS_CNT = 0;
			LONG_PRESS = 0;
		}
	}
	// ******************  ADC FUNCTION  ******************//
	// ******************  ADC FUNCTION  ******************//
	if(adc_mode)
	{
		if(votage > 12.0)  //power_up
		{
			if(workState == antiTheif)       //original state
			{
				workState = log;
			}
		}
		else
		{
			if(workState == log) 
			{
				workState = power_down_pend;
			}
		}
	}
	
	// ******************  SAVE FUNCTION  ******************//
	// ******************  SAVE FUNCTION  ******************//
	if(isSave)
	{
		RBP_SAVE_SET();
		if(rbpSaveACK) 
		{
			isSave --;
		}
		rbpSaveACK = 0;
	}
	else
		RBP_SAVE_RESET();
	
}

uint8_t online;
uint8_t look;
uint8_t save;
void Task_50ms()		//state change
{
	static WorkState_t last;
	if(last != workState)
	{
		buz_time = 3;		// 300ms
		rbpSaveACK = 0; rbpLookACK = 0; rbpOFFACK = 0;  // clean all signals
		isSave = 0; RBP_SAVE_RESET();
		
		if(workState == antiTheif)			//current workstate
		{
			RPW_OFF();
		}
		else if(workState == log)				//current workstate
		{
			RPW_ON();
			RBP_CALLONLINE();
			RBP_LOOK_RESET();
		}
		else if(workState == lookVideo)	//current workstate
		{
			RPW_ON();
			RBP_CALLONLINE();
			RBP_LOOK_SET();
		}else if(workState == power_down_pend)
		{
			RBP_CALLOFFLINE();
			RBP_LOOK_RESET();  //this value doesn't matter
		}
		else if(workState == stopPy)      //current workstate
		{
			RBP_CALLOFFLINE();
			RBP_LOOK_SET();
		}
		
	}
	last = workState;
	
	online = HAL_GPIO_ReadPin(SORI_1_GPIO_Port,SORI_1_Pin);
	save = HAL_GPIO_ReadPin(SORI_4_GPIO_Port,SORI_4_Pin);
	look = HAL_GPIO_ReadPin(SORI_5_GPIO_Port,SORI_5_Pin);
}
void Task_100ms()			//IO CONTROL
{
	static uint8_t last_IMU_Heavy;
	if(IMU_Heavy)
	{
		LED_ON();
		buz_time = 10;
		if(last_IMU_Heavy == 0)
			isSave = 1;
	}
	else
	{
		LED_OFF();
	}
	last_IMU_Heavy = IMU_Heavy;
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
	
}
void Task_1s()
{
	static uint8_t IMU_Heavy_CleanCnt = 0;
	static uint8_t RBP_PW_DOWN_CNT = 13;
	static uint8_t auto_power_off = 60;
	if(IMU_Heavy_CleanCnt++ > 5)		//anti_theif 5 seconds count down cleaner
	{
		IMU_Heavy_CleanCnt = 0;
		IMU_Heavy = 0;
	}

	if(rbpOFFACK == 1 && workState == power_down_pend)
	{
		if(RBP_PW_DOWN_CNT == 0)
		{
			workState = antiTheif;
			
			RBP_PW_DOWN_CNT = 13;
			rbpOFFACK = 0;
		}
		else
		{
			RBP_PW_DOWN_CNT--;
		}
	}
	
	if( adc_mode && (votage < 12.0) && workState == lookVideo)
	{
		if(auto_power_off == 0)
		{
			workState = power_down_pend;
			auto_power_off = 60;
		}
		else
		{
			auto_power_off--;
		}
	}
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,0xFFFF);
	adc_value = HAL_ADC_GetValue(&hadc1);
	votage = adc_value  * 3.3 * 5 / 4096.0 ;

}

void TaskCheck(void)
{
	if(time_1ms)		{time_1ms--;		Task_1ms();		}
	if(time_20ms)		{time_20ms--;		Task_20ms();	}
	if(time_50ms)		{time_50ms--;		Task_50ms();	}
	if(time_100ms)	{time_100ms--;	Task_100ms();	}
	if(time_1s)			{time_1s--;			Task_1s();		}
}

void RAB_Clock(void)
{
	RAB_Clk++;
	time_1ms = 1;
	if(!(RAB_Clk % 20))		{time_20ms++;			}
	if(!(RAB_Clk % 50))		{time_50ms++;			}
	if(!(RAB_Clk % 100))	{time_100ms++;		}
	if(!(RAB_Clk % 1000))	{time_1s++;			RAB_Clk = 0;	}
}

void RBP_RxCallback(void)
{
	static uint8_t rxOFFCnt = 0;
	static uint8_t rxSaveCnt = 0;
	static uint8_t rxLookCnt = 0;
	
	if(rbpRx == '\n') rxLookCnt++;
	else 							rxLookCnt = 0;
	if(rxLookCnt == 3) {rbpLookACK = 1; rxLookCnt = 0; buz_time = 1;}
	
	if(rbpRx == '\t') rxSaveCnt++;
	else 							rxSaveCnt = 0;
	if(rxSaveCnt == 3) {rbpSaveACK = 1; rxSaveCnt = 0; buz_time = 1;}
	
	if(rbpRx == '\r') rxOFFCnt++;
	else 							rxOFFCnt = 0; 
	if(rxOFFCnt == 3) {rbpOFFACK = 1; rxOFFCnt = 0; buz_time = 1;}
	
	HAL_UART_Receive_IT(&RBP_UART, &rbpRx, 1);
}
