#include "gpio.h"

int main() {
	bcm2835	*chip = openChip();

	printf("base: %d\n", chip->peripheral_base);
	printf("size: %d\n", chip->peripheral_size);
	printf("address: %p\n", chip->peripherals);

	unsigned int pin = 5;
	setFunction(chip->gpio, pin, GPIO_FUNCTION_OUTPUT);

	for (int i = 0; i < 60; i++) {
		writeLine(chip->gpio, pin, i % 2);
		sleep(1);
	}

	closeChip(chip);
}
