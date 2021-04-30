#include "gpio.h"

struct gpiochip chip;

bool getChipInfo(int fd, struct gpiochip_info *info) {
	int rv = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, info);

	if (rv < 0) {
		printf("couldn't get gpio info");
		return 1;
	}

	return 0;
}

bool getLineInfo(int fd, struct gpioline_info *info, int offset) {
	info->line_offset = offset;

	int rv = ioctl(fd, GPIO_GET_LINEINFO_IOCTL, info);

	if (rv < 0) {
		printf("couldn't get line info");
		return 1;
	}

	return 0;
}

bool requestLine(int fd, struct gpiohandle_request *req) {
	int rv = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, req);

	if (rv < 0) {
		printf("couldn't request lines\n");
		return 1;
	}

	return 0;
}

// upper bound for pin is hard-coded !!!
bool setValue(int fd, unsigned int pin, bool value) {
	if (pin > 1) {
		printf("pin out of bounds\n");
		return 1;
	}

	struct gpiohandle_data data;

	data.values[pin] = value;

	int rv = ioctl(fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

	if (rv < 0) {
		printf("couldn't set value\n");
		return 1;
	}

	return 0;
}
