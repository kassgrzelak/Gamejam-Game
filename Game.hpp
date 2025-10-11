#pragma once

#include "common.hpp"
#include "enemies.hpp"
#include "GameState.hpp"
#include "Player.hpp"
#include <string>
#include <vector>

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

	std::vector<Enemy*> enemies;

	void loadLevel(const std::string& bgType, float borderRadius);
	void loadBg(const std::string& bgType, int maxNum);

	void update();

	void updateEnemies(float dt);

	void draw() const;

	void drawBg() const;
	void drawBorder() const;
};