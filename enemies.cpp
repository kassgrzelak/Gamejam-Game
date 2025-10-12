#include "enemies.hpp"

TestEnemy::TestEnemy(Vec2 pos, const Texture2D& tex)
	: Enemy(pos, 100.0f, 10.0f, 2.0f, tex) { }

void TestEnemy::update(const GameState& gameState, const Player& player, float dt)
{
	Vec2 dirToPlayer = player.getPos() - pos;

	if (dirToPlayer != Vec2::zero())
	{
		Vec2 targetVel = dirToPlayer.norm() * 500.0f;

		vel = vel.lerp(targetVel, 3.0f * dt);

		pos += vel * dt;
	}
}
