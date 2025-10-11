#pragma once

#include "Enemy.hpp"

class TestEnemy : public Enemy
{
public:
	TestEnemy(Vec2 pos);

	void update(const GameState& gameState, const Player& player, float dt) override;
};