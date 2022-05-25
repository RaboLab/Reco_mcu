#include "HardwareDefV1_0.h"
#include "gsm.h"
#include "main.h"
#include "usart.h"
#include "string.h"
static uint8_t gsmRxBuff[200];
static uint8_t gsmRxCnt;
static uint8_t gsmTxBuff[200];
static uint8_t gsmTxCnt;

uint8_t gsmClear;
static uint8_t gsmRx;

void GSM_Init(void)
{
	HAL_UART_Receive_IT(&GSM_UART, &gsmRx, 1);
	HAL_UART_Transmit(&GSM_UART, (uint8_t *)"AT\r\n", sizeof("AT\r\n"),0xFF);
	HAL_Delay(2000);
	HAL_UART_Transmit(&GSM_UART, (uint8_t *)"AT\r\n", sizeof("AT\r\n"),0xFF);
}

void GSM_RxCallback(void)
{
	gsmRxBuff[gsmRxCnt++] = gsmRx;
	HAL_UART_Receive_IT(&GSM_UART, &gsmRx, 1);
}
void GSM_RxClear(void)
{
	gsmRxCnt = 0;
	memset(gsmRxBuff, 0, 70);
	HAL_UART_Receive_IT(&GSM_UART, &gsmRx, 1);
}
void GSM_Tx(void)
{
	HAL_UART_Transmit(&GSM_UART, gsmTxBuff, gsmTxCnt,0xFF);
}
