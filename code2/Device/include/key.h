#ifndef __KEY_H_
#define __KEY_H_

#include "main.h"

extern uint16_t GPIOA_Trg;
extern uint16_t GPIOA_Cont;

extern uint16_t GPIOB_Trg;
extern uint16_t GPIOB_Cont;

extern uint16_t GPIOC_Trg;
extern uint16_t GPIOC_Cont;

extern uint16_t GPIOD_Trg;
extern uint16_t GPIOD_Cont;

extern uint16_t GPIOE_Trg;
extern uint16_t GPIOE_Cont;
void RAB_GPIO_Key_Read(GPIO_TypeDef *GPIOx);


#endif
