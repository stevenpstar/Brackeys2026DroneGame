#ifndef BANE_RAY
#define BANE_RAY

#include <bane/utility/AABB.hpp>
#include <glm/fwd.hpp>
struct DebugRay {
  unsigned int VAO, VBO;
  glm::vec3 startPoint;
  glm::vec3 endPoint;
};
class Camera;
class Shader;
glm::vec3 getRayFromMouse(Camera *camera, int width, int height, double xpos, double ypos);
bool rayWithWorldPlane(glm::vec3 rayDir, glm::vec3 rayOrigin, float offset, glm::vec3 &hitPoint);
bool rayWithAABB(std::unique_ptr<AABB> &aabb, glm::vec3 rayDir, glm::vec3 rayOrigin, glm::vec3 &hitPoint);
void renderRay(glm::vec3 rayOrigin, glm::vec3 rayDir, Camera *camera, Shader *shader);
#endif
