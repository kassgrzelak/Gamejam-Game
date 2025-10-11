#pragma once

#include "common.hpp"
#include "GameState.hpp"
#include "Player.hpp"

class Enemy
{
public:
	Enemy(Vec2 pos, float health, const char* spriteFilePath);
	~Enemy() = default;

	virtual void update(const GameState& gameState, const Player& player, float dt) = 0;

	void draw(const GameCamera& camera, const GameState& gameState) const;

protected:
	Vec2 pos;
	Vec2 vel = Vec2::zero();

	float health;

	Texture2D tex;
};