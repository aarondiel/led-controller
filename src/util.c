#include "util.h"

void delay(unsigned int sec) {
	clock_t endtime = time(0) + sec;

	while (time(0) < endtime) {
		;
	}
}
