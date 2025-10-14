#pragma once

#include "Enemy.hpp"

class TestEnemy : public Enemy
{
public:
	TestEnemy(Vec2 pos, const Texture2D& tex);

	std::vector<Bullet> update(const GameState& gameState, const Player& player, float dt) override;
};