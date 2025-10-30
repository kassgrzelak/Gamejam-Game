#include "Bullet.hpp"

Bullet::Bullet(Vec2 pos, Vec2 vel, float damage, float recoil, int pierceLimit, float spriteScale, const Texture2D& tex)
	: pos(pos), vel(vel), damage(damage), recoil(recoil), pierceLimit(pierceLimit), spriteScale(spriteScale), tex(tex) { }

void Bullet::update(const std::list<Enemy*>& enemies, const Player& player, float dt)
{
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
		(float)tex.width * camera.getZoom() * spriteScale, (float)tex.height * camera.getZoom() * spriteScale
	};

	const float rot = vel.angle() * RAD2DEG + 90;

	DrawTexturePro(tex, sourceRec, destRec, { destRec.width / 2, destRec.height / 2 }, rot, WHITE);
}

Vec2 Bullet::getPos() const
{
	return pos;
}

Vec2 Bullet::getVel() const
{
	return vel;
}

float Bullet::getDamage() const
{
	return damage;
}

float Bullet::getRecoil() const
{
	return recoil;
}

float Bullet::getSize() const
{
	return tex.width * spriteScale;
}

int Bullet::getPierceLimit() const
{
	return pierceLimit;
}

const Texture2D& Bullet::getTex() const
{
	return tex;
}

void Bullet::setPos(Vec2 pos)
{
	this->pos = pos;
}

void Bullet::setVel(Vec2 vel)
{
	this->vel = vel;
}

void Bullet::addHitEnemy(Enemy* enemy)
{
	enemiesHit.insert(enemy);
	--pierceLimit;
}

bool Bullet::alreadyHit(Enemy* enemy) const
{
	return enemiesHit.contains(enemy);
}
