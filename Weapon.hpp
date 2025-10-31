#pragma once

#include "Bullet.hpp"

class Weapon
{
public:
	const char* name;

	Weapon(const char* name, int ammo, float bulletSpeed, double fireTime, int numBullets, float coneAngle, Bullet bullet, const Sound& fireSound);

	int getAmmo() const;
	float getFireTimeRatio(const GameState& gameState) const;
	const Bullet& getBullet() const;

	std::vector<Bullet> fire(const GameState& gameState, Player& player);

	void ammoPickup();

private:
	int ammo;
	float bulletSpeed;
	double fireTime;

	int numBullets;
	float coneAngle;

	Bullet bullet;

	double timeOfLastShot = -999;

	const Sound& fireSound;
};