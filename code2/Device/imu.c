#include "imu.h"
#include "string.h"

#include "HardwareDefV1_0.h"

#define IMU_RX_BUFF_LEN 44

uint8_t* buffer_ptr;
uint8_t buffer_11[88];
uint8_t buffer_12[88];

volatile uint8_t DMA_Mem_No1;

jy62_t jy62;
__IO uint32_t jy62_2_rxCnt;


void IMU_Fetch_Init(void)
{
	IMU_Uart_with_DMA_Init();
}

static void IMU_Uart_with_DMA_Init(void)
{
	__HAL_UART_ENABLE_IT(&IMU_UART, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&IMU_UART, buffer_11, 88);
	
	
//	SET_BIT(IMU_UART.Instance->CR3, USART_CR3_DMAR);
//	__HAL_UART_ENABLE_IT(&IMU_UART, UART_IT_IDLE);
//	__HAL_DMA_DISABLE(IMU_UART.hdmarx);
//	HAL_Delay(1);
//	IMU_UART.hdmarx->Instance->PAR = (uint32_t) & (IMU_UART.Instance->DR);
//	IMU_UART.hdmarx->Instance->M0AR = (uint32_t)buffer_11;
//	IMU_UART.hdmarx->Instance->M1AR = (uint32_t)buffer_12;
//	IMU_UART.hdmarx->Instance->NDTR = IMU_RX_BUFF_LEN * 2;
//	
//	SET_BIT(IMU_UART.hdmarx->Instance->CR, DMA_SxCR_DBM);
//	__HAL_DMA_ENABLE(IMU_UART.hdmarx);
}

inline void set_acc(jy62_t * jy62, uint8_t * buffer, uint8_t index)
{
	memcpy(jy62->Data, &(buffer[index+1]), 6);
}
inline void set_gyro(jy62_t * jy62, uint8_t * buffer, uint8_t index)
{
	memcpy(&(jy62->Data[6]), &(buffer[index+1]), 8);
}

inline void set_angle(jy62_t * jy62, uint8_t * buffer, uint8_t index)
{
	memcpy(&(jy62->Data[14]), &(buffer[index+1]), 6);

}
inline void set_quad(jy62_t * jy62, uint8_t * buffer, uint8_t index)
{
	memcpy(&(jy62->Data[20]), &(buffer[index+1]), 8);
}

__IO uint8_t sumCheck;

void imu_fetch(jy62_t * jy62t, uint8_t* buffer)
{
	if( buffer[0] != 0x55 || buffer[1] != 0x51 )
		return;
	
	for(int x = 0; x < 4; ++x)
	{
		sumCheck = 0;
		for(int y = 0; y < 10; ++y)
			sumCheck += buffer[ y + x * 11 ];
		if (sumCheck == buffer[ x * 11 + 10 ])
		{
			if(buffer[ x * 11 + 1 ] == 0x51)
				set_acc(jy62t, buffer, x * 11 + 1);
			else if (buffer[ x * 11 + 1 ] == 0x52)
				set_gyro(jy62t, buffer, x * 11 + 1);
			else if (buffer[ x * 11 + 1 ] == 0x53)
				set_angle(jy62t, buffer, x * 11 + 1);
			else if (buffer[ x * 11 + 1 ] == 0x59)
				set_quad(jy62t, buffer, x * 11 + 1);
		}
	}
	
}
 
__IO uint8_t thisTimeRx = 0;

void IMU_set_Z_Axis(UART_HandleTypeDef * huart, jy62_t * jy62t)
{
	// 1. Unlock
	uint8_t Unlock[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
	uint8_t Data[5] = {0xFF, 0xAA, 0x01, 0x04, 0x00};
	uint8_t Save[5] = {0xFF, 0xAA, 0x00, 0x00, 0x00};
	
	HAL_UART_Transmit(huart, Unlock, 5, 0xFFFF);
	HAL_Delay(1000);
	
	// 2. Set
	HAL_UART_Transmit(huart, Data, 5, 0xFFFF);
	HAL_Delay(1000);
	// 3. Store
	
	HAL_UART_Transmit(huart, Save, 5, 0xFFFF);
	HAL_Delay(1000);
}

void IMU_set_OffsetEular(UART_HandleTypeDef * huart, jy62_t * jy62t)
{
	// 1. Unlock
	uint8_t Unlock[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
	uint8_t Data[5] = {0xFF, 0xAA, 0x01, 0x08, 0x00};
	uint8_t Save[5] = {0xFF, 0xAA, 0x00, 0x00, 0x00};
	
	HAL_UART_Transmit(huart, Unlock, 5, 0xFFFF);
	HAL_Delay(1000);
	
	// 2. Set
	HAL_UART_Transmit(huart, Data, 5, 0xFFFF);
	HAL_Delay(1000);
	// 3. Store
	
	HAL_UART_Transmit(huart, Save, 5, 0xFFFF);
	HAL_Delay(1000);
}
void IMU_fetch_IDLE_IRQHandle(UART_HandleTypeDef * huart)
{
	__HAL_UART_CLEAR_PEFLAG(huart);
	__HAL_DMA_DISABLE(huart->hdmarx);

	thisTimeRx = 88 - huart->hdmarx->Instance->NDTR;
	huart->hdmarx->Instance->NDTR = 88;
	
	if(huart == &huart1)
	{
		if(DMA_Mem_No1)
		{
			huart->hdmarx->Instance->M0AR = (uint32_t)buffer_11;
			__HAL_DMA_ENABLE(huart->hdmarx);
			if(thisTimeRx == IMU_RX_BUFF_LEN)
			{
				
				imu_fetch(&jy62, buffer_12);
			}
		}
		else
		{
			huart->hdmarx->Instance->M0AR = (uint32_t)buffer_12;
			__HAL_DMA_ENABLE(huart->hdmarx);
			if(thisTimeRx == IMU_RX_BUFF_LEN)
			{
				imu_fetch(&jy62, buffer_11);
			}
		}
		
		DMA_Mem_No1 = !DMA_Mem_No1;
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if( ( huart->ErrorCode & HAL_UART_ERROR_FE ) || ( huart->ErrorCode & HAL_UART_ERROR_NE ) || (huart->ErrorCode & HAL_UART_ERROR_ORE ) )
	{
	
		__HAL_UART_CLEAR_PEFLAG(huart);
		DMA_Mem_No1 = 0;
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
		HAL_UART_Receive_DMA(&huart1, buffer_11, 88);
	}
}
