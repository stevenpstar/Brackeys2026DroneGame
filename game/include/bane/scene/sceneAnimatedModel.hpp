#ifndef BANE_SCENE_ANIMATED_MODEL
#define BANE_SCENE_ANIMATED_MODEL
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct SceneAnimation {
  std::string name;
  bool loop;
  int index;
};

struct SceneAnimatedModel {
  int id;
  std::string name;
  std::string filePath;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  std::string diffuse;
  std::string specular;
  bool castShadow;
  bool playing;
  int defaultAnimation;
  std::vector<SceneAnimation> animations;
};
#endif
