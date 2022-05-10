
#include "key.h"

uint16_t GPIOA_Trg;
uint16_t GPIOA_Cont;
uint16_t GPIOB_Trg;
uint16_t GPIOB_Cont;
uint16_t GPIOC_Trg;
uint16_t GPIOC_Cont;
uint16_t GPIOD_Trg;
uint16_t GPIOD_Cont;
uint16_t GPIOE_Trg;
uint16_t GPIOE_Cont;

//low level trigger.  
//PRESS: GPIO.value=0, Trg = 1, Cnt = 1. 
//Which means: while key press, trg & Cnt = True

void RAB_GPIO_Key_Read(GPIO_TypeDef *GPIOx)
{
	uint16_t ReadData = (uint16_t)GPIOx->IDR ^ 0xFFFF;	  // 16	  KEY_DOWN WHEN ACTIVATE
//	uint16_t ReadData = (uint16_t)GPIOx->IDR ^ 0xFFFF;	// 16   KEY_UP   WHEN ACTIVATE
	if(GPIOx == GPIOA)
	{
		GPIOA_Trg = ReadData & (ReadData ^ GPIOA_Cont);      // 2
		GPIOA_Cont = ReadData;   
	}   
	else if(GPIOx == GPIOB)
	{
		GPIOB_Trg = ReadData & (ReadData ^ GPIOB_Cont);      // 2
		GPIOB_Cont = ReadData;   
	}     
	else if(GPIOx == GPIOC)
	{
		GPIOC_Trg = ReadData & (ReadData ^ GPIOC_Cont);      // 2
		GPIOC_Cont = ReadData;   
	}     	   
	else if(GPIOx == GPIOD)
	{
		GPIOD_Trg = ReadData & (ReadData ^ GPIOD_Cont);      // 2
		GPIOD_Cont = ReadData;   
	}     	   
	else if(GPIOx == GPIOE)
	{
		GPIOE_Trg = ReadData & (ReadData ^ GPIOE_Cont);      // 2
		GPIOE_Cont = ReadData;   
	}     	
}
