#include "Bullet.hpp"
#include "Enemy.hpp"

static constexpr double hurtFlashLength = 0.1;

Enemy::Enemy(Vec2 pos, float health, float damage, float spriteScale, const Texture2D& tex)
	: pos(pos), health(health), damage(damage), spriteScale(spriteScale), tex(tex) { }

void Enemy::draw(const GameState& gameState, const GameCamera& camera) const
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
		(float)tex.width * camera.getZoom() * spriteScale, (float)tex.height * camera.getZoom() * spriteScale
	};

	const float rot = vel.angle() * RAD2DEG;

	unsigned char whiteness = (unsigned char)clamp((gameState.time - timeOfLastHit) / hurtFlashLength * 255, 0.0, 255.0);
	Color tint = { 255, whiteness, whiteness, 255 };

	DrawTexturePro(tex, sourceRec, destRec, { destRec.width / 2, destRec.height / 2 }, rot, tint);
}

Vec2 Enemy::getPos() const
{
	return pos;
}

float Enemy::getHealth() const
{
	return health;
}

float Enemy::getDamage() const
{
	return damage;
}

void Enemy::hit(const GameState& gameState, const Bullet& bullet)
{
	health -= bullet.getDamage();
	vel += (bullet.getVel()  - vel).norm() * bullet.getRecoil();
	pos += randPointInCircle(5); // Subtle shake effect.
	timeOfLastHit = gameState.time;
}

bool Enemy::collidesWithPlayer(const Player& player) const
{
	return CheckCollisionCircles(pos, tex.width / 2.0f, player.getPos(), player.getSize() / 2);
}

bool Enemy::collidesWithBullet(const Bullet& bullet) const
{
	return CheckCollisionCircles(pos, tex.width / 2.0f, bullet.getPos(), bullet.getSize() / 2);
}
