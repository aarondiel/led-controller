#include "util.h"

// tests if arr contains duplicate values or values outside of 0 or maxvalue
bool duplicateElements(unsigned int *arr, unsigned int arrcount, unsigned int maxvalue) {
	unsigned int counter[maxvalue];

	for (int i = 0; i < arrcount; i++) {
		if (arr[i] < 0 || i > maxvalue)
			return true;
		
		if (++counter[arr[i]] > 1)
			return true;
	}

	return false;
}

void delay(unsigned int sec) {
	clock_t endtime = time(0) + sec;

	while (time(0) < endtime) {
		;
	}
}
