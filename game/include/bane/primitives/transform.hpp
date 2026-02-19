#ifndef BANE_TRANSFORM
#define BANE_TRANSFORM
#include <glm/glm.hpp>
struct Transform {
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
};
#endif
