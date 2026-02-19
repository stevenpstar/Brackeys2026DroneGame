#ifndef BANE_TEST_CHAR_CONTROLLER
#define BANE_TEST_CHAR_CONTROLLER
#include "SDL3/SDL_stdinc.h"
#include "bane/controllers/spectatorController.hpp"
#include "bane/primitives/animatedModel.hpp"
#include "bane/utility/AABB.hpp"
#include <bane/controllers/icontroller.hpp>
struct SDL_Window;
class Camera;
class Character;

class TestCharController : public IController
{
  private:
    Camera* camera;
    Character* character;
    bool forwardDown = false;
    bool rightDown = false;
    bool leftDown = false;
    bool backDown = false;
    bool disabled = false;
    float movementSpeed = 1.f;
    float m_lastX;
    float m_lastY;
    float pitch;
    float yaw;
    bool initialising = true;

  public:
    TestCharController(Camera* camera);

    std::vector<AABB>* collisionBoxes;

    void setMovementSpeed(float speed);
    void setCharacter(Character* chr);
    void setCameraPosition(glm::vec3 pos);

    virtual void processInput(SDL_Window* window, Uint32 button, bool pressed) override;
    virtual void processMouse(SDL_Window* window, double xpos, double ypos) override;
    virtual void update(float deltaTime) override;
    virtual void Enable() override;
    virtual void Disable() override;
};
#endif
