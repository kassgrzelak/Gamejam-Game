#include "Bullet.hpp"

Bullet::Bullet(const Texture2D& tex, Vec2 pos, Vec2 vel)
	: tex(tex), pos(pos), vel(vel) { }

void Bullet::update(float dt)
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
		(float)tex.width / camera.getZoom(), (float)tex.height / camera.getZoom()
	};

	const float rot = vel.angle() * RAD2DEG;

	DrawTexturePro(tex, sourceRec, destRec, { destRec.width / 2, destRec.height / 2 }, rot, WHITE);
}