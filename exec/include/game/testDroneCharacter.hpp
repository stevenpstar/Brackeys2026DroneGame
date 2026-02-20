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
  bool isLaserActive = false;
  bool isScannerActive = false;
  bool isDetectorActive = false;
  float scanTimer = 0.f;
  float scanTime = 100.f;
  Transform transform;
  int laserColliderIndex = -1;
  int scannerColliderIndex = -1;
  bool techPiece1Collected = false;
  bool techPiece2Collected = false;
  bool techPiece3Collected = false;
  bool techPiece4Collected = false;
  void Render(std::unique_ptr<GameData> &gameData);
  void RenderColour(std::unique_ptr<GameData> &gameData);
  void renderDroneShadow(std::unique_ptr<GameData> &gameData);

private:
};

#endif
