#include "Game.hpp"

void Game::init()
{
	InitWindow(0, 0, "Game");
	ToggleBorderlessWindowed();
	SetWindowFocused();

	const int monitor = GetCurrentMonitor();

	gameState.screenWidth = GetMonitorWidth(monitor);
	gameState.screenHeight = GetMonitorHeight(monitor);

	SetWindowSize(gameState.screenWidth, gameState.screenHeight);

	player.init({ static_cast<float>(gameState.screenWidth / 2), static_cast<float>(gameState.screenHeight / 2) });
}

void Game::run()
{
	while (!WindowShouldClose())
	{
		if (!IsWindowFocused())
			SetWindowFocused();

		update();
		draw();
	}
}

void Game::end()
{
	CloseWindow();
}

void Game::update()
{
	const float dt = GetFrameTime();

	player.update(gameState, dt);
}

void Game::draw()
{
	BeginDrawing();

	ClearBackground(BLACK);

	player.draw();

	if (gameState.showFPS)
		DrawFPS(5, 5);

	EndDrawing();
}