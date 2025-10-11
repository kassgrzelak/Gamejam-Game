#include "Enemy.hpp"

Enemy::Enemy(Vec2 pos, float health, const char* spriteFilePath)
	: pos(pos), health(health)
{
	tex = LoadTexture(spriteFilePath);
}

void Enemy::draw(const GameCamera& camera, const GameState& gameState) const
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