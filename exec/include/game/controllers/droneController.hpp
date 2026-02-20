#ifndef BRACKEYS_DRONE_CONTROLLER
#define BRACKEYS_DRONE_CONTROLLER

#include "SDL3/SDL_stdinc.h"
#include "game/testDroneCharacter.hpp"
#include <game/data.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

class DroneController {
private:
  bool disabled = false;
  bool enabled = true;
  float pitch = 0.f;
  float yaw = 0.f;
  bool forwardDown = false;
  bool rightDown = false;
  bool leftDown = false;
  bool backDown = false;
  glm::vec3 moveVector = glm::vec3(0.f, 0.f, 0.f);
  glm::mat4 rotationDir = glm::mat4(1.f);
  float rotation_angle = 0.f;
  float cameraSpeed = 1.f;
  float forwardMovement = 0.f;
  float leftMovement = 0.f;
  float upMovement = 0.f;
  float rotationAmountX = 0.f;
  float rotationAmountY = 0.f;
  glm::vec3 velocity = glm::vec3(0.f, 0.f, 0.f);
  int colliderIndex = -1;
  glm::vec3 targetLocation = glm::vec3(0.f);
  bool autoMoving = false;
  bool inChargingArea = false;

public:
  float batteryCharge = 100.f;
  float batteryCapacity = 100.f;

public:
  glm::vec3 batteryColour = glm::vec3(0.f);
  glm::vec3 dirLightOffset = glm::vec3(0.f, 0.f, 0.f);
  glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float t);

  virtual void processInput(SDL_Window *window, Uint32 button, bool pressed,
                            std::unique_ptr<GameData> &gameData);
  virtual void processGamepadButtonInput(SDL_Window *window, std::string button,
                                         bool pressed,
                                         std::unique_ptr<GameData> &gameData);
  virtual void processGamepadJoystickInput(SDL_Window *window, std::string axis,
                                           std::string stick, float value);
  virtual void processGamepadTrigger(SDL_Window *window, std::string trigger,
                                     float value);

  virtual void update(float deltaTime, std::unique_ptr<GameData> &gameData);
  virtual void processMouse(SDL_Window *window, double xpos, double ypos,
                            std::unique_ptr<GameData> &gameData);
  virtual void processMouseInput(SDL_Window *window, int button, bool pressed,
                                 std::unique_ptr<GameData> &gameData);
  virtual void Enable();
  virtual void Disable();
  virtual void setColliderIndex(int id);
  DroneController();
  void SetCharacter(const char *path, std::unique_ptr<GameData> &gameData);
  void setSnowPixels(GLubyte *snowPixels, std::unique_ptr<GameData> &gameData,
                     float x_offset, float y_offset);
  std::unique_ptr<DroneCharacter> droneCharacter;
  void ActivateLaser(std::unique_ptr<GameData> &gameData);
};

#endif
