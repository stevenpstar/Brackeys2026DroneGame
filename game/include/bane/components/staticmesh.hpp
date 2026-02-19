#ifndef BANE_STATIC_MESH
#define BANE_STATIC_MESH
#include <bane/components/bobject.hpp>
#include <bane/primitives/model.hpp>
#include <bane/utility/AABB.hpp>
class Camera;
class Shader;
class Model;
class StaticMesh : public Bobject {
private:
public:
  Model model;
  StaticMesh(const char *modelPath, std::string objName, int id);
  void Render(Camera *camera) override;
  void Render(glm::mat4 transform, Shader *shader, Camera *camera, unsigned int shadowTex, unsigned int normalTex) override;
  void RenderBasic() override;
  void Update(float dt) override;
};
#endif
