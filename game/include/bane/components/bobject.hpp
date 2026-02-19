// Bane Object = Bobject
#ifndef BANE_BOBJECT
#define BANE_BOBJECT
#include <bane/primitives/transform.hpp>
#include <bane/utility/AABB.hpp>
#include <glm/glm.hpp>
#include <string>
class Camera;
class Shader;
class Bobject {
private:
public:
  int id;
  unsigned int textureId;
  unsigned int specularId;
  unsigned int normalMapId;
  unsigned int heightMapId;
  unsigned int heightMap2Id;
  unsigned int emissiveMapId;
  std::string name;
  std::string fileName;
  AABB editorCollision;
  Transform transform;
  bool castShadow = false;
  bool hasSpecularMap = false;
  bool hasNormalMap = false;
  bool hasHeightMap = false;
  bool hasHeightMap2 = false;
  bool hasEmissiveMap = false;
  std::vector<Bobject *> children;
  Bobject(std::string objName, int id);
  virtual void Render(Camera *camera);
  virtual void Render(glm::mat4 transform, Shader *shader, Camera *camera, unsigned int shadowTex, unsigned int normalTex);
  virtual void RenderBasic();
  virtual void Update(float dt);
  virtual void setPosition(glm::vec3 position);
};
#endif
