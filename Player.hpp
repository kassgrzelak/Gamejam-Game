#pragma once

#include "common.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"

class Enemy;

class Player
{
public:
	void init();

	void update(const GameState& gameState, float dt);
	void draw(const GameState& gameState, const GameCamera& camera) const;

	void hit(const GameState& gameState, Enemy* enemy);

	Vec2 getPos() const;
	Vec2 getVel() const;
	float getRot() const;
	Vec2 getTipPos() const;
	float getVelRatio() const;
	float getSize() const;

	bool isDead() const;

	void setVel(Vec2 vel);

private:
	Vec2 pos = Vec2::zero();
	Vec2 vel = Vec2::zero();
	float rot = 0.0f;
	float rotVel = 0.0f;

	float health = 100;

	double timeOfLastHit = -999;

	Texture2D shipTex;

	void updateVel(float dt);
	void updateRot(float dt);
	void updateCollisions(const GameState& gameState);

	Vec2 getDir() const;
};