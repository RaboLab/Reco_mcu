#include "loop_it.h"
#include "loop.h"

#include "main.h"
#include "HardwareDefV1_0.h"
#include "imu_buff.h"
#include "gps.h"

#include "vofa.h"

extern void RBP_RxCallback(void);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &IMU_UART)
	{
		IMU_RxCallback();
	}
	if(huart == &GPS_UART)
	{
		GPS_RxCallback();
	}
	if(huart == &RBP_UART)
	{
		RBP_RxCallback();
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if( ( huart->ErrorCode & HAL_UART_ERROR_FE ) || ( huart->ErrorCode & HAL_UART_ERROR_NE ) || (huart->ErrorCode & HAL_UART_ERROR_ORE ) )
	{
		__HAL_UART_CLEAR_PEFLAG(huart);
		if(huart == &GPS_UART)
		{
			GPS_Init();
		}
		else if(huart == &IMU_UART)
		{
			IMU_Init();
		}
	}
}
