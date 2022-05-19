#include "stdlib.h"
#include "string.h"
#include "imu_buff.h"

#include "vofa.h"
#include "usart.h"
#include "HardwareDefV1_0.h"

#define IMU_BUFF_SIZE 44


static uint8_t *Buffer;
uint8_t *IMU_Data;

static __IO uint8_t Rx;
static __IO uint8_t RxCnt;
static uint8_t RxState;
uint8_t IMU_Ready;
void IMU_Init()
{
	RxState = 0;
	Buffer = (uint8_t *)malloc(IMU_BUFF_SIZE);
	Buffer[0] = 0x55;
	Buffer[1] = 0x51;
	IMU_Data = (uint8_t *)malloc(IMU_BUFF_SIZE);
	
	HAL_UART_Receive_IT(&IMU_UART,(uint8_t *)&Rx,1);
}
void IMU_RxCallback()
{
	if(RxState == 0 && Rx == 0x55)
	{
		RxState = 1;
	}
	else if(RxState == 1 && Rx == 0x51)
	{
		RxState = 2;		//Frame Head, Ready to Rx
		RxCnt = 2;
	}
	else if(RxState == 2)
	{
		Buffer[RxCnt++] = Rx;
		if(RxCnt == 44)
		{
			vofa.printf("Last Bit:%#x\n", Rx);
			RxState = 0;		// Frame End
			memcpy(IMU_Data, Buffer, IMU_BUFF_SIZE);
			IMU_Ready = 1;
		}
	}
	HAL_UART_Receive_IT(&IMU_UART,(uint8_t *)&Rx,1);
}
