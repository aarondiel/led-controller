#include "bcm2835.h"

int main() {
	bcm2835	*chip = bcm2835_open();

	printf("base: 0x%x\n", chip->peripheral_base);
	printf("size: 0x%x\n", chip->peripheral_size);
	printf("address: %p\n", chip->peripherals);

	// gpclock_div_register *dataline = (gpclock_div_register *)(chip->peripherals + 0x40441);

	const unsigned int pin = 18;
	gpio_set_function(chip->gpio, pin, GPIO_FUNCTION_FUNC5);

	// pwm_set_clock -> divider 16
	gpclock_set_clock(chip->pwmclock, 16, 0);

	// pwm_set_mode(0, 1, 1) -> set channel 0 to markspace mode (1) and enable (1)
	chip->pwm->control.m_s0 = 1;
	chip->pwm->control.silence_bit0 = 1;
	chip->pwm->control.repeat_last_data0 = 1;
	chip->pwm->control.power0 = 1;

	// pwm_set_range
	const unsigned int range = 1024;
	chip->pwm->range0 = range;

	chip->pwm->data0 = 0xaaaaaaaa;

	unsigned long end = time(NULL) + 25;
	int direction = 1;
	int data = 1;
	while (time(NULL) < end) {
		if (data == 1)
            direction = 1;   // Switch to increasing
		else if (data == range - 1)
            direction = -1;  // Switch to decreasing

        data += direction;
		chip->pwm->data0 = data;
	}

	gpio_set_function(chip->gpio, pin, GPIO_FUNCTION_INPUT);

	bcm2835_close(chip);
}
