#pragma once

#include "common.hpp"

struct GameState
{
	int screenWidth;
	int screenHeight;

	float borderRadius;

	double time = 0.0;

	Vec2 getScreenCenter() const;
};