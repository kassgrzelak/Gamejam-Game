#pragma once

#include "common.hpp"
#include "GameState.hpp"

class GameCamera
{
public:
	void init();

	void update(Vec2 playerPos, Vec2 playerVel, float playerVelRatio, float dt);

	Vec2 getPos() const;
	Vec2 getVel() const;
	float getZoom() const;

	Vec2 getScreenPos(const GameState& gameState, Vec2 pos) const;

private:
	Vec2 pos = Vec2::zero();
	Vec2 vel = Vec2::zero();
	float zoom;
};