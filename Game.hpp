#pragma once

#include "common.hpp"

class Game
{
public:
	void init();

	void run();

	void end();

private:
	bool showFPS = false;

	int screenWidth = 800;
	int screenHeight = 600;

	void draw();
};