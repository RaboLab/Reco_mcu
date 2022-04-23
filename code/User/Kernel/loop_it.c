#include "loop_it.h"
#include "loop.h"

#include "HardwareDefV1_0.h"
#include "gps.h"


#include "usart.h"


//uint16_t system_new_clock = 0;
//uint8_t system_new_clock_cnt = CONTROL_FREQ_HZ / 1000;

void SysTick_Handler(void)
{
		HAL_IncTick();  //��δ��ϵͳ�ӹܣ�Systick 1msһ��
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &GPS_UART)
	{
		GPS_RxCallback();
	}
}
