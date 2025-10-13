#pragma once

#include "common.hpp"
#include "GameState.hpp"
#include "Player.hpp"

class Bullet;

class Enemy
{
public:
	Enemy(Vec2 pos, float health, float damage, float spriteScale, const Texture2D& tex);
	~Enemy() = default;

	virtual void update(const GameState& gameState, const Player& player, float dt) = 0;

	void draw(const GameState& gameState, const GameCamera& camera) const;

	Vec2 getPos() const;
	float getHealth() const;
	float getDamage() const;

	void hit(const GameState& gameState, const Bullet& bullet);

	bool collidesWithPlayer(const Player& player) const;
	bool collidesWithBullet(const Bullet& bullet) const;

protected:
	Vec2 pos;
	Vec2 vel = Vec2::zero();

	float health;
	float damage;

	const Texture2D& tex;
	float spriteScale;

	double timeOfLastHit = -999;
};