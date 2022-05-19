#include "loop_it.h"
#include "loop.h"

#include "main.h"
#include "HardwareDefV1_0.h"
#include "imu_buff.h"
#include "gps.h"

#include "vofa.h"


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
}

//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//{
//	if( ( huart->ErrorCode & HAL_UART_ERROR_FE ) || ( huart->ErrorCode & HAL_UART_ERROR_NE ) || (huart->ErrorCode & HAL_UART_ERROR_ORE ) )
//	{

//		__HAL_UART_CLEAR_PEFLAG(huart);
//		DMA_Mem_No1 = 0;
//		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
//		HAL_UART_Receive_DMA(&huart1, buffer_11, 88);
//	}
//}
