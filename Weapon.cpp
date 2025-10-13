#include "Weapon.hpp"

Weapon::Weapon(const char* name, int ammo, float bulletSpeed, double fireTime, int numBullets, float coneAngle, Bullet bullet, const Sound& fireSound)
	: name(name), ammo(ammo), bulletSpeed(bulletSpeed), fireTime(fireTime),
	numBullets(numBullets), coneAngle(coneAngle), bullet(bullet), fireSound(fireSound) { }

std::vector<Bullet> Weapon::fire(const GameState& gameState, Player& player)
{
	if (gameState.time - timeOfLastShot < fireTime || ammo == 0) return {};

	const Vec2 oldPlayerVel = player.getVel();
	//player.setVel(oldPlayerVel + Vec2(0, bullet.getRecoil()).rotate(player.getRot()));

	PlaySound(fireSound);
	--ammo;
	timeOfLastShot = gameState.time;

	std::vector<Bullet> bullets = {};

	if (numBullets % 2 == 1)
	{
		bullet.setPos(player.getTipPos());
		bullet.setVel(Vec2(0, -bulletSpeed).rotate(player.getRot()) + oldPlayerVel);

		bullets.push_back(bullet);
	}

	const float halfAngle = coneAngle / 2;
	const float step = halfAngle / (numBullets / 2);

	for (float angle = halfAngle; angle > 0.01f; angle -= step)
	{
		bullet.setPos(player.getTipPos());
		bullet.setVel(Vec2(0, -bulletSpeed).rotate((player.getRot() + angle)) + oldPlayerVel);
		bullets.push_back(bullet);

		bullet.setPos(player.getTipPos());
		bullet.setVel(Vec2(0, -bulletSpeed).rotate((player.getRot() - angle)) + oldPlayerVel);
		bullets.push_back(bullet);
	}

	return bullets;
}
