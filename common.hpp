#pragma once

#include "logging.hpp"
#include "rand.hpp"
#include "raylib.h"
#include "Vec2.hpp"

float lerp(float a, float b, float t);

template<typename T>
T clamp(T val, T lower, T upper)
{
	return val < lower ? lower : (val > upper ? upper : val);
}

Vec2 randPointInCircle(float radius);