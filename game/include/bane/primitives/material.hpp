#ifndef BANE_MATERIAL
#define BANE_MATERIAL
#include <glm/glm.hpp>
class Shader;
struct Material {
  unsigned int diffuseTextureId;
  unsigned int shadowMapTextureId;
  unsigned int cubeMapTextureId;
  //  unsigned int specularTextureId;
  bool reflective;
  bool useRimLighting;
  float shine;

  glm::vec3 objectTint;
};

void AssignMaterialUnif(Material *material, Shader *shader);
#endif
