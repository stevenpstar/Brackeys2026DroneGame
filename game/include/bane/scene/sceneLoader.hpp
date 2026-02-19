#ifndef BANE_SCENE_LOADER
#define BANE_SCENE_LOADER
#include <bane/core/scene.hpp>
#include <string>
Scene loadScene(std::string scenePath);
Scene createEmptyScene(std::string name);
#endif
