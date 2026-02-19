#ifndef BANE_TEMPLATE_DATA
#define BANE_TEMPLATE_DATA

#include "game/settings.hpp"
#include <bane/components/bobject.hpp>
#include <bane/components/camera.hpp>
#include <bane/components/staticmesh.hpp>
#include <bane/core/scene.hpp>
#include <bane/data/resource.hpp>
#include <bane/primitives/mesh.hpp>
#include <bane/primitives/model.hpp>
#include <bane/primitives/texture.hpp>
#include <bane/utility/AABB.hpp>
#include <bane/utility/shader.hpp>
#include <memory>
struct SDL_Window;
struct Texture;

struct GameState {
  int hoveredModelIndex = -1;
};

struct GameData {
  SDL_Window *window;
  std::unique_ptr<Resources> resources;
  std::vector<Scene> scenes;
  std::unique_ptr<GameSettings> settings;
  GameState state;
};

#endif
