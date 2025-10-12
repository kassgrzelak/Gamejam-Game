#include "Enemy.hpp"

Enemy::Enemy(Vec2 pos, float health, const Texture2D& tex, float spriteScale)
	: pos(pos), health(health), tex(tex), spriteScale(spriteScale) { }

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
		(float)tex.width * camera.getZoom() * spriteScale, (float)tex.height * camera.getZoom() * spriteScale
	};

	const float rot = vel.angle() * RAD2DEG;

	DrawTexturePro(tex, sourceRec, destRec, { destRec.width / 2, destRec.height / 2 }, rot, WHITE);
}

Vec2 Enemy::getPos() const
{
	return pos;
}

bool Enemy::collidesWithPlayer(const Player& player) const
{
	return CheckCollisionCircles(pos, tex.width / 2.0f, player.getPos(), player.getSize() / 2);
}
