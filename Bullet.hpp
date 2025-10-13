#pragma once

#include "common.hpp"
#include "Enemy.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"
#include <unordered_set>
#include <vector>

class Bullet
{
public:
	Bullet(Vec2 pos, Vec2 vel, float damage, float recoil, int pierceLimit, float spriteScale, const Texture2D& tex);

	void update(const std::vector<Enemy*>& enemies, const Player& player, float dt);
	void draw(const GameState& gameState, const GameCamera& camera) const;

	Vec2 getPos() const;
	Vec2 getVel() const;
	float getDamage() const;
	float getRecoil() const;
	float getSize() const;
	int getPierceLimit() const;

	void setPos(Vec2 pos);
	void setVel(Vec2 vel);

	void addHitEnemy(Enemy* enemy);
	bool alreadyHit(Enemy* enemy) const;

private:
	Vec2 pos;
	Vec2 vel;

	float damage;
	float recoil;
	int pierceLimit;

	std::unordered_set<Enemy*> enemiesHit = {};

	float spriteScale;

	Texture2D tex;
};