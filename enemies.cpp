#include "Bullet.hpp"
#include "enemies.hpp"

Wasp::Wasp(Vec2 pos, const Texture2D& tex)
	: Enemy(pos, 50.0f, 5.0f, 3.0f, tex)
{
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

Fly::Fly(Vec2 pos, const Texture2D& tex)
	: Enemy(pos, 30.0f, 7.0f, 3.0f, tex)
{
}

std::vector<Bullet> Fly::update(const GameState& gameState, const Player& player, float dt)
{
	float dist = (player.getPos() - pos).mag();
	Vec2 dir = (player.getPos() + player.getVel() * dist / 1400.0f) - pos;

	if (dist != 0.0f)
	{
		Vec2 targetVel = dir.norm() * 1400.0f;

		vel = vel.lerp(targetVel, 4.0f * dt);

		pos += vel * dt;
	}

	return {};
}

BigFly::BigFly(Vec2 pos, const Texture2D& tex)
	: Enemy(pos, 100.0f, 10.0f, 5.0f, tex)
{
	bulletTex = LoadTexture("assets/sprites/laserRed.png");
}

std::vector<Bullet> BigFly::update(const GameState& gameState, const Player& player, float dt)
{
	Vec2 dir = player.getPos() - pos;

	if (dir != Vec2::zero())
	{
		Vec2 targetVel = dir.norm() * 800.0f;

		vel = vel.lerp(targetVel, 2.0f * dt);

		pos += vel * dt;
	}
	
	if (gameState.time - timeOfLastShot > 3.0f && dir != Vec2::zero())
	{
		Vec2 bulletDir = ((player.getPos() + player.getVel() * dir.mag() / 1600.0f * 0.5f) - pos).norm();
		Bullet bullet = Bullet(pos, bulletDir * 1600.0f, 10.0f, 1000.0f, 1, 3.0f, bulletTex);
		timeOfLastShot = gameState.time;

		return { bullet };
	}
	else
		return {};
}
