#ifndef COLD_CASE_FPS_CONTROLLER
#define COLD_CASE_FPS_CONTROLLER
#include <bane/controllers/icontroller.hpp>
#include <game/data.hpp>
#include <glm/glm.hpp>

class Camera;
class FpsController {
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
  float cameraSpeed = 1.f;

public:
  virtual void processInput(SDL_Window *window, Uint32 button, bool pressed,
                            std::unique_ptr<GameData> &gameData);
  virtual void update(float deltaTime, std::unique_ptr<GameData> &gameData);
  virtual void processMouse(SDL_Window *window, double xpos, double ypos,
                            std::unique_ptr<GameData> &gameData);
  virtual void processMouseInput(SDL_Window *window, int button, bool pressed,
                                 std::unique_ptr<GameData> &gameData);
  virtual void Enable();
  virtual void Disable();
  FpsController();
};
#endif
