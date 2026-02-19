#include "bane/data/resource.hpp"
#include "game/renderStaticModels.hpp"
#include <bane/bane.hpp>
#include <bane/utility/shader.hpp>
#include <game/data.hpp>
#include <game/render.hpp>
#include <memory>
void Render(std::unique_ptr<GameData> &gameData, float dt) {
  Poll();

  Camera testCamera = Camera(glm::vec3(0.f, 0.f, 0.f));
  gameData->resources->lightData.lightSpaceMatrix = RenderShadow(
      gameData->window, 2048, 2048, gameData->resources->lightData.dirLight,
      gameData->resources->Shaders
          .at(GetShader(gameData->resources, "shadowShader"))
          .get(),
      &testCamera);

  renderShadowedModels(gameData->resources);

  SwitchBuffer();
  ClearColour();

  renderModels(gameData);
}
