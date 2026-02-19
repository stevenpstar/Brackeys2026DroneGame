#ifndef BANE_GAME_LOGIC
#define BANE_GAME_LOGIC

#include <bane/utility/shader.hpp>
#include <game/data.hpp>
#include <memory>
void Update(std::unique_ptr<GameData> &gameData, float dt);

#endif
