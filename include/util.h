#pragma once

#include <time.h>
#include <stdbool.h>
#include <stdio.h>

struct color {
	char g;
	char r;
	char b;
};

void addNanoSec(struct timespec *clock, unsigned long nsec);
void printBin(int n);
