#include <linux/gpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool getChipInfo(int fd) {
	struct gpiochip_info info;

	int rv = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &info);

	if (rv < 0) {
		printf("couldn't get gpio info");
		return 1;
	}

	printf("name: %s\n", info.name);
	printf("label: %s\n", info.label);
	printf("lines: %d\n", info.lines);

	return 0;
}

bool getLineInfo(int fd, int offset) {
	struct gpioline_info info;

	info.line_offset = offset;

	int rv = ioctl(fd, GPIO_GET_LINEINFO_IOCTL, &info);

	if (rv < 0) {
		printf("couldn't get line info");
		return 1;
	}

	printf("consumer: %s\n", info.consumer);
	printf("flags: %d\n", info.flags);
	printf("offset: %d\n", info.line_offset);
	printf("name: %s\n", info.name);

	return 0;
}

bool reqestLine(int fd) {
	struct gpiohandle_request req;

	req.flags |= GPIOHANDLE_REQUEST_OUTPUT;
	req.lines = 1;
	req.lineoffsets[0] = 16;
	req.default_values[0] = 0;

	strcpy(req.consumer_label, "aaron's cool program");

	int rv = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);

	if (rv < 0) {
		printf("couldn't request lines");
		return 1;
	}

	return 0;
}

bool setValue(int fd) {
	struct gpiohandle_data data;

	data.values[0] = 1;

	int rv = ioctl(fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

	if (rv < 0) {
		printf("couldn't set value");
		return 1;
	}

	return 0;
}

int main() {
	int fd = open("/dev/gpiochip0", O_RDWR);

	if (fd < 0) {
		printf("couldn't open file");
		return 0;
	}

	if (!(getChipInfo(fd) || getLineInfo(fd, 16) || reqestLine(fd) || setValue(fd)))
		getchar();

	close(fd);

	return 0;
}
