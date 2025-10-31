#include "Pickup.hpp"

Pickup::Pickup(Vec2 pos, PickupType type, const Texture2D& tex)
	: pos(pos), type(type), tex(tex) { }

void Pickup::draw(const GameState& gameState, const GameCamera& camera) const
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

	DrawTexturePro(tex, sourceRec, destRec, { destRec.width / 2, destRec.height / 2 }, 0, WHITE);
}