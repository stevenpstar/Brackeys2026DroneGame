#ifndef BANE_MODEL
#define BANE_MODEL
#include <bane/primitives/mesh.hpp>
#include <bane/primitives/texture.hpp>
#include <glm/fwd.hpp>
#include <string>
#include <vector>
class Shader;
class Camera;
struct aiMesh;
struct aiNode;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

class Model {
public:
  Model(const char *path);
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  glm::vec3 tint;
  std::string name;
  void Render(Shader *shader, Camera *camera);
  void Render(glm::mat4 transform, Shader *shader, Camera *camera, unsigned int shadowTex, unsigned int normalTex);
  void RenderBasic();
  void SetPosition(glm::vec3 pos);
  std::vector<Mesh> meshes;

private:
  std::string directory;
  std::vector<Texture> textures_loaded;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
  unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};
#endif
