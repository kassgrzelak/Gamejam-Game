#include "Game.hpp"
#include <string>

static constexpr float bgScale = 2.0f;
static constexpr float parallax = 1.2f;

static constexpr int startupFrames = 20;

static constexpr double enemySpawnDelay = 10.0f;

void Game::init()
{
	initRand();

	InitWindow(0, 0, "Gamejam Game");
	InitAudioDevice();
	ToggleFullscreen();
	DisableCursor();

	const int monitor = GetCurrentMonitor();

	gameState.screenWidth = GetMonitorWidth(monitor);
	gameState.screenHeight = GetMonitorHeight(monitor);

	SetWindowSize(gameState.screenWidth, gameState.screenHeight);

	player.init();
	camera.init();

	borderTex = LoadTexture("assets/sprites/border.png");

	weaponDockTex = LoadTexture("assets/sprites/weaponDock.png");
	fireBarTex = LoadTexture("assets/sprites/fireBar.png");

	healthDockTex = LoadTexture("assets/sprites/healthDock.png");

	skullTex = LoadTexture("assets/sprites/skull.png");

	testSound = LoadSound("assets/sfx/testSound.wav");

	orbTex = LoadTexture("assets/sprites/orb.png");
	laserTex = LoadTexture("assets/sprites/laser.png");

	weapons.push_back(Weapon("Plasma Rifle", 999, 1000, 0.1, 1, 0.0f,
		Bullet(Vec2::zero(), Vec2::zero(), 10, 100, 1, 0.8f, orbTex), testSound));

	weapons.push_back(Weapon("Laser Shotgun", 999, 1500, 0.8, 5, 0.4f,
		Bullet(Vec2::zero(), Vec2::zero(), 5, 500, 3, 1.5f, laserTex), testSound));

	weapons.push_back(Weapon("Laser Sniper", 999, 5000, 1.4, 1, 0.0f,
		Bullet(Vec2::zero(), Vec2::zero(), 50, 1000, 5, 2.5f, laserTex), testSound));

	testEnemyTex = LoadTexture("assets/sprites/enemy1.png");

	loadLevel("purp", 2000.0f);
}

void Game::run()
{
	while (!WindowShouldClose())
	{
		if (startupFrameCounter < startupFrames)
		{
			++startupFrameCounter;
			SetWindowFocused();
		}
		else
			update();

		draw();
	}
}

void Game::end()
{
	CloseWindow();
}

void Game::loadLevel(const std::string& bgType, float borderRadius)
{
	if (bgType == "blue")
		loadBg("blue", 5);
	else if (bgType == "green")
		loadBg("green", 5);
	else if (bgType == "purp")
		loadBg("purp", 5);
	else if (bgType == "star")
		loadBg("star", 5);
	else
		throw std::exception("invalid bgType.");

	gameState.borderRadius = borderRadius;
}

void Game::loadBg(const std::string& bgType, int maxNum)
{
	const int num = randInt(1, maxNum);
	std::string path = "assets/sprites/" + bgType + std::to_string(num) + ".png";

	bgTex = LoadTexture(path.c_str());
}

Weapon& Game::getCurrentWeapon()
{
	return weapons[weaponIndex];
}

const Weapon& Game::getCurrentWeapon() const
{
	return weapons[weaponIndex];
}

void Game::nextWeapon()
{
	++weaponIndex;
	weaponIndex %= weapons.size();
}

void Game::prevWeapon()
{
	--weaponIndex;

	if (weaponIndex == -1)
		weaponIndex = weapons.size() - 1;

	weaponIndex %= weapons.size();
}

void Game::update()
{
	gameState.time = GetTime();
	const float dt = GetFrameTime();

	keyInput();

	spawnEnemies();

	player.update(gameState, dt);

	updateEnemies(dt);
	updatePlayerMadeBullets(dt);
	updateEnemyMadeBullets(dt);
	camera.update(player.getPos(), player.getVel(), player.getVelRatio(), dt);
}

void Game::keyInput()
{
	if (!player.isDead() && (IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
	{
		std::vector<Bullet> bullets = getCurrentWeapon().fire(gameState, player);
		playerMadeBullets.insert(playerMadeBullets.end(), bullets.begin(), bullets.end());
	}

	if (GetMouseWheelMoveV().y < 0 || IsKeyPressed(KEY_Q))
		prevWeapon();
	else if (GetMouseWheelMoveV().y > 0 || IsKeyPressed(KEY_E))
		nextWeapon();
}

void Game::updateEnemies(float dt)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); )
	{
		if ((*enemy)->getHealth() <= 0)
		{
			++killCounter;

			enemy = enemies.erase(enemy);
			continue;
		}

		std::vector<Bullet> bullets = (*enemy)->update(gameState, player, dt);
		enemyMadeBullets.insert(enemyMadeBullets.end(), bullets.begin(), bullets.end());

		if (!player.isDead() && (*enemy)->collidesWithPlayer(player))
			player.hit(gameState, *enemy);

		++enemy;
	}

	for (auto enemy1 = enemies.begin(); enemy1 != enemies.end(); ++enemy1)
	{
		auto enemy2 = enemy1;
		++enemy2;

		for (; enemy2 != enemies.end(); ++enemy2)
		{
			Vec2 rel = (*enemy2)->getPos() - (*enemy1)->getPos();
			float dist = rel.mag();

			if (dist == 0.0f)
				continue;

			float minDist = (*enemy1)->getSize() / 2.0f + (*enemy2)->getSize() / 2.0f;
			float depth = minDist - dist;

			if (depth <= 0.0f)
				continue;

			(*enemy1)->setPos((*enemy1)->getPos() - rel.norm() * depth / 2);
			(*enemy2)->setPos((*enemy2)->getPos() + rel.norm() * depth / 2);
		}
	}
}

void Game::spawnEnemies()
{
	if (gameState.time - timeOfLastEnemySpawn < enemySpawnDelay)
		return;

	float theta = randF() * 2.0f * PI;
	float radius = gameState.borderRadius + 100.0f;
	Vec2 pos = Vec2(radius * cos(theta), radius * sin(theta));

	enemies.push_back(new TestEnemy(pos, testEnemyTex));

	++enemySpawnCounter;
	timeOfLastEnemySpawn = gameState.time;
}

void Game::updatePlayerMadeBullets(float dt)
{
	for (auto bullet = playerMadeBullets.begin(); bullet != playerMadeBullets.end(); )
	{
		if (bullet->getPierceLimit() < 1)
		{
			bullet = playerMadeBullets.erase(bullet);
			continue;
		}

		if (bullet->getPos().sqrMag() > gameState.borderRadius * gameState.borderRadius)
		{
			bullet = playerMadeBullets.erase(bullet);
			continue;
		}

		bullet->update(enemies, player, dt);

		for (auto enemy : enemies)
			if (!bullet->alreadyHit(enemy) && enemy->collidesWithBullet(*bullet))
			{
				bullet->addHitEnemy(enemy);
				enemy->hit(gameState, *bullet);
				break;
			}

		++bullet;
	}
}

void Game::updateEnemyMadeBullets(float dt)
{
	for (auto bullet = enemyMadeBullets.begin(); bullet != enemyMadeBullets.end(); )
	{
		if (bullet->getPos().sqrMag() > gameState.borderRadius * gameState.borderRadius)
		{
			bullet = enemyMadeBullets.erase(bullet);
			continue;
		}

		bullet->update(enemies, player, dt);

		if (player.collidesWithBullet(*bullet))
		{
			player.hit(gameState, *bullet);
			bullet = enemyMadeBullets.erase(bullet);
			continue;
		}

		++bullet;
	}
}

void Game::draw() const
{
	BeginDrawing();

	ClearBackground(BLACK);
	drawBg();

	for (auto enemy : enemies)
		enemy->draw(gameState, camera);

	player.draw(gameState, camera);

	for (auto& bullet : playerMadeBullets)
		bullet.draw(gameState, camera);
	for (auto& bullet : enemyMadeBullets)
		bullet.draw(gameState, camera);

	drawBorder();

	drawWeaponUI();
	drawHealthUI();

	DrawTextureEx(skullTex, { 10, 10 }, 0, 2.0f, getSkullTint());

	DrawText(std::to_string(killCounter).c_str(), 80, 20, 50, RED);

	if (player.isDead())
		DrawText("YOU DIED", (int)(gameState.getScreenCenter().x - MeasureText("YOU DIED", 100) / 2.0f), (int)gameState.getScreenCenter().y - 50, 100, RED);

	EndDrawing();
}

void Game::drawBg() const
{
	const Rectangle sourceRec =
	{
			camera.getPos().x / bgScale / parallax - gameState.getScreenCenter().x / bgScale / camera.getZoom(),
			camera.getPos().y / bgScale / parallax - gameState.getScreenCenter().y / bgScale / camera.getZoom(),
			gameState.screenWidth / bgScale / camera.getZoom(), gameState.screenHeight / bgScale / camera.getZoom()
	};
	const Rectangle destRec = { 0, 0, (float)gameState.screenWidth, (float)gameState.screenHeight };

	DrawTexturePro(bgTex, sourceRec, destRec, { 0, 0 }, 0, WHITE);
}

void Game::drawBorder() const
{
	const Rectangle sourceRec =
	{
		0, 0,
		(float)borderTex.width, (float)borderTex.height
	};

	Vec2 topLeftScreenPos = camera.getScreenPos(gameState, Vec2(-gameState.borderRadius, -gameState.borderRadius));

	const Rectangle destRec =
	{
		topLeftScreenPos.x, topLeftScreenPos.y,
		2 * gameState.borderRadius * camera.getZoom(), 2 * gameState.borderRadius * camera.getZoom()
	};

	DrawTexturePro(borderTex, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
}

void Game::drawWeaponUI() const
{
	drawWeaponDock();
	drawFireBar();
	drawUIBullet();

	DrawText(getCurrentWeapon().name, 260, gameState.screenHeight - 57, 50, WHITE);
	DrawText(std::to_string(getCurrentWeapon().getAmmo()).c_str(), 815, gameState.screenHeight - 57, 50, WHITE);
}

void Game::drawWeaponDock() const
{
	DrawTexture(weaponDockTex, 0, gameState.screenHeight - weaponDockTex.height, WHITE);
}

void Game::drawFireBar() const
{
	const float fireTimeRatio = getCurrentWeapon().getFireTimeRatio(gameState);

	const Rectangle sourceRec =
	{
		0, 0,
		(float)fireBarTex.width, (float)fireBarTex.height
	};
	const Rectangle destRec =
	{
		10, (float)gameState.screenHeight - 10,
		(float)fireBarTex.width, 129 * fireTimeRatio
	};

	DrawTexturePro(fireBarTex, sourceRec, destRec, { 0, destRec.height }, 0, fireTimeRatio < 1.0f ? BLUE : WHITE);
}

void Game::drawUIBullet() const
{
	const Texture2D& bulletTex = getCurrentWeapon().getBullet().getTex();

	const Rectangle sourceRec =
	{
		0, 0,
		(float)bulletTex.width, (float)bulletTex.height
	};

	const Rectangle destRec =
	{
		41, (float)gameState.screenHeight - 137,
		127, 127
	};

	DrawTexturePro(bulletTex, sourceRec, destRec, { 0, 0 }, 0, WHITE);
}

void Game::drawHealthUI() const
{
	drawHealthDock();
	player.drawHealthUI(gameState);
}

void Game::drawHealthDock() const
{
	DrawTexture(healthDockTex, gameState.screenWidth - healthDockTex.width, healthUIY, WHITE);
}

Color Game::getSkullTint() const
{
	unsigned char whiteness = (unsigned char)(255 - clamp(killCounter / 50.0f * 255, 0.0f, 255.0f));
	return { 255, whiteness, whiteness, 255 };
}
