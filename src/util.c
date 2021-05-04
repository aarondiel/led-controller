#include "util.h"

void printBin(int n) {
	printf("0b");

	while (n) {
		if (n & 1)
			printf("1");
		else
			printf("0");

		n >>= 1;
	}

	printf("\n");
}

void nanodelay(unsigned int nanosec) {
	struct timespec req;

	req.tv_sec = 0;
	req.tv_nsec = nanosec;

	nanosleep(&req, NULL);
}
