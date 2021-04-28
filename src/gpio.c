#include <linux/gpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void delay(unsigned int sec) {
	clock_t endtime = time(0) + sec;

	while (time(0) < endtime) {
		;
	}
}

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

int main() {
	int fd = open("/dev/gpiochip0", O_RDWR);

	if (fd < 0) {
		printf("couldn't open file\n");
		return 0;
	}

	struct gpiochip_info chipinfo;
	getChipInfo(fd, &chipinfo);

	printf(
		"name: %s\nlabel: %s\nlines: %d\n",
		chipinfo.name, chipinfo.label, chipinfo.lines
	);

	// for (int i = 0; i < chipinfo.lines; i++) {
	//	struct gpioline_info lineinfo;
	//	getLineInfo(fd, &lineinfo, i);
	//
	//	printf(
	//		"\noffset: %02d\nname: %s\nconsumer: %s\nflags: %d\n",
	//		lineinfo.line_offset,
	//		lineinfo.name,
	//		lineinfo.consumer,
	//		lineinfo.flags
	//	);
	// }

	struct gpiohandle_request req = {
		.flags = GPIOHANDLE_REQUEST_OUTPUT,
		.lines = 1,
		.lineoffsets = { 16 },
		.default_values = { 0 },
		.consumer_label = "aaron's cool program"
	};

	requestLine(fd, &req);

	for (int i = 0; i < 10; i++) {
		setValue(req.fd, 0, 1);
		printf("on\n");
		delay(1);

		setValue(req.fd, 0, 0);
		printf("off\n");
		delay(1);
	}

	close(fd);
	close(req.fd);

	return 0;
}
