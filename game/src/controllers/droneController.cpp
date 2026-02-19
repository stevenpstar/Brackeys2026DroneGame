#include "game/controllers/droneController.hpp"
#include "SDL3/SDL_gamepad.h"
#include "SDL3/SDL_stdinc.h"
#include "bane/bane.hpp"
#include "bane/utility/AABB.hpp"
#include "game/testDroneCharacter.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

DroneController::DroneController() { SetCursorOnOff(true); }

void DroneController::SetCharacter(const char *path,
                                   std::unique_ptr<GameData> &gameData) {
  this->droneCharacter =
      std::make_unique<DroneCharacter>(DroneCharacter(path, gameData));
  this->droneCharacter->transform.position = glm::vec3(-15.f, 15.f, -10.f);
}

void DroneController::processGamepadButtonInput(
    SDL_Window *window, std::string button, bool pressed,
    std::unique_ptr<GameData> &gameData) {
  if (button == "a" && pressed) {
    if (inChargingArea) {
      autoMoving = !autoMoving;
      if (autoMoving) {
        velocity = glm::vec3(0.f, 0.f, 0.f);
      }
    }

  } else if (button == "a" && !pressed) {
    forwardDown = false;
  }
}

void DroneController::processGamepadTrigger(SDL_Window *window,
                                            std::string trigger, float value) {
  if (trigger == "left") {

    if (value < 9000 && value > -9000) {
      this->upMovement = 0.f;
    }
    this->upMovement = -(value / 32768.0);
  }
  if (trigger == "right") {
    if (value < 9000 && value > -9000) {
      this->upMovement = 0.f;
    }

    this->upMovement = (value / 32768.0);
  }
}

void DroneController::processGamepadJoystickInput(SDL_Window *window,
                                                  std::string axis,
                                                  std::string stick,
                                                  float value) {
  if (axis == "x" && stick == "left") {
    if (value < 9000 && value > -9000) {
      forwardMovement = 0.f;
      return;
    }
    this->forwardMovement = -(value / 32768.0);
  }
  if (axis == "y" && stick == "left") {
    if (value < 9000 && value > -9000) {
      leftMovement = 0.f;
      return;
    }
    this->leftMovement = (value / 32768.0);
  }
  if (axis == "x" && stick == "right") {
    if (value < 9000 && value > -9000) {
      rotationAmountX = 0.f;
      return;
    }
    this->rotationAmountX = (value / 32768.f);
  }
  if (axis == "y" && stick == "right") {
    if (value < 9000 && value > -9000) {
      rotationAmountY = 0.f;
      return;
    }
    this->rotationAmountY = (value / 32768.f);
  }
}

void DroneController::processInput(__attribute__((unused)) SDL_Window *window,
                                   Uint32 button, bool pressed,
                                   std::unique_ptr<GameData> &gameData) {

  if (disabled) {
    return;
  }

  if ((button == SDLK_W || button == SDL_GAMEPAD_BUTTON_LABEL_A) && pressed) {
    // forwardDown = true;
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

glm::vec3 DroneController::lerp(glm::vec3 x, glm::vec3 y, float t) {
  return x * (1.f - t) + y * t;
}

void DroneController::update(float deltaTime,
                             std::unique_ptr<GameData> &gameData) {
  if (!inChargingArea) {
    batteryCharge -= 1.f * deltaTime;
  }
  this->droneCharacter->batteryPercentage =
      (batteryCharge / batteryCapacity) * 100.f;
  //  std::cout << "battery charge: " << this->droneCharacter->batteryPercentage
  //            << "%\n";
  if (batteryCharge < 0.f) {
    batteryCharge = 0.f;
  }
  batteryColour = glm::vec3(0.f, 1.f, 0.f);
  if (this->droneCharacter->batteryPercentage < 50.f) {
    batteryColour = glm::vec3(0.5f, 0.5f, 0.f);
  }
  if (this->droneCharacter->batteryPercentage < 20.f) {
    batteryColour = glm::vec3(1.f, 0.f, 0.f);
  }

  this->droneCharacter->droneModel->emissiveColour = batteryColour;

  if (autoMoving) {

    this->droneCharacter->transform.position =
        lerp(this->droneCharacter->transform.position, targetLocation, 0.05f);
    this->droneCharacter->droneModel->transform =
        this->droneCharacter->transform;
    batteryCharge += 20.f * deltaTime;
    if (batteryCharge > batteryCapacity) {
      batteryCharge = batteryCapacity;
      autoMoving = false;
    }
    return;
  }
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

  this->droneCharacter->transform.rotation.y -= rotationAmountX;
  this->droneCharacter->transform.rotation.y -= rotationAmountY;
  if (this->droneCharacter->transform.rotation.y >= 360.f) {
    this->droneCharacter->transform.rotation.y -= 360.f;
  } else if (this->droneCharacter->transform.rotation.y < 0.f) {
    this->droneCharacter->transform.rotation.y += 360.f;
  }

  const float maxVelocity = 0.1f;
  const float decelerationRate = 60.f;
  const float accelerationRate = 0.3f;
  glm::vec3 forwardvec =
      glm::vec3(0.f, 0.f, -1.f); // glm::normalize(glm::vec3(rotation[0]));
  glm::vec3 leftvec =
      glm::vec3(1.f, 0.f, 0.f); // glm::normalize(glm::vec3(rotation[2]));
  glm::vec3 moveVector = glm::vec3(0.f, 0.f, 0.f);
  if (velocity.z > 0.f) {
    velocity.z -= (velocity.z / decelerationRate);
  } else if (velocity.z < 0.f) {
    velocity.z += (-velocity.z / decelerationRate);
  }

  if (velocity.x > 0.f) {
    velocity.x -= (velocity.x / decelerationRate);
  } else if (velocity.x < 0.f) {
    velocity.x += (-velocity.x / decelerationRate);
  }

  if (velocity.y > 0.f) {
    velocity.y -= (velocity.y / decelerationRate);
  } else if (velocity.y < 0.f) {
    velocity.y += (-velocity.y / decelerationRate);
  }

  velocity.z -= (forwardMovement * accelerationRate) * deltaTime;
  if (velocity.z > maxVelocity) {
    velocity.z = maxVelocity;
  } else if (velocity.z < -maxVelocity) {
    velocity.z = -maxVelocity;
  }
  velocity.x +=
      (leftMovement * accelerationRate) * deltaTime; // * (5.f) * deltaTime;
                                                     // // 10 is speed
  if (velocity.x > maxVelocity) {
    velocity.x = maxVelocity;
  } else if (velocity.x < -maxVelocity) {
    velocity.x = -maxVelocity;
  }

  velocity.y +=
      (upMovement * accelerationRate) * deltaTime; // * (5.f) * deltaTime;
                                                   // // 10 is speed
  if (velocity.y > maxVelocity) {
    velocity.y = maxVelocity;
  } else if (velocity.y < -maxVelocity) {
    velocity.y = -maxVelocity;
  }

  this->droneCharacter->transform.position += velocity;
  this->droneCharacter->droneModel->transform = this->droneCharacter->transform;
  if (this->colliderIndex >= 0) {
    gameData->resources->Colliders.at(this->colliderIndex)->origin =
        this->droneCharacter->transform.position;
  }
  glm::vec3 normalisedRotationAmount =
      glm::normalize(this->droneCharacter->transform.rotation);
  glm::mat4 droneRotation = glm::mat4(1.f);
  droneRotation =
      glm::rotate(droneRotation, this->droneCharacter->transform.rotation.x,
                  glm::vec3(1.f, 0.f, 0.f));
  float droneAngle = glm::angle(
      glm::normalize(this->droneCharacter->droneModel->transform.rotation),
      glm::vec3(1.f, 0.f, 0.f));

  gameData->resources->lightData.pointLights.at(0)->position =
      glm::vec3(this->droneCharacter->transform.position.x,
                this->droneCharacter->transform.position.y + 15.f,
                this->droneCharacter->transform.position.z);
  glm::vec3 lightColour = glm::vec3(0.1f, 0.1f, 0.3f);
  // lightColour.z += abs((velocity.z / (maxVelocity * 4.0)));
  // lightColour.z += abs((velocity.x / (maxVelocity * 4.0)));
  // lightColour.z += abs((velocity.y / (maxVelocity * 4.0)));
  gameData->resources->lightData.pointLights.at(0)->diffuse = lightColour;

  glm::vec3 desiredCameraPosition =
      glm::vec3(this->droneCharacter->transform.position.x,
                this->droneCharacter->transform.position.y + 12.f,
                this->droneCharacter->transform.position.z + 12.f);
  glm::vec3 lerpedCameraPosition = gameData->resources->camera.getPosition();
  lerpedCameraPosition =
      lerp(lerpedCameraPosition, desiredCameraPosition, 3.f * deltaTime);
  if (lerpedCameraPosition.y > 30.f) {
    lerpedCameraPosition.y = 30.f;
  }
  // if (lerpedCameraPosition.x < 0.f) {
  //   lerpedCameraPosition.x = 0.f;
  // }

  if (lerpedCameraPosition.z > 10.f) {

    lerpedCameraPosition.z = 10.f;
  }

  // gameData->resources->lightData.dirLight =
  //     this->droneCharacter->droneModel->transform.position + dirLightOffset;
  gameData->resources->camera.setPosition(lerpedCameraPosition);
  // gameData->resources->camera.setPosition(glm::vec3(0.f, 15.f, 10.f));
  gameData->resources->camera.setCameraDirection(glm::vec3(0.f, -0.5f, -0.5f));
  // gameData->resources->camera.setProjection(1920, 1080, 45.f);

  // CHECK COLLISIONS DO THIS IN A DIFFERENT FUNCTION OR CLASS AAHHHH
  inChargingArea = false;
  if (this->colliderIndex >= 0) {
    for (int i = 0; i < gameData->resources->Colliders.size(); ++i) {
      if (i == this->colliderIndex) {
        continue;
      }
      if (gameData->resources->Colliders.at(this->colliderIndex)
              ->isColliding(gameData->resources->Colliders.at(i))) {
        if (gameData->resources->Colliders.at(i)->tags.size() > 0) {
          if (gameData->resources->Colliders.at(i)->tags[0] ==
              "charging_station") {
            inChargingArea = true;
            targetLocation = gameData->resources->Colliders.at(i)->origin;
          }
        }
      }
    }
  }

  for (int i = 0; i < this->droneCharacter->droneModel->boneRotations.size();
       ++i) {
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName == "root") {
      //  this->droneCharacter->droneModel->boneRotations[i].Angle =
      //      -(forwardMovement * 45.f);
      this->droneCharacter->droneModel->boneRotations[i].Angle =
          -((velocity.z * 20.f) * 10.f);
    }
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "frontright" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.y > 0.5f) {
      //  this->droneCharacter->droneModel->boneRotations[i].Angle =
      //      -(forwardMovement * 45.f);
      this->droneCharacter->droneModel->boneRotations[i].Angle =
          -this->droneCharacter->transform.rotation.y;
    }
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "frontright" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.x > 0.5f) {
      float angle = ((velocity.z * 32.f) * 30.f);
      if (angle > 30.f) {
        angle = 30.f;
      } else if (angle < -30.f) {
        angle = -30.f;
      }
      this->droneCharacter->droneModel->boneRotations[i].Angle = angle;
    }
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "frontright" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.z > 0.5f) {

      float angle = -((velocity.x * 32.f) * 30.f);
      if (angle > 30.f) {
        angle = 30.f;
      } else if (angle < -30.f) {
        angle = -30.f;
      }

      this->droneCharacter->droneModel->boneRotations[i].Angle = angle;
    }

    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "backright" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.y > 0.5f) {
      //  this->droneCharacter->droneModel->boneRotations[i].Angle =
      //      -(forwardMovement * 45.f);

      this->droneCharacter->droneModel->boneRotations[i].Angle =
          -this->droneCharacter->transform.rotation.y;
    }
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "backright" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.x > 0.5f) {
      float angle = ((velocity.z * 32.f) * 30.f);
      if (angle > 30.f) {
        angle = 30.f;
      } else if (angle < -30.f) {
        angle = -30.f;
      }

      this->droneCharacter->droneModel->boneRotations[i].Angle = angle;
    }
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "backright" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.z > 0.5f) {

      float angle = -((velocity.x * 32.f) * 30.f);
      if (angle > 30.f) {
        angle = 30.f;
      } else if (angle < -30.f) {
        angle = -30.f;
      }

      this->droneCharacter->droneModel->boneRotations[i].Angle = angle;
    }

    // ---- this is the divide between right and left! ---

    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "frontleft" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.y > 0.5f) {
      //  this->droneCharacter->droneModel->boneRotations[i].Angle =
      //      -(forwardMovement * 45.f);
      this->droneCharacter->droneModel->boneRotations[i].Angle =
          -this->droneCharacter->transform.rotation.y;
    }
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "frontleft" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.x > 0.5f) {
      float angle = ((velocity.z * 32.f) * 30.f);
      if (angle > 30.f) {
        angle = 30.f;
      } else if (angle < -30.f) {
        angle = -30.f;
      }

      this->droneCharacter->droneModel->boneRotations[i].Angle = angle;
    }
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "frontleft" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.z > 0.5f) {
      float angle = -((velocity.x * 32.f) * 30.f);
      if (angle > 30.f) {
        angle = 30.f;
      } else if (angle < -30.f) {
        angle = -30.f;
      }

      this->droneCharacter->droneModel->boneRotations[i].Angle = angle;
    }

    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "backleft" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.y > 0.5f) {
      //  this->droneCharacter->droneModel->boneRotations[i].Angle =
      //      -(forwardMovement * 45.f);
      this->droneCharacter->droneModel->boneRotations[i].Angle =
          -this->droneCharacter->transform.rotation.y;
    }
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "backleft" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.x > 0.5f) {
      float angle = ((velocity.z * 32.f) * 30.f);
      if (angle > 30.f) {
        angle = 30.f;
      } else if (angle < -30.f) {
        angle = -30.f;
      }

      this->droneCharacter->droneModel->boneRotations[i].Angle = angle;
    }
    if (this->droneCharacter->droneModel->boneRotations[i].BoneName ==
            "backleft" &&
        this->droneCharacter->droneModel->boneRotations[i].Axis.z > 0.5f) {
      float angle = -((velocity.x * 32.f) * 30.f);
      if (angle > 30.f) {
        angle = 30.f;
      } else if (angle < -30.f) {
        angle = -30.f;
      }

      this->droneCharacter->droneModel->boneRotations[i].Angle = angle;
    }
  }
}
void DroneController::processMouse(SDL_Window *window, double xpos, double ypos,
                                   std::unique_ptr<GameData> &gameData) {}

void DroneController::processMouseInput(SDL_Window *window, int button,
                                        bool pressed,
                                        std::unique_ptr<GameData> &gameData) {}

void DroneController::setColliderIndex(int id) { colliderIndex = id; }

void DroneController::setSnowPixels(GLubyte *pixels,
                                    std::unique_ptr<GameData> &gameData) {

  GLubyte *snowPixels = new GLubyte[128 * 128 * 4];
  const float coordinateConversion = 128.0 / 50.0;
  int x = floor((this->droneCharacter->transform.position.x + 25.f) *
                coordinateConversion);
  int y = floor((this->droneCharacter->transform.position.z + 25.f) *
                coordinateConversion);

  // x = floor((33) * coordinateConversion);
  // y = floor(11 * coordinateConversion);

  if (x < 0 || x > 127) {
    return;
  }
  if (y < 0 || y > 127) {
    return;
  }

  for (int i = 0; i < gameData->resources->Models.size(); ++i) {
    if (gameData->resources->Models.at(i)->id == 1) {
      glBindTexture(GL_TEXTURE_2D,
                    gameData->resources->Models.at(i)->heightMap2Id);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 128 * 4, 128 * 4, GL_RGBA,
                      GL_UNSIGNED_BYTE, snowPixels);
      //  std::cout << "snowPixels size: " << sizeof(snowPixels) << "\n";
      size_t elmes_per_line = 128 * 4;
      size_t row = y * elmes_per_line;
      size_t col = x * 4;
      size_t index = row + col;
      std::cout << "r channel at x, y: " << x << ", " << y << ": "
                << (int)snowPixels[index] << "index: " << index << "\n";
      snowPixels[index] = 0;
      snowPixels[index + 1] = 0;
      snowPixels[index + 2] = 0;
      snowPixels[index + 3] = 0;

      //   for (int i = 0; i < 128 + 128 * 4; i += 4) {
      //     snowPixels[i] = 0;
      //     snowPixels[i + 1] = 0;
      //     snowPixels[i + 2] = 0;
      //     snowPixels[i + 3] = 0;
      //   }
      break;
    }
  }
}

void DroneController::Enable() {}
void DroneController::Disable() {}
