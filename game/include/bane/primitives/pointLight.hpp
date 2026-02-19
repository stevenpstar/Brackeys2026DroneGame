#ifndef BANE_POINT_LIGHT
#define BANE_POINT_LIGHT
#include "bane/utility/AABB.hpp"
#include <glm/glm.hpp>
class Shader;
static const float PointLightLinear = 0.09f;
static const float PointLightConstant = 1.f;
static const float PointLightQuad = 0.032f;
struct PointLight {
  glm::vec3 position;
  glm::vec3 diffuse;
  float constant;
  float linear;
  float quadratic;
  float intensity;
};

void AssignPointLightUnif(PointLight *light, int index, Shader *shader);
#endif
