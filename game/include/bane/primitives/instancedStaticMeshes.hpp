#ifndef BANE_INSTANCED_STATIC_MODEL
#define BANE_INSTANCED_STATIC_MODEL
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <vector>
class Model;
class Shader;
class Camera;
class InstancedStaticMeshes
{
  private:
    Model* model;
    Shader* shader;
    std::vector<glm::mat4> transforms;

  public:
    InstancedStaticMeshes(Model* model, Shader* shdr, std::vector<glm::mat4> transforms);
    void Render(Camera* camera);
};
#endif
