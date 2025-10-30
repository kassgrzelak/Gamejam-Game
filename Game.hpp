#pragma once

#include "common.hpp"
#include "enemies.hpp"
#include "GameState.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include <list>
#include <string>
#include <vector>

class Game
{
public:
	void init();
	void run();
	void end();

private:
	int startupFrameCounter = 0;

	GameState gameState;
	Player player;
	GameCamera camera;

	Texture2D bgTex;
	Texture2D borderTex;

	Texture2D weaponDockTex;
	Texture2D fireBarTex;

	Texture2D healthDockTex;

	Sound testSound;

	Texture2D skullTex;
	int killCounter = 0;

	double timeOfLastEnemySpawn = -999;
	int enemySpawnCounter = 0;

	std::vector<Weapon> weapons;
	size_t weaponIndex = 0;

	Texture2D orbTex;
	Texture2D laserTex;

	std::vector<Bullet> enemyMadeBullets;
	std::vector<Bullet> playerMadeBullets;

	Texture2D testEnemyTex;

	std::list<Enemy*> enemies;

	void loadLevel(const std::string& bgType, float borderRadius);
	void loadBg(const std::string& bgType, int maxNum);

	Weapon& getCurrentWeapon();
	const Weapon& getCurrentWeapon() const;

	void nextWeapon();
	void prevWeapon();

	void update();

	void keyInput();

	void updateEnemies(float dt);

	void spawnEnemies();

	void updatePlayerMadeBullets(float dt);
	void updateEnemyMadeBullets(float dt);

	void draw() const;

	void drawBg() const;
	void drawBorder() const;

	void drawWeaponUI() const;
	void drawWeaponDock() const;
	void drawFireBar() const;
	void drawUIBullet() const;

	void drawHealthUI() const;
	void drawHealthDock() const;

	Color getSkullTint() const;
};