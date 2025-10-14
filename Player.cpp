#include "Bullet.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

static constexpr Vec2 frontDir = { 0.0f, -1.0f }; // The direction the player faces before any rotation is applied.

static constexpr float maxSpeed = 1200.0f;
static constexpr float normalAccel = 2.0f;
static constexpr float overspeedAccel = 40.0f;

static constexpr float rotSpeed = 10.0f;
static constexpr float rotAccel = 10.0f;

static constexpr float collisionDamping = 0.6f;

static constexpr float playerScale = 1.0f;

static constexpr double hurtFlashLength = 0.1;
static constexpr double invincibilityLength = 0.25;

void Player::init()
{
	shipTex = LoadTexture("assets/sprites/ship.png");
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

	unsigned char whiteness = (unsigned char)clamp((gameState.time - timeOfLastHit) / hurtFlashLength * 255, 0.0, 255.0);
	
	if (isDead())
		whiteness = 0;

	Color tint = { 255, whiteness, whiteness, 255 };

	DrawTexturePro(shipTex, sourceRec, destRec, { destRec.width / 2, destRec.height / 2 }, rot * RAD2DEG, tint);
}

void Player::hit(const GameState& gameState, Enemy* enemy)
{
	if (gameState.time - timeOfLastHit < invincibilityLength)
		return;

	timeOfLastHit = gameState.time;
	health -= enemy->getDamage();
}

void Player::hit(const GameState& gameState, const Bullet& bullet)
{
	if (gameState.time - timeOfLastHit < invincibilityLength)
		return;

	timeOfLastHit = gameState.time;
	health -= bullet.getDamage();
	vel += (bullet.getVel() - vel).norm() * bullet.getRecoil();
}

Vec2 Player::getPos() const
{
	return pos;
}

Vec2 Player::getVel() const
{
	return vel;
}

float Player::getRot() const
{
	return rot;
}

Vec2 Player::getTipPos() const
{
	return pos + Vec2(0, -shipTex.width * playerScale / 2.0f).rotate(rot);
}

float Player::getVelRatio() const
{
	return vel.mag() / maxSpeed;
}

float Player::getSize() const
{
	return shipTex.width * playerScale;
}

bool Player::isDead() const
{
	return health <= 0;
}

bool Player::collidesWithBullet(const Bullet& bullet) const
{
	return CheckCollisionCircles(pos, shipTex.width * playerScale / 2.0f, bullet.getPos(), bullet.getSize() / 2);
}

void Player::setVel(Vec2 vel)
{
	this->vel = vel;
}

void Player::updateVel(float dt)
{
	Vec2 targetVel = Vec2::zero();

	if (IsKeyDown(KEY_W) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
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
	else
	{
		Vec2 mouseDelta = GetMouseDelta();
		targetRotVel = mouseDelta.x * rotSpeed;
	}

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
