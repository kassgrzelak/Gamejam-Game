#include "common.hpp"

float lerp(float a, float b, float t)
{
	return (1 - t) * a + t * b;
}

Vec2 randPointInCircle(float radius)
{
	const float r = radius * sqrt(randF());
	const float theta = randF() * 2 * PI;

	return Vec2(r * cos(theta), r * sin(theta));
}
