#pragma once

#include <linux/gpio.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdbool.h>

struct gpiochip {
	char name[32];
};

bool getChipInfo(int fd, struct gpiochip_info *info);
bool getLineInfo(int fd, struct gpioline_info *info, int offset);
bool requestLine(int fd, struct gpiohandle_request *req);
bool setValue(int fd, unsigned int pin, bool value);
