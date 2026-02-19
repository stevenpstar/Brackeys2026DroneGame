#ifndef BANE_AABB
#define BANE_AABB

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>
class Camera;
class Shader;
class Bobject;
class AABB {
public:
  AABB(glm::vec3 origin, float xdim, float ydim, float zdim, Bobject *owner, std::string name, int id);
  std::string name;
  int id;
  glm::vec3 origin;
  float xdim;
  float ydim;
  float zdim;
  unsigned int VAO;
  unsigned int VBO;
  Bobject *owner;
  int ownerId = -1;
  std::vector<std::string> tags;
  void setupAABB();
  void render(Camera *camera, Shader *shader);
  bool isColliding(std::unique_ptr<AABB> &aabb);
};

float AABB_getMinX(std::unique_ptr<AABB> &aabb);
float AABB_getMaxX(std::unique_ptr<AABB> &aabb);

float AABB_getMinY(std::unique_ptr<AABB> &aabb);
float AABB_getMaxY(std::unique_ptr<AABB> &aabb);

float AABB_getMinZ(std::unique_ptr<AABB> &aabb);
float AABB_getMaxZ(std::unique_ptr<AABB> &aabb);

#endif
