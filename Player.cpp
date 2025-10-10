#include "Player.hpp"

static constexpr Vec2 frontDir = { 0.0f, -1.0f }; // The direction the player faces before any rotation is applied.

static constexpr float maxSpeed = 1200.0f;
static constexpr float normalAccel = 2.0f;
static constexpr float overspeedAccel = 40.0f;

static constexpr float rotSpeed = 5.0f;
static constexpr float rotAccel = 5.0f;

static Texture2D shipTex;

void Player::init(Vec2 pos)
{
	this->pos = pos;
	shipTex = LoadTexture("assets/ship.png");
}

void Player::update(const GameState& gameState, float dt)
{
	updateVel(dt);
	updateRot(dt);
}

void Player::draw()
{
	const Rectangle sourceRec =
	{
		0, 0,
		(float)shipTex.width, (float)shipTex.height
	};
	const Rectangle destRec =
	{
		(float)pos.x, (float)pos.y,
		(float)shipTex.width * 2, (float)shipTex.height * 2
	};

	DrawTexturePro(shipTex, sourceRec, destRec, { destRec.width / 2, destRec.height / 2 }, rot * RAD2DEG, WHITE);
}

void Player::updateVel(float dt)
{
	Vec2 targetVel = Vec2::zero();

	if (IsKeyDown(KEY_W))
	{
		targetVel = getDir() * maxSpeed;
	}

	const float accel = vel.sqrMag() > maxSpeed * maxSpeed ? overspeedAccel : normalAccel;

	vel = vel.lerp(targetVel, accel * dt);
	pos += vel * dt;
}

static float lerpAngle(float a, float b, float t)
{
	const float diff = fmod(b - a, 2.0f * PI);
	const float shortestDist = fmod(2 * diff, 2.0f * PI) - diff;

	return a + shortestDist * t;
}

static float lerp(float a, float b, float t)
{
	return (1 - t) * a + t * b;
}

void Player::updateRot(float dt)
{
	float targetRotVel = 0.0f;

	if (IsKeyDown(KEY_A))
		targetRotVel -= rotSpeed;
	else if (IsKeyDown(KEY_D))
		targetRotVel += rotSpeed;

	rotVel = lerp(rotVel, targetRotVel, rotAccel * dt);
	rot += rotVel * dt;
}

Vec2 Player::getDir()
{
	return frontDir.rotate(rot);
}
