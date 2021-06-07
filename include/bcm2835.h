#pragma once
#pragma pack(1)

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

typedef struct {
	bool power0: 1;
	bool mode0: 1;
	bool repeat_last_data0: 1;
	bool silence_bit0: 1;
	bool polarity0: 1;
	bool use_fifo0: 1;
	bool clear_fifo0: 1;
	bool m_s0: 1;
	bool power1: 1;
	bool mode1: 1;
	bool repeat_last_data1: 1;
	bool silence_bit1: 1;
	bool polarity1: 1;
	bool use_fifo1: 1;
	bool: 1;
	bool m_s1: 1;
	u_int16_t: 16;
} pwm_ctl_register;

typedef struct {
	bool fifo_full0: 1;
	bool fifo_empty0: 1;
	bool fifo_write_error0: 1;
	bool gap_occured0: 1;
	bool gap_occured1: 1;
	bool gap_occured2: 1;
	bool gap_occured3: 1;
	bool bus_error: 1;
	bool state0: 1;
	bool state1: 1;
	bool state2: 1;
	bool state3: 1;
	u_int32_t: 20;
} pwm_status_register;

typedef struct {
	u_int8_t dreq_theshold: 8;
	u_int8_t panic_theshold: 8;
	u_int16_t: 15;
	bool enable: 1;
} pwm_dmac_register;

typedef struct {
	pwm_ctl_register control;
	pwm_status_register status;
	pwm_dmac_register dma;
	u_int32_t: 32;
	u_int32_t range0: 32;
	u_int32_t data0: 32;
	u_int32_t fifo0: 32;
	u_int32_t: 32;
	u_int32_t range1: 32;
	u_int32_t data1: 32;
} pwm_peripherals;

enum {
	PWM_MODE_PWM = 0,
	PWM_MODE_SERIALISER = 1
};

typedef struct {
	u_int32_t function_select[6];
	u_int32_t: 32;
	u_int32_t set[2];
	u_int32_t: 32;
	u_int32_t clear[2];
	u_int32_t: 32;
	u_int32_t level[2];
	u_int32_t: 32;
	u_int32_t event_status[2];
	u_int32_t: 32;
	u_int32_t rising_edge_detect_enable[2];
	u_int32_t: 32;
	u_int32_t falling_edge_detect_enable[2];
	u_int32_t: 32;
	u_int32_t high_detect_enable[2];
	u_int32_t: 32;
	u_int32_t low_detect_enable[2];
	u_int32_t: 32;
	u_int32_t async_rising_edge_detect[2];
	u_int32_t: 32;
	u_int32_t async_falling_edge_detect[2];
	u_int32_t: 32;
	u_int32_t pull_up_down_enable;
	u_int32_t pull_up_down_clock[2];
	u_int32_t: 32;
} gpio_peripherals;

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

typedef struct {
	u_int8_t source: 4;
	bool enable: 1;
	bool kill: 1;
	bool: 1;
	bool busy: 1;
	bool invert: 1;
	u_int8_t mash: 2;
	u_int16_t: 13;
	u_int8_t password: 8;
} gpclock_control_register;

typedef struct {
	u_int16_t floating_part: 12;
	u_int16_t integer_part: 12;
	u_int8_t password: 8;
} gpclock_div_register;

#define gpclock_password 0x5a

typedef union {
	gpclock_control_register control;
	gpclock_div_register div;
} gpclock_peripherals;

enum {
	GPCLOCK_SOURCE_GROUND = 0,
	GPCLOCK_SOURCE_OSCILLATOR = 1,
	GPCLOCK_SOURCE_TESTDEBUG0 = 2,
	GPCLOCK_SOURCE_TESTDEBUG1 = 3,
	GPCLOCK_SOURCE_PLLA = 3,
	GPCLOCK_SOURCE_PLLC = 4,
	GPCLOCK_SOURCE_PLLD = 5,
	GPCLOCK_SOURCE_HDMI = 6,
};

#pragma pack()
typedef struct {
	u_int32_t peripheral_base;
	u_int32_t peripheral_size;
	u_int32_t *peripherals;
	gpio_peripherals *gpio;
	gpclock_peripherals *pwmclock;
	pwm_peripherals *pwm;
} bcm2835;

void bcm2835_delay(u_int64_t millis);
bool gpio_set_function(gpio_peripherals *gpio, u_int8_t pin, u_int8_t function);
bool gpio_write_line(gpio_peripherals *gpio, u_int8_t pin, bool state);
bool gpio_read_line(gpio_peripherals *gpio, u_int8_t pin);
bool gpclock_set_clock(gpclock_peripherals *gpclock, u_int16_t integer, u_int16_t floating);
bcm2835 *bcm2835_open();
void bcm2835_close(bcm2835 *chip);
