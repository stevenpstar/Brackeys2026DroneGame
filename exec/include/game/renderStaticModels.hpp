#ifndef COLD_CASE_RENDER_STATIC
#define COLD_CASE_RENDER_STATIC

#include "game/data.hpp"
#include <memory>
void renderModels(std::unique_ptr<GameData> &gameData);
void renderModel(std::unique_ptr<Resources> &resources, int i);
void renderOutlinedModel(std::unique_ptr<Resources> &resources, int i);
void renderShadowedModels(std::unique_ptr<Resources> &resources);
void RenderColliders(std::unique_ptr<GameData> &gameData);

#endif
