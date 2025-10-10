#include "Game.hpp"

void Game::init()
{
	InitWindow(screenWidth, screenHeight, "Game");
}

void Game::run()
{
	while (!WindowShouldClose())
	{
		draw();
	}
}

void Game::end()
{
	CloseWindow();
}

void Game::draw()
{
	BeginDrawing();

	if (showFPS)
		DrawFPS(5, 5);

	EndDrawing();
}