#include "GameState.hpp"

Vec2 GameState::getScreenCenter() const
{
    return Vec2((float)screenWidth, (float)screenHeight) / 2.0f;
}
