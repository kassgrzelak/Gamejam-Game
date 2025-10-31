#include "Bullet.hpp"
#include "enemies.hpp"

Wasp::Wasp(Vec2 pos, const Texture2D& tex)
	: Enemy(pos, 50.0f, 5.0f, 3.0f, tex)
{
	bulletTex = LoadTexture("assets/sprites/dog.png");
}

std::vector<Bullet> Wasp::update(const GameState& gameState, const Player& player, float dt)
{
	Vec2 dirToPlayer = player.getPos() - pos;

	if (dirToPlayer != Vec2::zero())
	{
		Vec2 targetVel = dirToPlayer.norm() * 1000.0f;

		vel = vel.lerp(targetVel, 3.0f * dt);

		pos += vel * dt;
	}

	return {};
}
