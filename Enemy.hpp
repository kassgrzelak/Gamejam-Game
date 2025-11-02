#pragma once

#include "common.hpp"
#include "GameState.hpp"
#include "Player.hpp"
#include <vector>

class Bullet;

class Enemy
{
public:
	Enemy(Vec2 pos, float health, float damage, float spriteScale, const Texture2D& tex);
	~Enemy();

	virtual std::vector<Bullet> update(const GameState& gameState, const Player& player, float dt) = 0;

	void draw(const GameState& gameState, const GameCamera& camera) const;

	void setPos(const Vec2& newPos);

	Vec2 getPos() const;
	Vec2 getVel() const;
	float getHealth() const;
	float getDamage() const;
	float getSize() const;

	void hit(const GameState& gameState, const Bullet& bullet);

	bool collidesWithPlayer(const Player& player) const;
	bool collidesWithBullet(const Bullet& bullet) const;

protected:
	Vec2 pos;
	Vec2 vel = Vec2::zero();

	float health;
	float damage;

	const Texture2D& tex;
	Texture2D bulletTex;
	Sound fireSound;
	float spriteScale;

	double timeOfLastHit = -999;
	double timeOfLastShot = -999;
};