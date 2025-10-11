#pragma once

#include "common.hpp"
#include "GameState.hpp"
#include "Player.hpp"
#include <string>

class Game
{
public:
	void init();
	void run();
	void end();

private:
	GameState gameState;
	Player player;
	GameCamera camera;

	Texture2D bgTex;
	Texture2D borderTex;

	void loadLevel(const std::string& bgType, float borderRadius);
	void loadBg(const std::string& bgType, int maxNum);

	void update();
	void draw() const;

	void drawBg() const;
	void drawBorder() const;
};