#include "gpioo.h"
#include "HardwareDefV1_0.h"

inline void LED_ON(void)
{
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
}
inline void LED_OFF(void)
{
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
}
inline void LED_TOG(void)
{
	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
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

inline void RBP_CALLONLINE(void)
{
	HAL_GPIO_WritePin(SORI_1_GPIO_Port,SORI_1_Pin, GPIO_PIN_SET);		//GPIO SET
}
inline void RBP_CALLOFFLINE(void)
{
	HAL_GPIO_WritePin(SORI_1_GPIO_Port,SORI_1_Pin, GPIO_PIN_RESET);		//Buzzer OFF
}

inline void RBP_SAVE_SET(void)
{
	HAL_GPIO_WritePin(SORI_4_GPIO_Port,SORI_4_Pin, GPIO_PIN_SET);		//Buzzer OFF
}
inline void RBP_SAVE_RESET(void)
{
	HAL_GPIO_WritePin(SORI_4_GPIO_Port,SORI_4_Pin, GPIO_PIN_RESET);		//Buzzer OFF
}

inline void RBP_LOOK_SET(void)
{
	HAL_GPIO_WritePin(SORI_5_GPIO_Port,SORI_5_Pin, GPIO_PIN_SET);		//Buzzer OFF
}
inline void RBP_LOOK_RESET(void)
{
	HAL_GPIO_WritePin(SORI_5_GPIO_Port,SORI_5_Pin, GPIO_PIN_RESET);		//Buzzer OFF
}
