#pragma once

#include "common.hpp"
#include "GameState.hpp"

class Player
{
public:
	void init(Vec2 pos);

	void update(const GameState& gameState, float dt);
	void draw();

private:
	Vec2 pos;
	Vec2 vel = Vec2::zero();
	float rot = 0.0f;
	float rotVel = 0.0f;

	void updateVel(float dt);
	void updateRot(float dt);

	Vec2 getDir();
};