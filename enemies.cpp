#include "Bullet.hpp"
#include "enemies.hpp"

TestEnemy::TestEnemy(Vec2 pos, const Texture2D& tex)
	: Enemy(pos, 1000.0f, 10.0f, 2.0f, tex)
{
	bulletTex = LoadTexture("assets/sprites/dog.png");
}

std::vector<Bullet> TestEnemy::update(const GameState& gameState, const Player& player, float dt)
{
	Vec2 dirToPlayer = player.getPos() - pos;

	if (dirToPlayer != Vec2::zero())
	{
		Vec2 targetVel = dirToPlayer.norm() * 500.0f;

		vel = vel.lerp(targetVel, 3.0f * dt);

		pos += vel * dt;
	}

	if (gameState.time - timeOfLastShot > 3.0)
	{
		Bullet bullet = Bullet(pos, vel + dirToPlayer.norm() * 1000.0f, 10, 100, 1, 0.5f, bulletTex);
		timeOfLastShot = gameState.time;

		return { bullet };
	}
	else
		return {};
}
