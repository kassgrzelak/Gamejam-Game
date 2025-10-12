#include "Bullet.hpp"

Bullet::Bullet(Vec2 pos, Vec2 vel, float homingStrength, float homingDistance, bool homesOnEnemies, const Texture2D& tex)
	: pos(pos), vel(vel), homingStrength(homingStrength), homingDistance(homingDistance),
	homesOnEnemies(homesOnEnemies), tex(tex) { }

void Bullet::update(const std::vector<Enemy*>& enemies, const Player& player, float dt)
{
	if (homesOnEnemies)
	{
		for (auto enemy : enemies)
		{
			Vec2 rel = enemy->getPos();
			
			if (rel.sqrMag() < homingDistance * homingDistance)
			{
				vel = vel.lerp(rel.norm() * vel.mag(), homingStrength * dt);
				break;
			}
		}
	}
	else
	{
		Vec2 rel = player.getPos();

		if (rel.sqrMag() < homingDistance * homingDistance)
			vel = vel.lerp(rel.norm() * vel.mag(), homingStrength * dt);
	}

	pos += vel * dt;
}

void Bullet::draw(const GameState& gameState, const GameCamera& camera) const
{
	const Vec2 screenPos = camera.getScreenPos(gameState, pos);

	const Rectangle sourceRec =
	{
		0, 0,
		(float)tex.width, (float)tex.height
	};
	const Rectangle destRec =
	{
		screenPos.x, screenPos.y,
		(float)tex.width * camera.getZoom(), (float)tex.height * camera.getZoom()
	};

	const float rot = vel.angle() * RAD2DEG;

	DrawTexturePro(tex, sourceRec, destRec, { destRec.width / 2, destRec.height / 2 }, rot, WHITE);
}

Vec2 Bullet::getPos() const
{
	return pos;
}