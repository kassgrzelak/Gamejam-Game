#include "Game.hpp"

static constexpr float bgScale = 2.0f;
static constexpr float parallax = 1.2f;

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

	testSound = LoadSound("assets/sfx/testSound.wav");

	testBulletTex = LoadTexture("assets/sprites/dog.png");

	weapons.push_back(Weapon("test weapon 1", 100, 1000, 0.1, 1, 0.0f,
		Bullet(Vec2::zero(), Vec2::zero(), 10, 100, 1, 0.5f, testBulletTex), testSound));

	weapons.push_back(Weapon("test weapon 2", 100, 1500, 0.8, 5, 0.4f,
		Bullet(Vec2::zero(), Vec2::zero(), 3, 500, 3, 0.5f, testBulletTex), testSound));

	weapons.push_back(Weapon("test weapon 3", 100, 5000, 1.4, 1, 0.0f,
		Bullet(Vec2::zero(), Vec2::zero(), 50, 1000, 5, 0.5f, testBulletTex), testSound));

	testEnemyTex = LoadTexture("assets/sprites/dog.png");

	enemies.push_back(new TestEnemy(Vec2(1000, 0), testEnemyTex));

	loadLevel("purp", 2000.0f);
}

void Game::run()
{
	while (!WindowShouldClose())
	{
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

	if (!player.isDead())
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
			enemy = enemies.erase(enemy);
			continue;
		}

		(*enemy)->update(gameState, player, dt);

		if (!player.isDead() && (*enemy)->collidesWithPlayer(player))
			player.hit(gameState, *enemy);

		++enemy;
	}
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
		bullet->update(enemies, player, dt);

		if (bullet->getPos().sqrMag() > gameState.borderRadius * gameState.borderRadius)
			bullet = enemyMadeBullets.erase(bullet);
		else
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

	if (player.isDead())
		DrawText("YOU DIED", gameState.getScreenCenter().x - MeasureText("YOU DIED", 100) / 2, gameState.getScreenCenter().y - 50, 100, RED);

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
