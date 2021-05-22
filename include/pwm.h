#pragma once

#include <stdbool.h>

struct ctl_register {
	bool power1: 1;
	bool mode1: 1;
	bool repeat_last_data1: 1;
	bool silence_bit1: 1;
	bool polarity1: 1;
	bool use_fifo1: 1;
	bool clear_fifo1: 1;
	bool m_s1: 1;
	bool power2: 1;
	bool mode2: 1;
	bool repeat_last_data2: 1;
	bool silence_bit2: 1;
	bool polarity2: 1;
	bool use_fifo2: 1;
	bool reserved14: 1;
	bool m_s2: 1;
	short reserved16: 16;
};

struct dmac_register {
	char dreq_theshold: 8;
	char panic_theshold: 8;
	short reserved16: 15;
	bool enable: 1;
};

typedef struct {
	struct ctl_register control;
	unsigned int status: 32;
	struct dmac_register dma;
	unsigned int range1: 32;
	unsigned int data1: 32;
	unsigned int fifo1: 32;
	unsigned int range2: 32;
	unsigned int data2: 32;
} pwm_peripherals;

enum {
	PWM_MODE_PWM = 0,
	PWM_MODE_SERIALISER = 1
};

void pwm_enable_channel(pwm_peripherals *pwm, bool channel, bool en);
void pwm_set_mode(pwm_peripherals *pwm, bool channel, bool mode);
void pwm_use_fifo(pwm_peripherals *pwm, bool channel, bool en);
void pwm_enable_dma(pwm_peripherals *pwm, bool en);
