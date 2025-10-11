#pragma once

#include "common.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"

class Bullet
{
public:
	Bullet(const Texture2D& tex, Vec2 pos, Vec2 vel);

	void update(float dt);
	void draw(const GameState& gameState, const GameCamera& camera) const;

private:
	Vec2 pos;
	Vec2 vel;

	const Texture2D& tex;
};