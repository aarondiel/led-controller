#include "bcm2835.h"

void bcm2835_delay(u_int64_t millis) {
	struct timespec sleep_timer = {
		.tv_sec = millis / 1000,
		.tv_nsec = (millis % 1000) * 1000000
	};

	nanosleep(&sleep_timer, NULL);
}

bool gpio_set_function(gpio_peripherals *gpio, u_int8_t pin, u_int8_t function) {
	if (pin > 53 || function > 7)
		return 1;

	u_int32_t bitshift = (pin % 10) * 3;
	u_int32_t bitmask = ~(0b111 << bitshift);
	u_int32_t payload = function << bitshift;

	gpio->function_select[pin / 10] = (gpio->function_select[pin / 10] & bitmask) | payload;

	return 0;
}

static bool gpio_clear_line(gpio_peripherals *gpio, u_int8_t pin) {
	if (pin > 53)
		return 1;

	gpio->clear[pin / 32] |= 1 << (pin % 32);

	return 0;
}

bool gpio_write_line(gpio_peripherals *gpio, u_int8_t pin, bool state) {
	if (pin > 53)
		return 1;

	if (!state)
		return gpio_clear_line(gpio, pin);

	gpio->set[pin / 32] |= 1 << (pin % 32);

	return 0;
}

bool gpio_read_line(gpio_peripherals *gpio, u_int8_t pin) {
	if (pin > 53)
		return 1;

	return gpio->level[pin / 32] & (1 << (pin % 32));
}

bool gpclock_set_clock(gpclock_peripherals *clock, u_int16_t integer, u_int16_t floating) {
	if (integer > 0xfff || floating > 0xfff)
		return 1;

	clock->control = (gpclock_control_register) {
		.password = gpclock_password,
		.source = GPCLOCK_SOURCE_OSCILLATOR
	};

	// give the clock time to handle the request
	bcm2835_delay(200);

	while(clock->control.busy)
		bcm2835_delay(1);

	clock->div = (gpclock_div_register) {
		.password = gpclock_password,
		.integer_part = integer,
		.floating_part = floating
	};

	clock->control = (gpclock_control_register) {
		.password = gpclock_password,
		.source = GPCLOCK_SOURCE_OSCILLATOR,
		.enable = 1
	};

	return 0;
}

void pwm_enable_channel(pwm_peripherals *pwm, bool channel, bool en) {
	if (channel == 0)
		pwm->control.power0 = en;

	if (channel == 1)
		pwm->control.power1 = en;
}

void pwm_set_mode(pwm_peripherals *pwm, bool channel, bool mode) {
	if (channel == 0)
		pwm->control.mode0 = mode;

	if (channel == 1)
		pwm->control.mode1 = mode;
}

void pwm_use_fifo(pwm_peripherals *pwm, bool channel, bool en) {
	if (channel == 0)
		pwm->control.use_fifo0 = en;

	if (channel == 1)
		pwm->control.use_fifo1 = en;
}

void pwm_enable_dma(pwm_peripherals *pwm, bool en) {
	pwm->dma.enable = en;
}

bcm2835 *bcm2835_open() {
	bcm2835 *chip = malloc(sizeof(bcm2835));
	FILE *fp = fopen("/proc/device-tree/soc/ranges", "rb");

	if (fp == NULL)
		goto error;

	u_int8_t buf[4];

	fseek(fp, 4, SEEK_SET);
	fread(buf, sizeof(u_int8_t), 4, fp);
	chip->peripheral_base = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3];

	fseek(fp, 8, SEEK_SET);
	fread(buf, sizeof(u_int8_t), 4, fp);
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
	chip->pwmclock = (gpclock_peripherals *)(chip->peripherals + 0x40440);
	chip->pwm = (pwm_peripherals *)(chip->peripherals + 0x83000);

	return chip;
error:
	free(chip);
	return NULL;
}

void bcm2835_close(bcm2835 *chip) {
	munmap(chip->peripherals, chip->peripheral_size);
	free(chip);
}
