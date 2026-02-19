#ifndef BANE_GAME_SNOWWORM
#define BANE_GAME_SNOWWORM

#include "bane/primitives/animatedModel.hpp"
#include "bane/primitives/transform.hpp"
#include "game/data.hpp"
#include <memory>

class SnowWorm {
public:
  SnowWorm(std::string path, std::unique_ptr<GameData> &gameData,
           std::string unique_Name, int unique_id);
  std::unique_ptr<AnimatedModel> model;
  Transform transform;
  void Render(std::unique_ptr<GameData> &gameData);
  void renderShadow(std::unique_ptr<GameData> &gameData);
};

#endif
