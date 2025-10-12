#pragma once

#include "common.hpp"
#include "enemies.hpp"
#include "GameState.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
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

	Sound testSound;

	std::vector<Weapon> weapons;
	int weaponIndex = 0;

	Texture2D testBulletTex;

	std::vector<Bullet> enemyMadeBullets;
	std::vector<Bullet> playerMadeBullets;

	Texture2D testEnemyTex;

	std::vector<Enemy*> enemies;

	void loadLevel(const std::string& bgType, float borderRadius);
	void loadBg(const std::string& bgType, int maxNum);

	Weapon& getCurrentWeapon();

	void nextWeapon();
	void prevWeapon();

	void update();

	void keyInput();

	void updateEnemies(float dt);

	void updatePlayerMadeBullets(float dt);
	void updateEnemyMadeBullets(float dt);

	void draw() const;

	void drawBg() const;
	void drawBorder() const;
};