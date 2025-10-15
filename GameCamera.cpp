#include "GameCamera.hpp"

static constexpr float speedMult = 8.0f;
static constexpr float accel = 10.0f;
static constexpr float slowZoom = 1.0f;
static constexpr float fastZoom = 0.5f;
static constexpr float zoomAccel = 1.0f;

void GameCamera::init()
{
    zoom = slowZoom;
}

Vec2 GameCamera::getPos() const
{
	return pos;
}

Vec2 GameCamera::getVel() const
{
	return vel;
}

float GameCamera::getZoom() const
{
	return zoom;
}

void GameCamera::update(Vec2 playerPos, Vec2 playerVel, float playerVelRatio, float dt)
{
    const Vec2 rel = playerPos - pos;
    const float dist = rel.mag();

	if (dist != 0)
	{
		const float targetSpeed = dist * speedMult;
		const float newSpeed = lerp(vel.mag(), targetSpeed, accel * dt);

		vel = rel / dist * newSpeed;

		pos += vel * dt;
	}

	const float targetZoom = lerp(slowZoom, fastZoom, playerVelRatio);

	zoom = lerp(zoom, targetZoom, zoomAccel * dt);
}

Vec2 GameCamera::getScreenPos(const GameState& gameState, Vec2 pos) const
{
    Vec2 rel = pos - this->pos;
    return rel * zoom + gameState.getScreenCenter();
}
