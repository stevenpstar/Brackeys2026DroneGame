#ifndef BANE_SPEC_CONTROLLER
#define BANE_SPEC_CONTROLLER
#include "SDL3/SDL_stdinc.h"
#include <bane/controllers/icontroller.hpp>
#include <glm/glm.hpp>
struct SDL_Window;
class Camera;
class Character;
class SpectatorController : public IController {
private:
  Camera *camera;
  bool forwardDown = false;
  bool rightDown = false;
  bool leftDown = false;
  bool backDown = false;
  float cameraSpeed = 2.5f;
  glm::vec3 moveVector = glm::vec3(0.f, 0.f, 0.f);
  float m_lastX;
  float m_lastY;
  float pitch;
  float yaw;
  bool initialising = true;
  bool disabled = false;

public:
  SpectatorController(Camera *camera);
  Character *character;
  bool captured = false;
  virtual void processInput(SDL_Window *window, Uint32 button, bool pressed) override;
  virtual void processMouse(SDL_Window *window, double xpos, double ypos) override;
  virtual void processMouseInput(SDL_Window *window, int button, bool pressed) override;
  virtual void update(float deltaTime) override;

  void centerMousePosition(int width, int height);
  virtual void Enable() override;
  virtual void Disable() override;
};
#endif
