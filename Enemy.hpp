#pragma once

#include "common.hpp"
#include "GameState.hpp"
#include "Player.hpp"

class Enemy
{
public:
	Enemy(Vec2 pos, float health, const Texture2D& tex, float spriteScale);
	~Enemy() = default;

	virtual void update(const GameState& gameState, const Player& player, float dt) = 0;

	void draw(const GameCamera& camera, const GameState& gameState) const;

	Vec2 getPos() const;

	bool collidesWithPlayer(const Player& player) const;

protected:
	Vec2 pos;
	Vec2 vel = Vec2::zero();

	float health;

	const Texture2D& tex;
	float spriteScale;
};