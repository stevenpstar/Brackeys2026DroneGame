#ifndef BANE_VERTEX
#define BANE_VERTEX
#include <array>
#define MAX_BONE_INFLUENCE 4
#include <glm/glm.hpp>
struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;

  std::array<int, 4> BoneIds;
  std::array<float, 4> Weights;
};
#endif
