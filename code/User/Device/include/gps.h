#ifndef __GPS_H_
#define __GPS_H_

#include "main.h"
struct calendar {
	uint16_t y;
	uint8_t m;
	uint8_t d;
};
struct time {
	uint8_t h;
	uint8_t m;
	uint8_t s;
};

struct locate {
	uint8_t ns;	//weidu
	uint8_t ew;
	double weidu;
	double jingdu;
	double speed;  //knots
	double angle;
};
typedef struct gps_struct {
	struct calendar cal;
	struct time t;
	struct locate loc;

	uint8_t isOK;
} gps_t;

extern gps_t gps;

void GPS_Init(void);

void GPS_RxCallback(void);
void GPS_GetRawData(void);
#endif
