#ifndef BRACKEYS_DRONE_CHARACTER
#define BRACKEYS_DRONE_CHARACTER

#include "bane/primitives/animatedModel.hpp"
#include "bane/primitives/transform.hpp"
#include "game/data.hpp"
#include <memory>
class DroneCharacter {
public:
  DroneCharacter(std::string path, std::unique_ptr<GameData> &gameData);
  std::unique_ptr<AnimatedModel> droneModel;
  float batteryPercentage = 100.f;
  Transform transform;
  void Render(std::unique_ptr<GameData> &gameData);
  void RenderColour(std::unique_ptr<GameData> &gameData);
  void renderDroneShadow(std::unique_ptr<GameData> &gameData);

private:
};

#endif
