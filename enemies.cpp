#include "enemies.hpp"

TestEnemy::TestEnemy(Vec2 pos)
	: Enemy(pos, 100, "assets/ship.png") { }

void TestEnemy::update(const GameState& gameState, const Player& player, float dt)
{
	pos += Vec2(100, 0) * dt;
}
