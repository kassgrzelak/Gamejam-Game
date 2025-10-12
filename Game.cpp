#include "Game.hpp"

static constexpr float bgScale = 2.0f;
static constexpr float parallax = 1.2f;

void Game::init()
{
	initRand();

	InitWindow(1920, 1080, "Gamejam Game");
	ToggleBorderlessWindowed();

	const int monitor = GetCurrentMonitor();

	gameState.screenWidth = GetMonitorWidth(monitor);
	gameState.screenHeight = GetMonitorHeight(monitor);

	SetWindowSize(gameState.screenWidth, gameState.screenHeight);

	player.init();
	camera.init();

	borderTex = LoadTexture("assets/border.png");

	testEnemyTex = LoadTexture("assets/dog.png");

	enemies.push_back(new TestEnemy(Vec2(100, 0), testEnemyTex));

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
	std::string path = "assets/" + bgType + std::to_string(num) + ".png";

	bgTex = LoadTexture(path.c_str());
}

void Game::update()
{
	const float dt = GetFrameTime();

	player.update(gameState, dt);
	updateEnemies(dt);
	camera.update(player.getPos(), player.getVel(), player.getVelRatio(), dt);
}

void Game::updateEnemies(float dt)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); )
	{
		(*enemy)->update(gameState, player, dt);

		++enemy;
	}
}

void Game::draw() const
{
	BeginDrawing();

	ClearBackground(BLACK);
	drawBg();

	for (auto enemy : enemies)
		enemy->draw(camera, gameState);

	player.draw(gameState, camera);

	drawBorder();

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
