#pragma once

#include "common.hpp"
#include "Enemy.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"
#include <vector>

class Bullet
{
public:
	Bullet(Vec2 pos, Vec2 vel, float homingStrength, float homingDistance, bool homesOnEnemies, const Texture2D& tex);

	void update(const std::vector<Enemy*>& enemies, const Player& player, float dt);
	void draw(const GameState& gameState, const GameCamera& camera) const;

	Vec2 getPos() const;

private:
	Vec2 pos;
	Vec2 vel;

	float homingStrength;
	float homingDistance;
	bool homesOnEnemies;

	const Texture2D& tex;
};