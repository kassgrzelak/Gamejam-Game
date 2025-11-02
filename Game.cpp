#include "Game.hpp"
#include <string>

static constexpr float bgScale = 2.0f;
static constexpr float parallax = 1.2f;

static constexpr int startupFrames = 22 * 60;

static double enemySpawnDelay = 10.0f;

static constexpr int maxKills = 25;

void Game::init()
{
	initRand();

	InitWindow(0, 0, "Gamejam Game");
	InitAudioDevice();
	ToggleBorderlessWindowed();
	DisableCursor();
	SetTargetFPS(60);
	SetWindowState(FLAG_VSYNC_HINT);

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

	plasmaRifleSound = LoadSound("assets/sfx/plasmaRifle.wav");
	laserShotgunSound = LoadSound("assets/sfx/laserShotgun.wav");
	laserSniperSound = LoadSound("assets/sfx/laserSniper.wav");

	pickupSound = LoadSound("assets/sfx/pickupSound.wav");

	bgMusic = LoadMusicStream("assets/sfx/Cosmos.wav");
	voices1 = LoadSound("assets/sfx/Cosmos Voices 1.wav");
	voices2 = LoadSound("assets/sfx/Cosmos Voices 2.wav");
	voices3 = LoadSound("assets/sfx/Cosmos Voices 3.wav");
	voices4 = LoadSound("assets/sfx/Cosmos Voices 4.wav");

	healthPickupTex = LoadTexture("assets/sprites/healthPickup.png");
	ammo1PickupTex = LoadTexture("assets/sprites/ammoPickup1.png");
	ammo2PickupTex = LoadTexture("assets/sprites/ammoPickup2.png");

	orbTex = LoadTexture("assets/sprites/orb.png");
	laserRedTex = LoadTexture("assets/sprites/laserRed.png");
	laserGreenTex = LoadTexture("assets/sprites/laserGreen.png");

	weapons.push_back(Weapon("Plasma Rifle", 999, 1600, 0.1, 1, 0.0f,
		Bullet(Vec2::zero(), Vec2::zero(), 15, 300, 1, 0.8f, orbTex), plasmaRifleSound));

	weapons.push_back(Weapon("Laser Shotgun", 10, 1600, 0.8, 5, 0.3f,
		Bullet(Vec2::zero(), Vec2::zero(), 10, 1000, 3, 1.5f, laserRedTex), laserShotgunSound));

	weapons.push_back(Weapon("Laser Sniper", 5, 5000, 1.4, 1, 0.0f,
		Bullet(Vec2::zero(), Vec2::zero(), 50, 2000, 5, 2.5f, laserGreenTex), laserSniperSound));

	waspTex = LoadTexture("assets/sprites/enemy1.png");
	flyTex = LoadTexture("assets/sprites/enemy2.png");
	bigFlyTex = LoadTexture("assets/sprites/enemy3.png");

	PlayMusicStream(bgMusic);

	loadLevel("blue", 3000.0f);
}

void Game::run()
{
	while (!WindowShouldClose())
	{
		gameState.time = GetTime();
		UpdateMusicStream(bgMusic);

		if (!IsMusicStreamPlaying(bgMusic))
		{
			StopMusicStream(bgMusic);
			PlayMusicStream(bgMusic);
		}

		if (startupFrameCounter < startupFrames)
		{
			++startupFrameCounter;
			SetWindowFocused();

			if (startupFrameCounter == startupFrames)
				timeOfStart = gameState.time;
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
		loadBg("blue", 3);
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
	const float dt = GetFrameTime();

	keyInput();

	spawnEnemies();

	player.update(gameState, dt);

	checkPickupCollision(dt);
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

void Game::checkPickupCollision(float dt)
{
	for (auto pickup = pickups.begin(); pickup != pickups.end(); )
	{
		if (player.collidesWithPickup(*pickup))
		{
			if (pickup->type == PickupType::Health)
				player.healthPickup();
			else if (pickup->type == PickupType::Ammo1)
				weapons[1].ammoPickup();
			else
				weapons[2].ammoPickup(); // I'M SORRY OKAY I'M ON A DEADLINE HERE

			PlaySound(pickupSound);

			pickup = pickups.erase(pickup);
			continue;
		}

		++pickup;
	}
}

void Game::updateEnemies(float dt)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); )
	{
		if ((*enemy)->getHealth() <= 0)
		{
			++killCounter;

			float random = randF();
			float healthChance = 0.1f + clamp((1 - 2.0f * player.getHealthRatio()) * 0.3f, 0.0f, 0.2f);

			if (random < healthChance)
				pickups.emplace_back((*enemy)->getPos(), PickupType::Health, healthPickupTex);
			else if (random < healthChance + 0.1f)
				pickups.emplace_back((*enemy)->getPos(), PickupType::Ammo1, ammo1PickupTex);
			else if (random < healthChance + 0.2f)
				pickups.emplace_back((*enemy)->getPos(), PickupType::Ammo2, ammo2PickupTex);

			if (killCounter == 1)
				playVoices();
			else if (randF() < 0.05f * (1 + 5.0f * clamp(killCounter / (float)maxKills, 0.0f, 1.0f)))
				playVoices();

			if (killCounter == maxKills)
				timeOfFinish = gameState.time;

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

void Game::playVoices() const
{
	if (IsSoundPlaying(voices1) || IsSoundPlaying(voices2) || IsSoundPlaying(voices3) || IsSoundPlaying(voices4))
		return;

	int num = randInt(1, 4);

	if (num == 1)
		PlaySound(voices1);
	else if (num == 2)
		PlaySound(voices2);
	else if (num == 3)
		PlaySound(voices3);
	else if (num == 4)
		PlaySound(voices4);
}

void Game::spawnEnemies()
{
	if (gameState.time - timeOfLastEnemySpawn < enemySpawnDelay || killCounter + enemies.size() >= maxKills)
		return;

	float theta = randF() * 2.0f * PI;
	float radius = gameState.borderRadius + 100.0f;
	Vec2 pos = Vec2(radius * cos(theta), radius * sin(theta));

	float random = randF();

	if (random < 0.6f)
		enemies.push_back(new Wasp(pos, waspTex));
	else if (random < 0.9f)
		enemies.push_back(new Fly(pos, flyTex));
	else
		enemies.push_back(new BigFly(pos, bigFlyTex));

	++enemySpawnCounter;
	timeOfLastEnemySpawn = gameState.time;
	enemySpawnDelay = clamp(enemySpawnDelay * 0.95, 3.0, 999.0);
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

	for (const auto& pickup : pickups)
		pickup.draw(gameState, camera);

	for (auto enemy : enemies)
		enemy->draw(gameState, camera);

	player.draw(gameState, camera);

	for (const auto& bullet : playerMadeBullets)
		bullet.draw(gameState, camera);
	for (const auto& bullet : enemyMadeBullets)
		bullet.draw(gameState, camera);

	drawBorder();

	drawWeaponUI();
	drawHealthUI();

	DrawTextureEx(skullTex, { 10, 10 }, 0, 2.0f, getSkullTint());

	DrawText((std::to_string(killCounter) + "/" + std::to_string(maxKills)).c_str(), 80, 20, 50, getSkullTint());

	if (player.isDead())
		DrawText("MISSION FAILED", (int)(gameState.getScreenCenter().x - MeasureText("MISSION FAILED", 100) / 2.0f), (int)gameState.getScreenCenter().y - 50, 100, RED);
	
	if (killCounter >= maxKills)
	{
		const double t = gameState.time - timeOfFinish;
		double playTime = gameState.time;

		unsigned char transparency1 = (unsigned char)clamp(t / 3 * 255, 0.0, 255.0);
		unsigned char transparency2 = (unsigned char)clamp((t - 2) / 3 * 255, 0.0, 255.0);
		unsigned char transparency3 = (unsigned char)clamp((t - 5) / 3 * 255, 0.0, 255.0);
		unsigned char transparency4 = (unsigned char)clamp((t - 8) / 3 * 255, 0.0, 255.0);

		std::string minutes = std::to_string((int)playTime / 60);
		std::string seconds = std::to_string((int)playTime % 60);

		if (minutes.length() == 1)
			minutes = "0" + minutes;
		if (seconds.length() == 1)
			seconds = "0" + seconds;

		DrawText(("Date: 42nd Archeon, 2621, T+" + minutes + ":" + seconds + " MT").c_str(), 50, 100, 50, {152, 241, 255, transparency1});
		DrawText("Location: Unmapped outskirts of Orpheus Cluster", 50, 150, 50, { 152, 241, 255, transparency2 });
		DrawText("The unforeseen alien threat has been eliminated. You will be regarded\nat home as a hero.", 50, 250, 50, { 152, 241, 255, transparency3 });
		DrawText("Glory to The Leader.", 50, 400, 50, { 255, 20, 20, transparency4 });

		playVoices();
	}

	if (startupFrameCounter < startupFrames)
	{
		unsigned char transparency1;
		unsigned char transparency2;
		unsigned char transparency3;
		unsigned char transparency4;
		unsigned char transparency5;
		unsigned char transparency6;

		if (startupFrameCounter < 19 * 60)
		{
			transparency1 = (unsigned char)(clamp(startupFrameCounter / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency2 = (unsigned char)(clamp((startupFrameCounter - 2 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency3 = (unsigned char)(clamp((startupFrameCounter - 5 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency4 = (unsigned char)(clamp((startupFrameCounter - 8 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency5 = (unsigned char)(clamp((startupFrameCounter - 12 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency6 = (unsigned char)(clamp((startupFrameCounter - 15 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
		}
		else
		{
			transparency1 = 255 - (unsigned char)(clamp((startupFrameCounter - 19 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency2 = 255 - (unsigned char)(clamp((startupFrameCounter - 19 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency3 = 255 - (unsigned char)(clamp((startupFrameCounter - 19 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency4 = 255 - (unsigned char)(clamp((startupFrameCounter - 19 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency5 = 255 - (unsigned char)(clamp((startupFrameCounter - 19 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
			transparency6 = 255 - (unsigned char)(clamp((startupFrameCounter - 19 * 60) / (60.0f * 3) * 255, 0.0f, 255.0f));
		}

		std::string seconds = std::to_string((int)gameState.time);
		if (seconds.length() == 1)
			seconds = "0" + seconds;

		DrawText(("Date: 42nd Archeon, 2621, T+00:" + seconds + " MT").c_str(), 50, 100, 50, {152, 241, 255, transparency1});
		DrawText("Location: Unmapped outskirts of Orpheus Cluster", 50, 150, 50, { 152, 241, 255, transparency2 });
		DrawText("The Earthen Federation is concerned with growing overpopulation\nin the homeworlds of Earth, Mars, and Venus.", 50, 250, 50, { 152, 241, 255, transparency3 });
		DrawText("The remaining planets in Earth's solar system would be costly to\nterraform, so you have been sent on a scouting mission to investigate\npotentially more habitable worlds in a nearby solar system.", 50, 350, 50, { 152, 241, 255, transparency4 });
		DrawText("You are an invader.", 50, 550, 50, { 255, 20, 20, transparency5 });
		DrawText("Glory to The Leader.", 50, 650, 50, { 152, 241, 255, transparency6 });

	}

	EndDrawing();
}

void Game::drawBg() const
{
	const Rectangle sourceRec =
	{
			(int)(camera.getPos().x / bgScale / parallax - gameState.getScreenCenter().x / bgScale / camera.getZoom()),
			(int)(camera.getPos().y / bgScale / parallax - gameState.getScreenCenter().y / bgScale / camera.getZoom()),
			(int)(gameState.screenWidth / bgScale / camera.getZoom()), (int)(gameState.screenHeight / bgScale / camera.getZoom())
	};
	const Rectangle destRec = { 0, 0, (int)gameState.screenWidth, (int)gameState.screenHeight };

	DrawTexturePro(bgTex, sourceRec, destRec, { 0, 0 }, 0, WHITE);
}

void Game::drawBorder() const
{
	const Rectangle sourceRec =
	{
		0, 0,
		(float)borderTex.width, (float)borderTex.height
	};

	Vec2 topLeftScreenPos = camera.getScreenPos(gameState, Vec2(-(gameState.borderRadius + 30.0f), -(gameState.borderRadius + 30.0f)));

	const Rectangle destRec =
	{
		(int)topLeftScreenPos.x, (int)topLeftScreenPos.y,
		2 * (gameState.borderRadius + 30.0f) * camera.getZoom(), 2 * (gameState.borderRadius + 30.0f) * camera.getZoom()
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
	unsigned char whiteness = (unsigned char)(255 - clamp(killCounter / (float)maxKills * 255, 0.0f, 255.0f));
	return { 255, whiteness, whiteness, 255 };
}
