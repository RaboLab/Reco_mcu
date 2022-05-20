#pragma once

#include "main.h"
typedef struct PACKAGE 
{
	uint8_t load[50];
	uint8_t lens;
	uint8_t isLoaded;
}pack_t;

void pack_load(void * ptr, uint8_t size);
pack_t * pack_open(void);

static int8_t find_empty_pack(void);
static int8_t find_pack(void);
