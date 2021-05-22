#include "gpio.h"

bcm2835 *openChip() {
	bcm2835 *chip = malloc(sizeof(bcm2835));
	FILE *fp = fopen("/proc/device-tree/soc/ranges", "rb");

	if (fp == NULL)
		goto error;

	unsigned char buf[4];

	fseek(fp, 4, SEEK_SET);
	fread(buf, sizeof(unsigned char), 4, fp);
	chip->peripheral_base = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3];

	fseek(fp, 8, SEEK_SET);
	fread(buf, sizeof(unsigned char), 4, fp);
	chip->peripheral_size = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3];

	fclose(fp);

	int fd = open("/dev/mem", O_RDWR);

	if (fd < 0)
		goto error;

	chip->peripherals = mmap(
		NULL,
		chip->peripheral_size,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		fd,
		chip->peripheral_base
	);
	close(fd);

	if (chip->peripherals == MAP_FAILED)
		goto error;

	chip->gpio = (gpio_peripherals *)(chip->peripherals + 0x80000);
	chip->pwm = (pwm_peripherals *)(chip->peripherals + 0x83000);

	return chip;

error:
	free(chip);
	return NULL;
}

void closeChip(bcm2835 *chip) {
	munmap(chip->peripherals, chip->peripheral_size);
	free(chip);
}

bool setFunction(gpio_peripherals *gpio, unsigned char pin, unsigned char function) {
	if (pin > 53 || function > 7)
		return 1;

	unsigned int bitshift = (pin % 10) * 3;
	unsigned int bitmask = ~(0b111 << bitshift);
	unsigned int payload = function << bitshift;

	gpio->function_select[pin / 10] = (gpio->function_select[pin / 10] & bitmask) | payload;

	return 0;
}

static bool clearLine(gpio_peripherals *gpio, unsigned char pin) {
	if (pin > 53)
		return 1;

	gpio->clear[pin / 32] |= 1 << (pin % 32);

	return 0;
}

bool writeLine(gpio_peripherals *gpio, unsigned char pin, bool state) {
	if (pin > 53)
		return 1;

	if (!state)
		return clearLine(gpio, pin);

	gpio->set[pin / 32] |= 1 << (pin % 32);

	return 0;
}

bool readLine(gpio_peripherals *gpio, unsigned char pin) {
	if (pin > 53)
		return 1;

	return gpio->level[pin / 32] & (1 << (pin % 32));
}
