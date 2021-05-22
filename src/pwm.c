#include "pwm.h"

void pwm_enable_channel(pwm_peripherals *pwm, bool channel, bool en) {
	if (channel == 0)
		pwm->control.power1 = en;

	if (channel == 1)
		pwm->control.power2 = en;
}

// set channel mode to either pwm or serializer mode
void pwm_set_mode(pwm_peripherals *pwm, bool channel, bool mode) {
	if (channel == 0)
		pwm->control.mode1 = mode;

	if (channel == 1)
		pwm->control.mode2 = mode;
}

void pwm_use_fifo(pwm_peripherals *pwm, bool channel, bool en) {
	if (channel == 0)
		pwm->control.use_fifo1 = en;

	if (channel == 1)
		pwm->control.use_fifo2 = en;
}

void pwm_enable_dma(pwm_peripherals *pwm, bool en) {
	pwm->dma.enable = en;
}
