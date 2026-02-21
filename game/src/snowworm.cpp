#include "bane/bane.hpp"
#include "bane/primitives/animatedModel.hpp"
#include "game/data.hpp"
#include "game/loadResources.hpp"
#include <game/snowworm.hpp>
#include <glad/glad.h>
#include <memory>

SnowWorm::SnowWorm(std::string path, std::unique_ptr<GameData> &gameData,
                   std::string unique_name, int unique_id) {
  this->model = std::make_unique<AnimatedModel>(AnimatedModel(
      (path + "../game/assets/animated_models/snowwithsinglebone.glb").c_str(),
      unique_name.c_str(), unique_id));
  this->model->textureId =
      getTextureByName("snowworm", gameData->resources->Textures);
  this->model->transform.position = glm::vec3(0.f, 20.f, 0.f);
  this->model->UpdateAnimation(0.f);
  this->model->PlayAnimation(0);
}

void SnowWorm::Render(std::unique_ptr<GameData> &gameData) {

  if (this->model) {
    glm::mat4 pos = glm::mat4(1.f);
    pos = glm::translate(pos, this->model->transform.position);
    glm::mat4 rot = glm::mat4(1.f);
    // This may not always work for rotations so ??
    rot = glm::rotate(rot, glm::radians(this->model->transform.rotation.x),
                      glm::vec3(1.f, 0.f, 0.f));
    rot = glm::rotate(rot, glm::radians(this->model->transform.rotation.y),
                      glm::vec3(0.f, 1.f, 0.f));
    rot = glm::rotate(rot, glm::radians(this->model->transform.rotation.z),
                      glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 scale = glm::mat4(1.f);
    scale = glm::scale(scale, glm::vec3(1.f, 1.f, 1.f));

    pos = pos * rot * scale;

    int shaderIndex = GetShader(gameData->resources, "animShader");

    gameData->resources->Shaders.at(shaderIndex)->use();
    this->model->SetBoneMatricesUnif(
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

    AnimatedModel *statMesh = dynamic_cast<AnimatedModel *>(this->model.get());
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
    glBindTexture(GL_TEXTURE_2D, this->model->textureId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, getShadowTexture());
    //  if (this->model->hasSpecularMap) {
    //    glActiveTexture(GL_TEXTURE2);
    //    glBindTexture(GL_TEXTURE_2D, this->model->specularId);
    //    gameData->resources->Shaders.at(shaderIndex)
    //        ->setBool("hasSpecularMap", this->model->hasSpecularMap);
    //  }
    //  if (this->model->hasEmissiveMap) {
    //    glActiveTexture(GL_TEXTURE3);
    //    glBindTexture(GL_TEXTURE_2D, this->model->emissiveMapId);
    //    gameData->resources->Shaders.at(shaderIndex)
    //        ->setBool("hasEmissiveMap", this->model->hasEmissiveMap);
    //    gameData->resources->Shaders.at(shaderIndex)
    //        ->setVec3("emissiveColour", this->model->emissiveColour);
    //    gameData->resources->Shaders.at(shaderIndex)
    //        ->setFloat("batteryPercentage", this->batteryPercentage / 100.f);
    //  }

    this->model->RenderBasic();
  }
}

void SnowWorm::renderShadow(std::unique_ptr<GameData> &gameData) {}
