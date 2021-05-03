#include "gpio.h"
#include "util.h"

int main() {
	gpiochip *chip = openGpioChip("/dev/gpiochip0");

	getLine(chip, 0, GPIOHANDLE_REQUEST_OUTPUT);

	for (int i = 0; i < 60; i++) {
		printf("on\n");
		setValue(&chip->lines[0], 0);
		delay(1);
		printf("off\n");
		setValue(&chip->lines[0], 1);
		delay(1);
	}

	closeGpioChip(chip);
	return 0;
}
