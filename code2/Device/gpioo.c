#include "gpioo.h"

inline void LED_ON(void)
{
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
}
inline void LED_OFF(void)
{
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
}

inline void RPW_ON(void)
{
	HAL_GPIO_WritePin(RPW_GPIO_Port,RPW_Pin,GPIO_PIN_SET);
}
inline void RPW_OFF(void)
{
	HAL_GPIO_WritePin(RPW_GPIO_Port, RPW_Pin, GPIO_PIN_RESET);			//PWR OFF
}

inline void Buz_ON(void)
{
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
}
inline void Buz_OFF(void)
{
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_RESET);		//Buzzer OFF
}
