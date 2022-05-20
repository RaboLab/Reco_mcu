#include "ring.h"
#include "main.h"
#include "string.h"

pack_t pack[10];

void pack_load(void * ptr, uint8_t size)
{
	int8_t pack_n = find_empty_pack();
	if(pack_n < 0)
		return;
	memcpy((void *)pack[pack_n].load, ptr, ( size > 50 ? 50 : size));
	pack[pack_n].isLoaded = 1;
	pack[pack_n].lens = ( size > 50 ? 50 : size);
}

pack_t * pack_open()
{
	int8_t pack_n = find_pack();
	if(pack_n == -1)
		return (pack_t *)NULL;
	pack[pack_n].isLoaded = 0;
	return &pack[pack_n];
}

static int8_t find_empty_pack()
{
	for(uint8_t x = 0; x < 11; x++)
	{
		if(pack[x].isLoaded == 0)
			return x;
	}
	return -1;
}

static int8_t find_pack()
{
	for(uint8_t x = 0; x < 11; x++)
	{
		if(pack[x].isLoaded == 1)
			return x;
	}
	return -1;
}
