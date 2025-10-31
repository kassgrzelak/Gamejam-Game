#pragma once

#include "common.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"

enum class PickupType
{
	Health,
	Ammo1, Ammo2  // No, I am not proud of this.
};

class Pickup
{
public:
	Vec2 pos;
	PickupType type;
	Texture2D tex;

	Pickup(Vec2 pos, PickupType type, const Texture2D& tex);

	void draw(const GameState& gameState, const GameCamera& camera) const;
};