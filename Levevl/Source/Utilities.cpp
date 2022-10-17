#include "Utilities.h"

Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(int x, int y) : x(x), y(y) {}

Vector2 Vector2::Up() {
	return Vector2(0, -1);
}

Vector2 Vector2::Left() {
	return Vector2(-1, 0);
}

Vector2 Vector2::Down() {
	return Vector2(0, 1);
}

Vector2 Vector2::Right() {
	return Vector2(1, 0);
}

const int Sign(const int i) {
	return (i > 0) - (i < 0);
}

const float Sign(const float f) {
	return (f > .0f) - (f < .0f);
}