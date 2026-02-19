#ifndef BANE_SCENE_CHARACTER
#define BANE_SCENE_CHARACTER
#include <bane/scene/sceneAABB.hpp>
#include <bane/scene/sceneAnimatedModel.hpp>
#include <glm/glm.hpp>
#include <string>
struct SceneCharacter {
  std::string name;
  SceneAnimatedModel animatedModel;
  SceneAABB collisionMesh;
  // transforms
  glm::vec3 position;

  // flags maybe/hostile/npc/enemy etc who knows
};
#endif
