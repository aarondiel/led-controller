#pragma once

#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#pragma pack(1)
typedef union {
	struct {
		unsigned char green: 8;
		unsigned char red: 8;
		unsigned char blue: 8;
	};

	struct {
		unsigned int value: 24;
	};
} color;
#pragma pack()

u_int32_t *write_buffer(unsigned int leds, unsigned int *size);
u_int32_t get_clock_divider();
