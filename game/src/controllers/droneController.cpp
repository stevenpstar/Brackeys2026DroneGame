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
#include <wchar.h>
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
        this->droneCharacter->isLaserActive = false;
        this->droneCharacter->isScannerActive = false;

      } else {
      }
    }

  } else if (button == "a" && !pressed) {
    forwardDown = false;
  }
  if (button == "b" && pressed && !autoMoving) {
    this->droneCharacter->isLaserActive = true;
  }
  if (button == "b" && !pressed) {
    this->droneCharacter->isLaserActive = false;
  }

  if (button == "y" && pressed && !autoMoving) {
    this->droneCharacter->isScannerActive = true;
  }
  if (button == "y" && !pressed) {
    this->droneCharacter->isScannerActive = false;
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

  if (this->droneCharacter->scannerColliderIndex >= 0) {
    gameData->resources->Colliders
        .at(this->droneCharacter->scannerColliderIndex)
        ->origin = glm::vec3(this->droneCharacter->transform.position.x,
                             this->droneCharacter->transform.position.y - 40.f,
                             this->droneCharacter->transform.position.z);
  }

  if (this->droneCharacter->laserColliderIndex >= 0) {
    gameData->resources->Colliders.at(this->droneCharacter->laserColliderIndex)
        ->origin = glm::vec3(this->droneCharacter->transform.position.x,
                             this->droneCharacter->transform.position.y - 40.f,
                             this->droneCharacter->transform.position.z);
  }

  if (!inChargingArea) {
    float batteryUsage = 0.f;
    if (this->droneCharacter->isLaserActive) {
      batteryUsage += 10.f;
    }
    if (this->droneCharacter->isScannerActive) {
      batteryUsage += 5.f;
    }
    batteryCharge -= (1.f + batteryUsage) * deltaTime;
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

  if (this->droneCharacter->isLaserActive) {
    // check collisions
    for (auto &collider : gameData->resources->Colliders) {
      if (gameData->resources->Colliders.at(
              this->droneCharacter->laserColliderIndex) == collider) {
        continue;
      }

      if (gameData->resources->Colliders
              .at(this->droneCharacter->laserColliderIndex)
              ->isColliding(collider)) {
        for (auto &tag : collider->tags) {
          if (tag == "unfound") {
            std::cout << "Found something!\n";
            tag = "found";
          }
        }
      }
    }
    gameData->resources->lightData.pointLights.at(0)->position =
        glm::vec3(this->droneCharacter->transform.position.x,
                  this->droneCharacter->transform.position.y - 2.f,
                  this->droneCharacter->transform.position.z);
    glm::vec3 lightColour = glm::vec3(1.0f, 0.25f, 0.2f);
    gameData->resources->lightData.pointLights.at(0)->diffuse = lightColour;
  } else if (this->droneCharacter->isScannerActive) {
    this->droneCharacter->scanTimer += 40.f * deltaTime;
    gameData->resources->lightData.pointLights.at(0)->position =
        glm::vec3(this->droneCharacter->transform.position.x,
                  this->droneCharacter->transform.position.y,
                  this->droneCharacter->transform.position.z);
    glm::vec3 lightColour = glm::vec3(0.2f, 0.85f, 0.2f);
    if (this->droneCharacter->scanTimer >= this->droneCharacter->scanTime) {
      this->droneCharacter->scanTimer = 0.f;
      for (auto &collider : gameData->resources->Colliders) {
        if (gameData->resources->Colliders.at(
                this->droneCharacter->laserColliderIndex) == collider) {
          continue;
        }

        if (gameData->resources->Colliders
                .at(this->droneCharacter->laserColliderIndex)
                ->isColliding(collider)) {
          for (auto &tag : collider->tags) {
            if (tag == "scannable") {
              tag = "scanned";
            }
            if (tag == "techpiece1") {
              std::cout << "Found tech piece 1\n";
              this->droneCharacter->techPiece1Collected = true;
            }
            if (tag == "techpiece2") {
              std::cout << "Found tech piece 2\n";
              this->droneCharacter->techPiece2Collected = true;
            }
          }
        }
      }
    }

    gameData->resources->lightData.pointLights.at(0)->diffuse = lightColour;
  } else {
    gameData->resources->lightData.pointLights.at(0)->position =
        glm::vec3(this->droneCharacter->transform.position.x,
                  this->droneCharacter->transform.position.y + 5.f,
                  this->droneCharacter->transform.position.z);
    glm::vec3 lightColour = glm::vec3(0.4f, 0.2, 0.0f);
    gameData->resources->lightData.pointLights.at(0)->diffuse = lightColour;
  }
  // lightColour.z += abs((velocity.z / (maxVelocity * 4.0)));
  // lightColour.z += abs((velocity.x / (maxVelocity * 4.0)));
  // lightColour.z += abs((velocity.y / (maxVelocity * 4.0)));

  glm::vec3 desiredCameraPosition =
      glm::vec3(this->droneCharacter->transform.position.x,
                this->droneCharacter->transform.position.y + 80.f,
                this->droneCharacter->transform.position.z + 40.f);
  glm::vec3 lerpedCameraPosition = gameData->resources->camera.getPosition();
  lerpedCameraPosition =
      lerp(lerpedCameraPosition, desiredCameraPosition, 3.f * deltaTime);
  if (lerpedCameraPosition.y > 110.f) {
    lerpedCameraPosition.y = 110.f;
  }
  if (lerpedCameraPosition.x < 0.f) {
    lerpedCameraPosition.x = 0.f;
  }

  if (lerpedCameraPosition.z > 90.f) {
    lerpedCameraPosition.z = 90.f;
  }

  // gameData->resources->lightData.dirLight =
  //     this->droneCharacter->droneModel->transform.position +
  //     dirLightOffset;
  gameData->resources->camera.setPosition(lerpedCameraPosition);
  // gameData->resources->camera.setPosition(glm::vec3(0.f, 15.f, 10.f));
  gameData->resources->camera.setCameraDirection(glm::vec3(0.f, -0.5f, -0.25f));
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

void DroneController::setSnowPixels(GLubyte *snowPixels,
                                    std::unique_ptr<GameData> &gameData,
                                    float x_offset, float y_offset) {
  if (!this->droneCharacter->isLaserActive) {
    return;
  }

  // GLubyte *snowPixels = new GLubyte[127 * 127 * 4];
  const float coordinateConversion = 256.0 / 100.0;
  int x = floor((this->droneCharacter->transform.position.x + 50.f - x_offset) *
                coordinateConversion);
  int y = floor((this->droneCharacter->transform.position.z + 50.f - y_offset) *
                coordinateConversion);

  //  x = floor((33) * coordinateConversion);
  //  y = floor(11 * coordinateConversion);

  if (x < 0 || x > 255) {
    return;
  }
  if (y < 0 || y > 255) {

    return;
  }
  for (int j = x - 3; j < x + 3; ++j) {
    for (int j2 = y - 3; j2 < y + 3; ++j2) {
      if (j < 0 || j > 255) {
        continue;
      }
      if (j2 < 0 || j2 > 255) {
        continue;
      }
      size_t elmes_per_line = 255 * 3;
      size_t row = j2 * elmes_per_line;
      size_t col = j * 3;
      size_t index = row + col;
      index = (j2 * 256 + j) * 3;
      int value = 0 + (((abs(x - j) + abs(y - j2)) * rand() % 50));
      int firstValue = snowPixels[index] - value;
      if (firstValue < 0) {
        firstValue = 0;
      }
      int secondValue = snowPixels[index + 1] - value;
      if (secondValue < 0) {
        secondValue = 0;
      }

      int thirdValue = snowPixels[index + 2] - value;
      if (thirdValue < 0) {
        thirdValue = 0;
      }

      snowPixels[index] = 0;     // firstValue;
      snowPixels[index + 1] = 0; // secondValue;
      snowPixels[index + 2] = 0; // thirdValue;
    }
  }

  return;
}

void DroneController::Enable() {}
void DroneController::Disable() {}
