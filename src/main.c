#include "gpio.h"
#include "pwm.h"

int main() {
	bcm2835	*chip = openChip();

	printf("base: %d\n", chip->peripheral_base);
	printf("size: %d\n", chip->peripheral_size);
	printf("address: %p\n", chip->peripherals);

	const unsigned int pin = 12;
	setFunction(chip->gpio, pin, GPIO_FUNCTION_FUNC0);

	pwm_enable_channel(chip->pwm, 0, 1);
	pwm_set_mode(chip->pwm, 0, PWM_MODE_SERIALISER);
	pwm_use_fifo(chip->pwm, 0, 1);
	pwm_enable_dma(chip->pwm, 1);

	closeChip(chip);
}
