
#include "game/testDroneCharacter.hpp"
#include "bane/bane.hpp"
#include "bane/primitives/animatedModel.hpp"
#include "game/data.hpp"
#include "game/loadResources.hpp"
#include <glad/glad.h>
#include <memory>
DroneCharacter::DroneCharacter(std::string path,
                               std::unique_ptr<GameData> &gameData) {
  this->droneModel = std::make_unique<AnimatedModel>(AnimatedModel(
      (path + "../game/assets/animated_models/dronefinal2.gltf").c_str(),
      "testdronegfheaogea", 556));
  this->droneModel->textureId =
      getTextureByName("droneblue", gameData->resources->Textures);
  this->droneModel->specularId =
      getTextureByName("droneworn_specular", gameData->resources->Textures);
  this->droneModel->emissiveMapId =
      getTextureByName("dronebatteryemissive", gameData->resources->Textures);
  this->droneModel->hasSpecularMap = true;
  this->droneModel->hasEmissiveMap = true;
  this->droneModel->transform.position = glm::vec3(0.f, 0.f, 0.f);
  this->droneModel->transform.rotation = glm::vec3(0.f, 0.f, 0.f);
  this->droneModel->transform.scale = glm::vec3(1.f, 1.f, 1.f);

  BoneRotation rBoosterRotationY;
  rBoosterRotationY.Axis = glm::vec3(0.f, 1.f, 0.f);
  rBoosterRotationY.Angle = 0.f;
  rBoosterRotationY.BoneName = "frontright";
  rBoosterRotationY.Global = true;

  BoneRotation rootRotation;
  rootRotation.Axis = glm::vec3(1.f, 0.f, 0.f);
  rootRotation.Angle = 0.f;
  rootRotation.BoneName = "root";
  rootRotation.Global = true;

  BoneRotation rBoosterRotationX;
  rBoosterRotationX.Axis = glm::vec3(1.0, 0.f, 0.f);
  rBoosterRotationX.Angle = 0.f;
  rBoosterRotationX.BoneName = "frontright";
  rBoosterRotationX.Global = true;

  BoneRotation rBoosterRotationZ;
  rBoosterRotationZ.Axis = glm::vec3(0.0, 0.f, 1.f);
  rBoosterRotationZ.Angle = 0.f;
  rBoosterRotationZ.BoneName = "frontright";
  rBoosterRotationZ.Global = true;

  BoneRotation rbBoosterRotationY;
  rbBoosterRotationY.Axis = glm::vec3(0.f, 1.f, 0.f);
  rbBoosterRotationY.Angle = 0.f;
  rbBoosterRotationY.BoneName = "backright";
  rbBoosterRotationY.Global = true;

  BoneRotation rbBoosterRotationX;
  rbBoosterRotationX.Axis = glm::vec3(1.0, 0.f, 0.f);
  rbBoosterRotationX.Angle = 0.f;
  rbBoosterRotationX.BoneName = "backright";
  rbBoosterRotationX.Global = true;

  BoneRotation rbBoosterRotationZ;
  rbBoosterRotationZ.Axis = glm::vec3(0.0, 0.f, 1.f);
  rbBoosterRotationZ.Angle = 0.f;
  rbBoosterRotationZ.BoneName = "backright";
  rbBoosterRotationZ.Global = true;

  // left side (maybe they should be reversed????

  BoneRotation lBoosterRotationY;
  lBoosterRotationY.Axis = glm::vec3(0.f, 1.f, 0.f);
  lBoosterRotationY.Angle = 0.f;
  lBoosterRotationY.BoneName = "frontleft";
  lBoosterRotationY.Global = true;

  BoneRotation lBoosterRotationX;
  lBoosterRotationX.Axis = glm::vec3(1.0, 0.f, 0.f);
  lBoosterRotationX.Angle = 0.f;
  lBoosterRotationX.BoneName = "frontleft";
  lBoosterRotationX.Global = true;

  BoneRotation lBoosterRotationZ;
  lBoosterRotationZ.Axis = glm::vec3(0.0, 0.f, 1.f);
  lBoosterRotationZ.Angle = 0.f;
  lBoosterRotationZ.BoneName = "frontleft";
  lBoosterRotationZ.Global = true;

  BoneRotation lbBoosterRotationY;
  lbBoosterRotationY.Axis = glm::vec3(0.f, 1.f, 0.f);
  lbBoosterRotationY.Angle = 0.f;
  lbBoosterRotationY.BoneName = "backleft";
  lbBoosterRotationY.Global = true;

  BoneRotation lbBoosterRotationX;
  lbBoosterRotationX.Axis = glm::vec3(1.0, 0.f, 0.f);
  lbBoosterRotationX.Angle = 0.f;
  lbBoosterRotationX.BoneName = "backleft";
  lbBoosterRotationX.Global = true;

  BoneRotation lbBoosterRotationZ;
  lbBoosterRotationZ.Axis = glm::vec3(0.0, 0.f, 1.f);
  lbBoosterRotationZ.Angle = 0.f;
  lbBoosterRotationZ.BoneName = "backleft";
  lbBoosterRotationZ.Global = true;

  this->droneModel->boneRotations.emplace_back(rootRotation);
  this->droneModel->boneRotations.emplace_back(rBoosterRotationY);
  this->droneModel->boneRotations.emplace_back(rBoosterRotationX);
  this->droneModel->boneRotations.emplace_back(rBoosterRotationZ);

  this->droneModel->boneRotations.emplace_back(rbBoosterRotationY);
  this->droneModel->boneRotations.emplace_back(rbBoosterRotationX);
  this->droneModel->boneRotations.emplace_back(rbBoosterRotationZ);

  this->droneModel->boneRotations.emplace_back(lBoosterRotationY);
  this->droneModel->boneRotations.emplace_back(lBoosterRotationX);
  this->droneModel->boneRotations.emplace_back(lBoosterRotationZ);

  this->droneModel->boneRotations.emplace_back(lbBoosterRotationY);
  this->droneModel->boneRotations.emplace_back(lbBoosterRotationX);
  this->droneModel->boneRotations.emplace_back(lbBoosterRotationZ);

  this->droneModel->PlayAnimation(0);
}

void DroneCharacter::RenderColour(std::unique_ptr<GameData> &gameData) {
  int shaderIndex = GetShader(gameData->resources, "animDebugShader");
  // std::cout << "pos x: " << this->droneModel->transform.scale.x << "\n";

  //  this->droneModel->SetBoneMatricesUnif(
  //      gameData->resources->Shaders.at(shaderIndex));
  gameData->resources->Shaders.at(shaderIndex)->use(); // outline shader
  glm::mat4 pos = glm::mat4(1.f);
  pos = glm::translate(pos, this->droneModel->transform.position);
  glm::mat4 rot = glm::mat4(1.f);
  // This may not always work for rotations so ??
  rot = glm::rotate(rot, this->droneModel->transform.rotation.x,
                    glm::vec3(1.f, 0.f, 0.f));
  // rot = glm::rotate(rot, this->droneModel->transform.rotation.x,
  //                   glm::vec3(0.f, 1.f, 0.f));
  rot = glm::rotate(rot, glm::radians(this->droneModel->transform.rotation.z),
                    glm::vec3(0.f, 0.f, 1.f));
  glm::mat4 scale = glm::mat4(1.f);
  scale = glm::scale(scale, this->droneModel->transform.scale);

  scale = glm::scale(scale, glm::vec3(1.025f, 1.025f, 1.025f));

  pos = glm::mat4(1.f);
  pos = glm::translate(pos, glm::vec3(0.f, 0.f, 0.f));
  rot = glm::mat4(1.f);
  scale = glm::mat4(1.f);
  scale = glm::scale(scale, glm::vec3(1.f, 1.f, 1.f));

  glm::mat4 model = pos * rot * scale;
  gameData->resources->Shaders.at(shaderIndex)->setMat4("model", model);
  gameData->resources->Shaders.at(shaderIndex)
      ->setMat4("view", gameData->resources->camera.getTransform());
  gameData->resources->Shaders.at(shaderIndex)
      ->setMat4("projection", gameData->resources->camera.projection);
  gameData->resources->Shaders.at(shaderIndex)
      ->setVec3("colour", glm::vec3(1.f, 0.f, 0.f));
  gameData->resources->Shaders.at(shaderIndex)->setFloat("alpha", 1.f);
  this->droneModel->RenderBasic();
}

void DroneCharacter::Render(std::unique_ptr<GameData> &gameData) {
  if (this->droneModel) {
    glm::mat4 pos = glm::mat4(1.f);
    pos = glm::translate(pos, this->droneModel->transform.position);
    glm::mat4 rot = glm::mat4(1.f);
    // This may not always work for rotations so ??
    rot = glm::rotate(rot, glm::radians(this->droneModel->transform.rotation.x),
                      glm::vec3(1.f, 0.f, 0.f));
    rot = glm::rotate(rot, glm::radians(this->droneModel->transform.rotation.y),
                      glm::vec3(0.f, 1.f, 0.f));
    rot = glm::rotate(rot, glm::radians(this->droneModel->transform.rotation.z),
                      glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 scale = glm::mat4(1.f);
    scale = glm::scale(scale, glm::vec3(1.f, 1.f, 1.f));

    pos = pos * rot * scale;

    int shaderIndex = GetShader(gameData->resources, "droneShader");

    gameData->resources->Shaders.at(shaderIndex)->use();
    this->droneModel->SetBoneMatricesUnif(
        gameData->resources->Shaders.at(shaderIndex));

    gameData->resources->Shaders.at(shaderIndex)->setBool("lightRim", true);
    gameData->resources->Shaders.at(shaderIndex)->setBool("reflective", false);
    gameData->resources->Shaders.at(shaderIndex)
        ->setMat4("lightSpaceMatrix",
                  gameData->resources->lightData.lightSpaceMatrix);
    //  glActiveTexture(GL_TEXTURE2);
    //  glBindTexture(GL_TEXTURE_CUBE_MAP, skies[0].texture);
    //  shaders[0].setInt("reflectionMap", 2);
    gameData->resources->Shaders.at(shaderIndex)->setMat4("model", pos);
    gameData->resources->Shaders.at(shaderIndex)
        ->setMat4("view", gameData->resources->camera.getTransform());
    gameData->resources->Shaders.at(shaderIndex)
        ->setMat4("projection", gameData->resources->camera.projection);
    gameData->resources->Shaders.at(shaderIndex)
        ->setVec3("viewPos", gameData->resources->camera.getPosition());
    gameData->resources->Shaders.at(shaderIndex)
        ->setVec3("camDir", gameData->resources->camera.getDirection());
    // set point light values, can probably actually just do this once unless
    // they move(?) TODO: Maybe set on scene load.
    for (int p = 0; p < gameData->resources->lightData.pointLights.size();
         ++p) {
      gameData->resources->Shaders.at(shaderIndex)
          ->setVec3("pointLights[" + std::to_string(p) + "].position",
                    gameData->resources->lightData.pointLights.at(p)->position);
      gameData->resources->Shaders.at(shaderIndex)
          ->setVec3("pointLights[" + std::to_string(p) + "].diffuse",
                    gameData->resources->lightData.pointLights.at(p)->diffuse);
      gameData->resources->Shaders.at(shaderIndex)
          ->setFloat(
              "pointLights[" + std::to_string(p) + "].constant",
              gameData->resources->lightData.pointLights.at(p)->constant);
      gameData->resources->Shaders.at(shaderIndex)
          ->setFloat("pointLights[" + std::to_string(p) + "].linear",
                     gameData->resources->lightData.pointLights.at(p)->linear);
      gameData->resources->Shaders.at(shaderIndex)
          ->setFloat(
              "pointLights[" + std::to_string(p) + "].quadratic",
              gameData->resources->lightData.pointLights.at(p)->quadratic);
      gameData->resources->Shaders.at(shaderIndex)
          ->setFloat(
              "pointLights[" + std::to_string(p) + "].intensity",
              gameData->resources->lightData.pointLights.at(p)->intensity);
    }

    AnimatedModel *statMesh =
        dynamic_cast<AnimatedModel *>(this->droneModel.get());
    if (statMesh) {

      gameData->resources->Shaders.at(shaderIndex)
          ->setVec3("objectColour", glm::vec3(1.f, 1.f, 1.f));
    } else {
      gameData->resources->Shaders.at(shaderIndex)
          ->setVec3("objectColour", glm::vec3(1.f, 1.f, 1.f));
    }
    gameData->resources->Shaders.at(shaderIndex)
        ->setVec3("lightPos", gameData->resources->lightData.dirLight);
    gameData->resources->Shaders.at(shaderIndex)
        ->setVec3("sunColour", gameData->resources->lightData.sunColour);
    gameData->resources->Shaders.at(shaderIndex)
        ->setFloat("sunIntensity", gameData->resources->lightData.sunIntensity);

    gameData->resources->Shaders.at(shaderIndex)
        ->setFloat("ambientStrength",
                   gameData->resources->lightData.ambientStrength);

    // Textures
    gameData->resources->Shaders.at(shaderIndex)->setInt("shadowMap", 1);
    gameData->resources->Shaders.at(shaderIndex)->setInt("diffuseTexture", 0);
    gameData->resources->Shaders.at(shaderIndex)->setInt("specularMap", 2);
    gameData->resources->Shaders.at(shaderIndex)->setInt("emissiveMap", 3);

    gameData->resources->Shaders.at(shaderIndex)->setBool("lightRim", true);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->droneModel->textureId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, getShadowTexture());
    if (this->droneModel->hasSpecularMap) {
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, this->droneModel->specularId);
      gameData->resources->Shaders.at(shaderIndex)
          ->setBool("hasSpecularMap", this->droneModel->hasSpecularMap);
    }
    if (this->droneModel->hasEmissiveMap) {
      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D, this->droneModel->emissiveMapId);
      gameData->resources->Shaders.at(shaderIndex)
          ->setBool("hasEmissiveMap", this->droneModel->hasEmissiveMap);
      gameData->resources->Shaders.at(shaderIndex)
          ->setVec3("emissiveColour", this->droneModel->emissiveColour);
      gameData->resources->Shaders.at(shaderIndex)
          ->setFloat("batteryPercentage", this->batteryPercentage / 100.f);
    }

    this->droneModel->RenderBasic();
  }
}

void DroneCharacter::renderDroneShadow(std::unique_ptr<GameData> &gameData) {
  unsigned int shadowTex = getShadowTexture();
  int shaderIndex = GetShader(gameData->resources, "animShadowShader");
  gameData->resources->Shaders.at(shaderIndex)->use();
  this->droneModel->SetBoneMatricesUnif(
      gameData->resources->Shaders.at(shaderIndex));

  glm::mat4 pos = glm::mat4(1.f);
  glm::mat4 rot = glm::mat4(1.f);
  glm::mat4 scale = glm::mat4(1.f);
  pos = glm::translate(pos, this->droneModel->transform.position);
  rot = glm::rotate(rot, glm::radians(this->droneModel->transform.rotation.x),
                    glm::vec3(1.f, 0.f, 0.f));
  rot = glm::rotate(rot, glm::radians(this->droneModel->transform.rotation.y),
                    glm::vec3(0.f, 1.f, 0.f));
  rot = glm::rotate(rot, glm::radians(this->droneModel->transform.rotation.z),
                    glm::vec3(0.f, 0.f, 1.f));
  scale = glm::scale(scale, glm::vec3(1.f, 1.f, 1.f));
  glm::mat4 transform = pos * rot * scale;
  gameData->resources->Shaders.at(shaderIndex)->setMat4("model", transform);
  gameData->resources->Shaders.at(shaderIndex)
      ->setMat4("lightSpaceMatrix",
                gameData->resources->lightData.lightSpaceMatrix);
  this->droneModel->RenderBasic();
}
