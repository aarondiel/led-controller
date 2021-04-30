#include "gpio.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>

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

	for (int i = 0; i < chipinfo.lines; i++) {
		struct gpioline_info lineinfo;
		getLineInfo(fd, &lineinfo, i);

		printf(
			"\noffset: %02d\nname: %s\nconsumer: %s\nflags: %d\n",
			lineinfo.line_offset,
			lineinfo.name,
			lineinfo.consumer,
			lineinfo.flags
		);
	}

	struct gpiohandle_request req = {
		.flags = GPIOHANDLE_REQUEST_OUTPUT,
		.lines = 1,
		.lineoffsets = { 0, 1, 2, 3, 4, 5 },
		.default_values = { 0 },
		.consumer_label = "aaron's cool program"
	};

	for (int i = 0; i < 26; i++)
		req.lineoffsets[i] = i;

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
