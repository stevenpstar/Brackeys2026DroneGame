#include "bane/bane.hpp"
#include <game/renderStaticModels.hpp>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

void renderModels(std::unique_ptr<GameData> &gameData) {
  for (int i = 0; i < gameData->resources->Models.size(); ++i) {
    if (gameData->state.hoveredModelIndex == i) {
      continue;
    }
    renderModel(gameData->resources, i);
  }
  if (gameData->state.hoveredModelIndex >= 0 &&
      gameData->state.hoveredModelIndex < gameData->resources->Models.size()) {
    renderOutlinedModel(gameData->resources, gameData->state.hoveredModelIndex);
  }
}

void renderModel(std::unique_ptr<Resources> &resources, int i) {
  glm::mat4 pos = glm::mat4(1.f);
  pos = glm::translate(pos, resources->Models.at(i)->transform.position);
  glm::mat4 rot = glm::mat4(1.f);
  // This may not always work for rotations so ??
  rot = glm::rotate(rot,
                    glm::radians(resources->Models.at(i)->transform.rotation.x),
                    glm::vec3(1.f, 0.f, 0.f));
  rot = glm::rotate(rot,
                    glm::radians(resources->Models.at(i)->transform.rotation.y),
                    glm::vec3(0.f, 1.f, 0.f));
  rot = glm::rotate(rot,
                    glm::radians(resources->Models.at(i)->transform.rotation.z),
                    glm::vec3(0.f, 0.f, 1.f));
  glm::mat4 scale = glm::mat4(1.f);
  scale = glm::scale(scale, resources->Models.at(i)->transform.scale);

  pos = pos * rot * scale;

  int shaderIndex = GetShader(resources, "simpleShader");

  resources->Shaders.at(shaderIndex)->use();
  resources->Shaders.at(shaderIndex)->setBool("lightRim", false);
  resources->Shaders.at(shaderIndex)->setBool("reflective", false);
  resources->Shaders.at(shaderIndex)
      ->setMat4("lightSpaceMatrix", resources->lightData.lightSpaceMatrix);
  //  glActiveTexture(GL_TEXTURE2);
  //  glBindTexture(GL_TEXTURE_CUBE_MAP, skies[0].texture);
  //  shaders[0].setInt("reflectionMap", 2);
  resources->Shaders.at(shaderIndex)->setMat4("model", pos);
  resources->Shaders.at(shaderIndex)
      ->setMat4("view", resources->camera.getTransform());
  resources->Shaders.at(shaderIndex)
      ->setMat4("projection", resources->camera.projection);
  resources->Shaders.at(shaderIndex)
      ->setVec3("viewPos", resources->camera.getPosition());
  resources->Shaders.at(shaderIndex)
      ->setVec3("camDir", resources->camera.getDirection());
  // set point light values, can probably actually just do this once unless
  // they move(?) TODO: Maybe set on scene load.
  for (int p = 0; p < resources->lightData.pointLights.size(); ++p) {
    resources->Shaders.at(shaderIndex)
        ->setVec3("pointLights[" + std::to_string(p) + "].position",
                  resources->lightData.pointLights.at(p)->position);
    resources->Shaders.at(shaderIndex)
        ->setVec3("pointLights[" + std::to_string(p) + "].diffuse",
                  resources->lightData.pointLights.at(p)->diffuse);
    resources->Shaders.at(shaderIndex)
        ->setFloat("pointLights[" + std::to_string(p) + "].constant",
                   resources->lightData.pointLights.at(p)->constant);
    resources->Shaders.at(shaderIndex)
        ->setFloat("pointLights[" + std::to_string(p) + "].linear",
                   resources->lightData.pointLights.at(p)->linear);
    resources->Shaders.at(shaderIndex)
        ->setFloat("pointLights[" + std::to_string(p) + "].quadratic",
                   resources->lightData.pointLights.at(p)->quadratic);
    resources->Shaders.at(shaderIndex)
        ->setFloat("pointLights[" + std::to_string(p) + "].intensity",
                   resources->lightData.pointLights.at(p)->intensity);
  }

  StaticMesh *statMesh =
      dynamic_cast<StaticMesh *>(resources->Models.at(i).get());
  if (statMesh) {

    resources->Shaders.at(shaderIndex)
        ->setVec3("objectColour", statMesh->model.tint);
  } else {
    resources->Shaders.at(shaderIndex)
        ->setVec3("objectColour", glm::vec3(1.f, 1.f, 1.f));
  }
  resources->Shaders.at(shaderIndex)
      ->setVec3("lightPos", resources->lightData.dirLight);
  resources->Shaders.at(shaderIndex)
      ->setVec3("sunColour", resources->lightData.sunColour);
  resources->Shaders.at(shaderIndex)
      ->setFloat("sunIntensity", resources->lightData.sunIntensity);

  resources->Shaders.at(shaderIndex)
      ->setFloat("ambientStrength", resources->lightData.ambientStrength);

  // Textures
  resources->Shaders.at(shaderIndex)->setInt("shadowMap", 1);
  resources->Shaders.at(shaderIndex)->setInt("diffuseTexture", 0);

  resources->Shaders.at(shaderIndex)->setInt("specularMap", 2);
  resources->Shaders.at(shaderIndex)->setInt("heightMap", 3);
  resources->Shaders.at(shaderIndex)->setInt("heightMap2", 4);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, resources->Models.at(i)->textureId);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, getShadowTexture());
  if (resources->Models.at(i)->hasSpecularMap) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, resources->Models.at(i)->specularId);
    resources->Shaders.at(shaderIndex)
        ->setBool("hasSpecularMap", resources->Models.at(i)->hasSpecularMap);
  }

  if (resources->Models.at(i)->hasHeightMap) {
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, resources->Models.at(i)->heightMapId);
    resources->Shaders.at(shaderIndex)
        ->setBool("hasHeightMap", resources->Models.at(i)->hasHeightMap);

    resources->Shaders.at(shaderIndex)->setFloat("heightIntensity", 10.f);
  }

  if (resources->Models.at(i)->hasHeightMap2) {
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, resources->Models.at(i)->heightMap2Id);
    resources->Shaders.at(shaderIndex)
        ->setBool("hasHeightMap2", resources->Models.at(i)->hasHeightMap2);
    resources->Shaders.at(shaderIndex)->setFloat("height2Intensity", 1.f);
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, resources->Models.at(i)->textureId);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, getShadowTexture());

  resources->Models.at(i)->RenderBasic();
}

void renderShadowedModels(std::unique_ptr<Resources> &resources) {
  unsigned int shadowTex = getShadowTexture();
  for (int i = 0; i < resources->Models.size(); ++i) {
    if (resources->Models.at(i)->castShadow == false) {
      continue;
    }
    int shaderIndex = GetShader(resources, "shadowShader");
    //  resources->Shaders.at(shaderIndex)->use();
    glm::mat4 pos = glm::mat4(1.f);
    glm::mat4 rot = glm::mat4(1.f);
    glm::mat4 scale = glm::mat4(1.f);
    pos = glm::translate(pos, resources->Models.at(i)->transform.position);
    rot = glm::rotate(rot, resources->Models.at(i)->transform.rotation.x,
                      glm::vec3(1.f, 0.f, 0.f));
    rot = glm::rotate(rot, resources->Models.at(i)->transform.rotation.y,
                      glm::vec3(0.f, 1.f, 0.f));
    rot = glm::rotate(rot, resources->Models.at(i)->transform.rotation.z,
                      glm::vec3(0.f, 0.f, 1.f));
    scale = glm::scale(scale, resources->Models.at(i)->transform.scale);
    glm::mat4 transform = pos * rot * scale;
    resources->Shaders.at(shaderIndex)->setMat4("model", transform);
    resources->Shaders.at(shaderIndex)
        ->setMat4("lightSpaceMatrix", resources->lightData.lightSpaceMatrix);
    resources->Models.at(i)->RenderBasic();
  }
}

void renderOutlinedModel(std::unique_ptr<Resources> &resources, int i) {
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);

  renderModel(resources, i);

  int shaderIndex = GetShader(resources, "gridShader");

  // rendering selection outline
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilMask(0x00);
  glDisable(GL_DEPTH_TEST);
  resources->Shaders.at(shaderIndex)->use(); // outline shader
  glm::mat4 pos = glm::mat4(1.f);
  pos = glm::translate(pos, resources->Models.at(i)->transform.position);
  glm::mat4 rot = glm::mat4(1.f);
  // This may not always work for rotations so ??
  rot = glm::rotate(rot,
                    glm::radians(resources->Models.at(i)->transform.rotation.x),
                    glm::vec3(1.f, 0.f, 0.f));
  rot = glm::rotate(rot,
                    glm::radians(resources->Models.at(i)->transform.rotation.y),
                    glm::vec3(0.f, 1.f, 0.f));
  rot = glm::rotate(rot,
                    glm::radians(resources->Models.at(i)->transform.rotation.z),
                    glm::vec3(0.f, 0.f, 1.f));
  glm::mat4 scale = glm::mat4(1.f);
  scale = glm::scale(scale, resources->Models.at(i)->transform.scale);

  scale = glm::scale(scale, glm::vec3(1.025f, 1.025f, 1.025f));

  glm::mat4 model = pos * rot * scale;
  resources->Shaders.at(shaderIndex)->setMat4("model", model);
  resources->Shaders.at(shaderIndex)
      ->setMat4("view", resources->camera.getTransform());
  resources->Shaders.at(shaderIndex)
      ->setMat4("projection", resources->camera.projection);
  resources->Shaders.at(shaderIndex)
      ->setVec3("colour", glm::vec3(1.f, 1.f, 1.f));
  resources->Shaders.at(shaderIndex)->setFloat("alpha", 0.5f);
  resources->Models.at(i)->RenderBasic();
  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glEnable(GL_DEPTH_TEST);
}

void RenderColliders(std::unique_ptr<GameData> &gameData) {
  for (int i = 0; i < gameData->resources->Colliders.size(); ++i) {

    int shaderIndex = GetShader(gameData->resources, "gridShader");
    gameData->resources->Shaders.at(shaderIndex)->use();
    glm::vec3 colour = glm::vec3(1.0, 0.0, 0.0);
    gameData->resources->Shaders.at(shaderIndex)->setVec3("colour", colour);
    gameData->resources->Shaders.at(shaderIndex)->setFloat("alpha", 1.f);
    gameData->resources->Colliders.at(i)->render(
        &gameData->resources->camera,
        gameData->resources->Shaders.at(shaderIndex).get());
  }
}
