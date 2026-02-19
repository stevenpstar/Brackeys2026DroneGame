#ifndef BANE_BONE_ROTATION
#define BANE_BONE_ROTATION

#include <glm/ext/vector_float3.hpp>
#include <string>
struct BoneRotation {
  std::string BoneName;
  float Angle;
  glm::vec3 Axis;
  bool Global = false;
};

#endif
