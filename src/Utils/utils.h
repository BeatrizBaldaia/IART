#pragma once

#include <stdlib.h>

double getRandomBetween(double min, double max) {
	return ((double)rand() / RAND_MAX) * (max - min) + min;
}

bool probable(double probability) {
	return (double)rand() / RAND_MAX <= probability;
}
