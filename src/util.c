#include "util.h"

const unsigned int signals_per_bit = 3;
const unsigned int bits_per_byte = 8;
const unsigned int bytes_per_led = 3;
const unsigned int min_reset_time_us = 55;
const unsigned int us_per_s = 1000000;
const unsigned int osc_freq = 19200000;

const unsigned int frequency = 800000;

u_int32_t *write_buffer(unsigned int leds, unsigned int *size) {
	unsigned int buffer_size = (
		sizeof(color) * leds * signals_per_bit + // leds
		min_reset_time_us * frequency * signals_per_bit / (us_per_s * bits_per_byte) + // reset
		8 // random addition to reset for experimental purposes
	) & ~0b11; // round down to nearest multiple of 4, so that the buffer can be treated as a array of unsgigned ints

	// storing 24 bit ints inside buffer, not actual colors
	color *buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	color testcolor = {
		.red = 0x00,
		.green = 0x00,
		.blue = 0x00
	};

	for (int led = 0; led < leds; led++) {
		for (int bit = 0; bit < 24; bit++) {
			if (testcolor.value >> (23 - bit) & 1)
				buffer[3 * led + bit / 8].value |= 0b110 << (3 * ((23 - bit) % 8));
			else
				buffer[3 * led + bit / 8].value |= 0b100 << (3 * ((23 - bit) % 8));
		}
	}

	if (size != NULL)
		*size = buffer_size;

	return (u_int32_t *)buffer;
}

u_int32_t get_clock_divider() {
	return osc_freq / (3 * frequency);
}
