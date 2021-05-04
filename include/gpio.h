#pragma once

#include <linux/gpio.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "util.h"

typedef struct {
	int fd;
	unsigned int offset;
	char name[32];
	bool value;
} gpioline;

typedef struct {
	int fd;
	char name[32];
	char label[32];
	gpioline **lines;
	unsigned int linecount;
} gpiochip;

gpiochip *openGpioChip(char *path);
bool getLine(gpiochip *chip, unsigned int offset, unsigned long mode);
void closeGpioChip(gpiochip *chip);
bool setValue(gpioline *line, bool value);
