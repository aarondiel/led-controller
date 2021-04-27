#include <linux/gpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool getChipInfo(int fd) {
	struct gpiochip_info info;
	memset(&info, 0, sizeof(struct gpiochip_info));

	int rv = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &info);

	if (rv < 0) {
		printf("couldnt get gpio info");
		return -1;
	}

	printf("name: %s\n", info.name);
	printf("label: %s\n", info.label);
	printf("lines: %d\n", info.lines);

	return 0;
}

int main() {
	int fd = open("/dev/gpiochip0", O_RDWR);

	if (fd < 0) {
		printf("couldn't open file");
		return -1;
	}

	getChipInfo(fd);

	close(fd);
}
