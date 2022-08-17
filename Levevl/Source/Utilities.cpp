#include "Utilities.h"

const int Sign(const int i) {
	return (i > 0) - (i < 0);
}

const float Sign(const float f) {
	return (f > .0f) - (f < .0f);
}