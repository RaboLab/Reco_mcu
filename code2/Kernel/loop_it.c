#include "loop_it.h"
#include "loop.h"

#include "main.h"
#include "imu.h"

#include "vofa.h"

extern uint8_t bufxx;

//void SysTick_Handler(void)
//{
//		HAL_IncTick();  
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &IMU_UART)
	{
		HAL_UART_Transmit(&huart2,&bufxx,1,0xff);
		HAL_UART_Receive_IT(&huart1,&bufxx,1);
	}
}
