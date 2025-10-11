#include "Bullet.hpp"
#include "Player.hpp"

static constexpr Vec2 frontDir = { 0.0f, -1.0f }; // The direction the player faces before any rotation is applied.

static constexpr float maxSpeed = 1200.0f;
static constexpr float normalAccel = 2.0f;
static constexpr float overspeedAccel = 40.0f;

static constexpr float rotSpeed = 5.0f;
static constexpr float rotAccel = 5.0f;

static constexpr float collisionDamping = 0.6f;

static constexpr float playerScale = 1.0f;

void Player::init()
{
	shipTex = LoadTexture("assets/ship.png");
}

void Player::update(const GameState& gameState, float dt)
{
	updateVel(dt);
	updateRot(dt);
	updateCollisions(gameState);
}

void Player::draw(const GameState& gameState, const GameCamera& camera) const
{
	const Vec2 screenPos = camera.getScreenPos(gameState, pos);

	const Rectangle sourceRec =
	{
		0, 0,
		(float)shipTex.width, (float)shipTex.height
	};
	const Rectangle destRec =
	{
		(float)screenPos.x, (float)screenPos.y,
		(float)shipTex.width * playerScale * camera.getZoom(), (float)shipTex.height * playerScale * camera.getZoom()
	};

	DrawTexturePro(shipTex, sourceRec, destRec, { destRec.width / 2, destRec.height / 2 }, rot * RAD2DEG, WHITE);
}

Vec2 Player::getPos() const
{
	return pos;
}

Vec2 Player::getVel() const
{
	return vel;
}

float Player::getVelRatio() const
{
	return vel.mag() / maxSpeed;
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

void Player::updateCollisions(const GameState& gameState)
{
	if (pos.sqrMag() > gameState.borderRadius * gameState.borderRadius)
	{
		Vec2 normal = pos.norm();

		while (pos.sqrMag() > gameState.borderRadius * gameState.borderRadius)
			pos -= normal;

		float inComp = vel.dot(normal); // Component into border.

		vel -= normal * inComp; // Cancel that component.
		vel -= normal * inComp * collisionDamping; // Then bounce back with damping.
	}
}

Vec2 Player::getDir() const
{
	return frontDir.rotate(rot);
}
