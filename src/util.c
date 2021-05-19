#include "util.h"

void addNanoSec(struct timespec *clock, unsigned long nsec) {
	clock->tv_nsec += nsec;

	if (clock->tv_nsec >= 1000000000) {
		clock->tv_sec++;
		clock->tv_nsec -= 1000000000;
	}
}

void printBin(int n) {
	printf("0b");

	for (int i = 0; i < 32; i++) {
		if (n & (1 << (31 - i)))
			printf("1");
		else
			printf("0");

		if (i % 3 == 1)
			printf(" ");
	}

	printf("\n");
}
