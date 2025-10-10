#pragma once

#include "common.hpp"
#include "GameState.hpp"
#include "Player.hpp"

class Game
{
public:
	void init();
	void run();
	void end();

private:
	GameState gameState;
	Player player;

	void update();
	void draw();
};