#include "gps.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "usart.h"
#include "vofa.h"
#include "HardwareDefV1_0.h"

static uint8_t GPS_Buffer[256];
static volatile uint8_t GPS_Data[256];
static uint8_t rx;
static uint8_t rxCnt;

gps_t gps;
uint8_t GPS_Ready;
uint8_t GPS_size = sizeof(gps_t);


void GPS_Init(void)
{
	gps.isOK = 0;
	gps.end[0] = '\n';
	gps.end[1] = '\n';
	HAL_UART_Receive_IT(&GPS_UART, &rx, 1);
}

void GPS_RxCallback(void)
{
	
	if(rx == '$')  // 0x24  Frame begin
	{
		rxCnt = 0;
		goto end;
	}
	else if(rx == '\n' || rxCnt >= 256) //end
	{
		rxCnt = 0;
		//process
		if(GPS_Ready == 0)
			memcpy((void *)GPS_Data, GPS_Buffer, 256);
		GPS_Ready = 1;	
		goto end;
	}
	
	GPS_Buffer[rxCnt++] = rx;	//Receiving

end:
	HAL_UART_Receive_IT(&GPS_UART, &rx, 1);
}

uint8_t len = 0;
int32_t temp_int;
uint32_t sizexx = sizeof(gps);  //debug use
uint8_t buff[12];
volatile uint8_t* ptr1;

uint32_t tick_cnt = 0;
void GPS_GetRawData(void)
{
	tick_cnt = HAL_GetTick();
//	uint8_t len = 0;
	ptr1 = GPS_Data;
	
	uint8_t strlens = strlen((const char *)ptr1) - 12;
	if(GPS_CRC_isOK() == 0) 
		return;
	for (uint8_t x = 0; strlens; x++)
	{
		for (len = 0; ptr1[len] != ','; len++) if (len >= strlens) break;

		strlens -= len;
		
		memset(buff, 0, 12);
		strncpy((char*)buff, (char*)ptr1, len);

		switch (x)
		{
		case 1: 
			temp_int = atoi((char*)buff);   //hhmmss
			gps.t.h = ((uint32_t)(temp_int / 10000) + 8) % 24;
			gps.t.m = (uint32_t)(temp_int / 100) % 100;
			gps.t.s = (uint32_t)(temp_int) % 100;
			break;   //time

		case 2: 
			if (buff[0] == 'A')
				gps.isOK = 1;
			else
				gps.isOK = 0;
			break;   //A / V

		case 3: 
			gps.loc.weidu = atof((char*)buff);
			break;	 //WEIDU
		case 4: 
			gps.loc.ns = buff[0];
			break;   //NS
		case 5: 
			gps.loc.jingdu = atof((char*)buff);
			break;   //JINGDU
		case 6:
			gps.loc.ew = buff[0];
			break;   //EW
		case 7:
			gps.loc.speed = atof((char*)buff);
			break;   //KNOTS
//			case 8: 
//				gps.loc.angle = atof((char*)buff);
//				break;   //ANGLE
		case 9: 
			temp_int = atoi((char*)buff);                     //ddmmyy
			gps.cal.d = (uint32_t)(temp_int / 10000);
			gps.cal.m = ((uint32_t)(temp_int / 100)) % 100;
			gps.cal.y = ((uint32_t)(temp_int)) % 100 + 2000;
		
			if(gps.t.h < 8) ++gps.cal.d;
			break;   //DATE
		default:
			break;
		}
		ptr1 = ptr1 + len + 1;
	}
	tick_cnt = HAL_GetTick() - tick_cnt;
}

uint8_t ans;
uint8_t crc;
uint8_t crc_n;
uint32_t error = 0;
static uint8_t GPS_CRC_isOK(void)
{
	for(crc_n = 60; GPS_Data[crc_n] != 0x2A; ++crc_n)
		if(crc_n > 100)
		{
			error += 1;
			gps.isOK = 0;
			return 0;
		}
	++crc_n;
	ans = GPS_Data[0];
	for(uint8_t cnt = 1; cnt < crc_n - 1; cnt++)
	{
		ans ^= GPS_Data[cnt];
	}
	
	crc = (uint8_t)((GPS_Data[crc_n] & 0x0F) << 4) + (uint8_t)(GPS_Data[crc_n + 1] & 0x0F); //67
	
	if(GPS_Data[crc_n + 1 ] >= 0x41) 
		crc += 9;
	if(GPS_Data[crc_n] >= 0x41) 
		crc = (uint8_t)(crc + 0x90);
	if(ans != crc)
	{
		error += 1;
		gps.isOK = 0;
		return 0;
	}
	return 1;
}
