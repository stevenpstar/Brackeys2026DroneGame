#ifndef BANE_SCENE
#define BANE_SCENE
#include <bane/primitives/pointLight.hpp>
#include <bane/scene/sceneAnimatedModel.hpp>
#include <bane/scene/sceneModel.hpp>
#include <glm/glm.hpp>
#include <vector>
struct SceneCharacter;
class AABB;
struct Scene {
  std::string sceneName;
  // maybe at some point a 2d vector, so we can instance static mesh rendering automatically(?)
  std::vector<SceneModel> staticModels;
  std::vector<AABB> colliders;
  std::vector<SceneAnimatedModel> animatedModels;
  // std::vector<SceneCharacter> characters;
  // increase point light count eventually
  std::vector<PointLight> pointLights;
  glm::vec3 lightDirection;
  float ambientStrength;
  float sunlightIntensity;
};
#endif
