#include "gps.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"


#include "usart.h"
#include "vofa.h"
#include "HardwareDefV1_0.h"

static uint8_t GPS_Buffer[70];

static uint8_t rx;
static uint8_t rxCnt;

gps_t gps;

void GPS_Init(void)
{
	HAL_UART_Receive_IT(&GPS_UART, &rx, 1);
}

void GPS_RxCallback(void)
{
	if(rx == '$')  // 0x24  begin
	{
		rxCnt = 0;
		goto end;
	}
	else if(rx == '\n') // 0x0A  end
	{
		//process
		if(GPS_Buffer[2] == 'R') GPS_GetRawData();
		goto end;
	}
	GPS_Buffer[rxCnt++] = rx;
	
end:
	HAL_UART_Receive_IT(&GPS_UART, &rx, 1);
}
uint8_t len = 0;
uint32_t temp_int;
uint32_t sizexx = sizeof(gps);
void GPS_GetRawData(void)
{
	uint8_t buff[12];
	uint8_t* ptr1;
//	uint8_t len = 0;
	
	ptr1 = GPS_Buffer;
	uint8_t strlens = strlen((const char *)ptr1) - 12;
	

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
			case 8: 
				gps.loc.angle = atof((char*)buff);
				break;   //ANGLE
			case 9: 
				temp_int = atoi((char*)buff);   //ddmmyy
				gps.cal.d = (uint32_t)(temp_int / 10000);
				gps.cal.m = ((uint32_t)(temp_int / 100)) % 100;
				gps.cal.y = ((uint32_t)(temp_int)) % 100 + 2000;
			
				if(gps.t.h < 8) ++gps.cal.d;
				break;   //DATE
			}

			ptr1 = ptr1 + len + 1;
		}
}
