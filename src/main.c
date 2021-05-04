#include "gpio.h"
#include "util.h"

int main() {
	gpiochip *chip = openGpioChip("/dev/gpiochip0");

	getLine(chip, 0, GPIOHANDLE_REQUEST_OUTPUT);

	clock_t endtime = time(0) + 10;
	gpioline *line = chip->lines[0];
	struct timespec clock, nextframe;

	while (time(0) < endtime) {
		clock_gettime(CLOCK_MONOTONIC, &clock);
		memcpy(&nextframe, &clock, sizeof(struct timespec));
		nextframe.tv_nsec += 33333333;

		// GRB color
		int payload = 0x000000;

		for (int j = 0; j < 200; j++) {
			for (int i = 0; i < 24; i++) {
				if ((payload >> 23) & 1) {
					// T1H 0.8µs
					setValue(line, 0);
					clock.tv_nsec += 800;
					clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &clock, NULL);

					// T1L 0.45µs
					setValue(line, 1);
					clock.tv_nsec += 450;
					clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &clock, NULL);
				} else {
					// T0H 0.4µs
					setValue(line, 1);
					clock.tv_nsec += 400;
					clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &clock, NULL);

					// T0L 0.85µs
					setValue(line, 0);
					clock.tv_nsec += 850;
					clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &clock, NULL);
				}

				payload <<= 1;
			}
		}

		// RES > 50µs
		setValue(line, 0);
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &nextframe, NULL);
	}

	closeGpioChip(chip);
	return 0;
}
