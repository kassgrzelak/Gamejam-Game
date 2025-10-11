#pragma once

#include "common.hpp"

struct GameState
{
	int screenWidth;
	int screenHeight;

	float borderRadius;

	Vec2 getScreenCenter() const;
};