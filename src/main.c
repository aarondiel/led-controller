#include "bcm2835.h"

int main() {
	bcm2835	*chip = bcm2835_open();

	printf("base: 0x%x\n", chip->peripheral_base);
	printf("size: 0x%x\n", chip->peripheral_size);
	printf("address: %p\n", chip->peripherals);

	const unsigned int pin = 18;
	gpio_set_function(chip->gpio, pin, GPIO_FUNCTION_FUNC5);

	// divider 16
	gpclock_set_clock(chip->pwmclock, 16, 0);

	// set mode
	chip->pwm->control = (pwm_ctl_register) {
		.power0 = 1,
		.m_s0 = 1
	};

	// set range
	const unsigned int range = 1024;
	chip->pwm->range0 = range;

	unsigned long end = time(NULL) + 10;
	int direction = 1;
	int data = 1;
	while (time(NULL) < end) {
		if (data == 1)
			direction = 1;
		else if (data == range - 1)
			direction = -1;  // Switch to decreasing

		data += direction;
		chip->pwm->data0 = data;
		bcm2835_delay(1);
	}

	gpio_set_function(chip->gpio, pin, GPIO_FUNCTION_INPUT);

	bcm2835_close(chip);
}
