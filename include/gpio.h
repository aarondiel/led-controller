#pragma once

#include <malloc.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
	unsigned int function_select[6];
	unsigned int reserved_08;
	unsigned int set[2];
	unsigned int reserved_24;
	unsigned int clear[2];
	unsigned int reserved_30;
	unsigned int level[2];
	unsigned int reserved_3c;
	unsigned int event_status[2];
	unsigned int reserved_48;
	unsigned int rising_edge_detect_enable[2];
	unsigned int reserved_54;
	unsigned int falling_edge_detect_enable[2];
	unsigned int reserved_60;
	unsigned int high_detect_enable[2];
	unsigned int reserved_6c;
	unsigned int low_detect_enable[2];
	unsigned int reserved_78;
	unsigned int async_rising_edge_detect[2];
	unsigned int reserved_84;
	unsigned int async_falling_edge_detect[2];
	unsigned int reserved_90;
	unsigned int pull_up_down_enable;
	unsigned int pull_up_down_clock[2];
	unsigned int reserved_a0;
} gpio_peripherals;

typedef struct {
	unsigned int peripheral_base;
	unsigned int peripheral_size;
	unsigned int *peripherals;
	gpio_peripherals *gpio;
} bcm2835;

enum gpio_function {
	GPIO_FUNCTION_INPUT = 0b000,
	GPIO_FUNCTION_OUTPUT = 0b001,
	GPIO_FUNCTION_FUNC0 = 0b100,
	GPIO_FUNCTION_FUNC1 = 0b101,
	GPIO_FUNCTION_FUNC2 = 0b110,
	GPIO_FUNCTION_FUNC3 = 0b111,
	GPIO_FUNCTION_FUNC4 = 0b011,
	GPIO_FUNCTION_FUNC5 = 0b010
};

bcm2835 *openChip();
void closeChip(bcm2835 *chip);
bool setFunction(gpio_peripherals *gpio, unsigned char pin, unsigned char function);
bool writeLine(gpio_peripherals *gpio, unsigned char pin, bool state);
bool readLine(gpio_peripherals *gpio, unsigned char pin);
