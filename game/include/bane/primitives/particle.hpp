#ifndef BANE_PARTICLE
#define BANE_PARTICLE
#include <glm/glm.hpp>
struct Particle {
  glm::vec3 pos, speed;
  glm::mat4 transform;
  float life;
};
#endif
