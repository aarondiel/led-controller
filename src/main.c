#include "bcm2835.h"
#include "util.h"

int main() {
	bcm2835	*chip = bcm2835_open();

	printf("base: 0x%x\n", chip->peripheral_base);
	printf("size: 0x%x\n", chip->peripheral_size);
	printf("address: %p\n", chip->peripherals);

	const unsigned int pin = 18;
	gpio_set_function(chip->gpio, pin, GPIO_FUNCTION_FUNC5);

	// clock devider is the maximum clock frequency
	// divided by the desired frequency
	// (multiplied by 3 because each bit corresponds to 3 signals)
	gpclock_set_clock(chip->pwmclock, get_clock_divider(), 0);

	// set mode
	chip->pwm->control = (pwm_ctl_register) {
		.power0 = 1,
		.power1 = 0,
		.use_fifo0 = 1,
		.use_fifo1 = 0,
		.m_s0 = 0,
		.m_s1 = 0,
		.clear_fifo0 = 1,
		.repeat_last_data0 = 0,
		.repeat_last_data1 = 0,
		.silence_bit0 = 0,
		.silence_bit1 = 0,
		.mode0 = 1,
		.mode1 = 0
	};

	chip->pwm->range0 = 32;

	const int num_leds = 50;

	u_int32_t size;
	u_int32_t *buffer = write_buffer(num_leds, &size);
	const unsigned int indecies = size / 4;

	const unsigned int end = time(NULL) + 15;
	while(time(NULL) < end) {
		for (int i = 0; i < indecies; i++) {
			while (chip->pwm->status.fifo_full0);
			// printf("byte 0x%03d:\t0x%08x\n", 4 * i, buffer[i]);
			chip->pwm->fifo0 = buffer[i];
		}
	}

	free(buffer);

	gpio_set_function(chip->gpio, pin, GPIO_FUNCTION_INPUT);

	bcm2835_close(chip);
}
