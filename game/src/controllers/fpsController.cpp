#include "SDL3/SDL_mouse.h"
#include "bane/bane.hpp"
#include "game/data.hpp"
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_stdinc.h>
#include <bane/components/camera.hpp>
#include <game/controllers/fpsController.hpp>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

FpsController::FpsController() { SetCursorOnOff(false); }

void FpsController::processInput(__attribute__((unused)) SDL_Window *window,
                                 Uint32 button, bool pressed,
                                 std::unique_ptr<GameData> &gameData) {

  if (disabled) {
    return;
  }

  if (button == SDLK_W && pressed) {
    std::cout << "Yeah we're going forward!\n";
    forwardDown = true;
  } else if (button == SDLK_W && !pressed) {
    forwardDown = false;
  }

  if (button == SDLK_S && pressed) {
    backDown = true;
  } else if (button == SDLK_S && !pressed) {
    backDown = false;
  }

  if (button == SDLK_A && pressed) {
    leftDown = true;
  } else if (button == SDLK_A && !pressed) {
    leftDown = false;
  }

  if (button == SDLK_D && pressed) {
    rightDown = true;
  } else if (button == SDLK_D && !pressed) {
    rightDown = false;
  }

  if (button == SDLK_UP && pressed) {

  } else if (button == SDLK_DOWN && pressed) {
  }
}

void FpsController::processMouseInput(SDL_Window *, int button, bool pressed,
                                      std::unique_ptr<GameData> &gameData) {
  if (button == SDL_BUTTON_RIGHT && pressed) {

  } else if (button == SDL_BUTTON_RIGHT && !pressed) {
  }

  if (button == SDL_BUTTON_MIDDLE && pressed) {

  } else if (button == SDL_BUTTON_MIDDLE && !pressed) {
  }
}

void FpsController::processMouse(SDL_Window *, double xpos, double ypos,
                                 std::unique_ptr<GameData> &gameData) {
  return;
  const float sensitivity = 0.08f;

  yaw += (xpos * sensitivity);
  pitch += (-ypos * sensitivity);

  if (pitch > 89.f) {
    pitch = 89.f;
  } else if (pitch < -89.f) {
    pitch = -89.f;
  }

  glm::vec3 dir;
  dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  dir.y = sin(glm::radians(pitch));
  dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  gameData->resources->camera.setCameraDirection(glm::normalize(dir));
  // std::cout << "camera direction: " +
  //                  glm::to_string(gameData->resources->camera.getDirection())
  //           << "\n";
}

void FpsController::update(float deltaTime,
                           std::unique_ptr<GameData> &gameData) {
  if (disabled) {
    return;
  }
  float speed = 0.02f;
  float forward = 0.f;
  float left = 0.f;
  if (forwardDown) {
    forward += 1.f;
  }
  if (backDown) {
    forward -= 1.f;
  }
  if (leftDown) {
    left += 1.f;
  }
  if (rightDown) {
    left -= 1.f;
  }
  moveVector = glm::vec3(0.f, 0.f, 0.f);
  moveVector += gameData->resources->camera.getDirection() * forward * speed;
  moveVector += gameData->resources->camera.getCameraRight() * left * speed;
  moveVector.y = 0.f;
  gameData->resources->camera.setPosition(
      gameData->resources->camera.getPosition() + moveVector * cameraSpeed);
  gameData->resources->camera.lookAtTarget(
      gameData->resources->camera.getPosition() +
      gameData->resources->camera.getDirection());
}

void FpsController::Enable() { disabled = false; }
void FpsController::Disable() { disabled = true; }
