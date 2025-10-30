#include "Bullet.hpp"
#include "enemies.hpp"

TestEnemy::TestEnemy(Vec2 pos, const Texture2D& tex)
	: Enemy(pos, 100.0f, 5.0f, 3.0f, tex)
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

	/*if (gameState.time - timeOfLastShot > 1.5)
	{
		Bullet bullet = Bullet(pos, vel + (player.getPos() + player.getVel() * 2.0f - pos).norm() * 1000.0f, damage, 100, 1, 0.5f, bulletTex);
		timeOfLastShot = gameState.time;

		return { bullet };
	}
	else*/
	return {};
}
