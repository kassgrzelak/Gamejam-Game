#pragma once

#include "common.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"

class Bullet;
class Enemy;

class Player
{
public:
	void init();

	void update(const GameState& gameState, float dt);
	void draw(const GameState& gameState, const GameCamera& camera) const;
	void drawHealthUI(const GameState& gameState) const;

	void hit(const GameState& gameState, Enemy* enemy);
	void hit(const GameState& gameState, const Bullet& bullet);

	Vec2 getPos() const;
	Vec2 getVel() const;
	float getRot() const;
	Vec2 getTipPos() const;
	float getVelRatio() const;
	float getSize() const;
	float getHealthRatio() const;

	bool isDead() const;
	bool collidesWithBullet(const Bullet& bullet) const;

	void setVel(Vec2 vel);

private:
	Vec2 pos = Vec2::zero();
	Vec2 vel = Vec2::zero();
	float rot = 0.0f;
	float rotVel = 0.0f;

	const float maxHealth = 100;
	float health = maxHealth;

	Texture2D healthBarTex;
	Texture2D damageBarTex;

	float getHealthBarX() const;

	float damageBarX = 0;
	bool damageBarMoving = false;

	double timeOfLastHit = -999;
	double timeOfDamageBarHit = -999;

	Texture2D shipTex;

	void updateVel(float dt);
	void updateRot(float dt);
	void updateCollisions(const GameState& gameState);

	Vec2 getDir() const;
};