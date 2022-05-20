#pragma once

#include "main.h"
__packed struct calendar {
	uint16_t y;
	uint8_t m;
	uint8_t d;
};
__packed struct time {
	uint8_t h;
	uint8_t m;
	uint8_t s;
};

__packed struct locate {
	uint8_t ns;	//weidu
	uint8_t ew;
	float weidu;
	float jingdu;
	float speed;
};
typedef __packed struct gps_struct {
	__packed struct calendar cal;
	__packed struct time t;
	__packed struct locate loc;

	uint8_t isOK;
	uint8_t end[2];
	
} gps_t;

extern gps_t gps;
extern uint8_t GPS_Ready;
extern uint8_t GPS_size;

void GPS_Init(void);

void GPS_RxCallback(void);
void GPS_GetRawData(void);

static uint8_t GPS_CRC_isOK(void);
