#include "gpio.h"


static bool getChipInfo(int fd, struct gpiochip_info *info) {
	int rv = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, info);

	if (rv < 0)
		return 1;

	return 0;
}

static bool getLineInfo(int fd, struct gpioline_info *info, int offset) {
	info->line_offset = offset;

	int rv = ioctl(fd, GPIO_GET_LINEINFO_IOCTL, info);

	if (rv < 0)
		return 1;

	return 0;
}

void closeGpioChip(gpiochip *chip) {
	for (int i = 0; i < chip->linecount; i++) {
		if (chip->lines[i].fd == -1) {
			break;
		}

		close(chip->lines[i].fd);
	}

	close(chip->fd);
	free(chip->lines);
	free(chip);
}

gpiochip *openGpioChip(char *path) {
	int fd = open(path, O_RDWR);

	if (fd < 0) {
		printf("couldn't open device:\n%s\n", strerror(errno));
		goto error;
	}

	struct gpiochip_info info;
	if (getChipInfo(fd, &info)) {
		printf("couldn't get chip info:\n%s\n", strerror(errno));
		goto error;
	}

	gpiochip *chip = malloc(sizeof(gpiochip));
	chip->fd = fd;
	strcpy(chip->name, info.name);
	strcpy(chip->label, info.label);
	chip->lines = calloc(chip->linecount, sizeof(gpioline));
	chip->linecount = info.lines;

	for (int i = 0; i < chip->linecount; i++)
		chip->lines[i].fd = -1;

	return chip;

error:
	close(fd);
	return NULL;
}

// append gpioline to gpiochip
bool getLine(gpiochip *chip, unsigned int offset, unsigned long mode) {
	if (mode != GPIOHANDLE_REQUEST_OUTPUT && mode != GPIOHANDLE_REQUEST_INPUT) {
		errno = EINVAL;
		goto error;
	}

	struct gpioline_info info;
	if (getLineInfo(chip->fd, &info, offset))
		goto error;

	struct gpiohandle_request req = {
		.flags = mode,
		.lines = 1,
		.lineoffsets = { 0 },
		.default_values = { 0 },
		.consumer_label = "aaron's cool program"
	};
	req.lineoffsets[0] = offset;
	int rv = ioctl(chip->fd, GPIO_GET_LINEHANDLE_IOCTL, &req);

	if (rv < 0)
		goto error;

	for (int i = 0; i < chip->linecount; i++) {
		if (chip->lines[i].fd == -1) {
			chip->lines[i].fd = req.fd;
			chip->lines[i].offset = offset;
			strcpy(chip->lines[i].name, info.name);
			return 0;
		}

		// line has been requested before
		if (chip->lines[i].offset == offset) {
			close(chip->lines[i].fd);
			chip->lines[i].offset = offset;
			chip->lines[i].fd = req.fd;
			strcpy(chip->lines[i].name, info.name);
			return 0;
		}
	}
	return 1;

error:
	printf("couln't request line:\n%s\n", strerror(errno));
	return 1;
}

bool setValue(gpioline *line, bool value) {
	if (line->fd == -1) {
		errno =	EBADF;
		goto error;
	}

	struct gpiohandle_data data;

	data.values[line->offset] = value;

	int rv = ioctl(line->fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

	if (rv < 0)
		goto error;

	return 0;

error:
	printf("couldn't set value:\n%s\n", strerror(errno));
	return 1;
}
