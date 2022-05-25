#pragma once 
#include "main.h"
extern uint8_t gsmClear;
void GSM_Init(void);

void GSM_RxCallback(void);

void GSM_RxClear(void);
void GSM_Tx(void);
