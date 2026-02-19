#ifndef BANE_GAME_RENDER
#define BANE_GAME_RENDER

#include <bane/utility/shader.hpp>
#include <game/data.hpp>
#include <memory>
void Render(std::unique_ptr<GameData> &gameData, float dt);

#endif
