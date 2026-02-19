#ifndef BANE_ICONTROLLER
#define BANE_ICONTROLLER

#include "SDL3/SDL_stdinc.h"
struct SDL_Window;
class IController {
public:
  virtual void processInput(SDL_Window *window, Uint32 button, bool pressed);
  virtual void update(float deltaTime);
  virtual void processMouse(SDL_Window *window, double xpos, double ypos);
  virtual void processMouseInput(SDL_Window *window, int button, bool pressed);
  virtual void Enable();
  virtual void Disable();
};
#endif
