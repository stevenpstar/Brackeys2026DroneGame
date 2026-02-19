#ifndef BANE_MESH
#define BANE_MESH
#include <bane/primitives/texture.hpp>
#include <bane/primitives/vertex.hpp>
#include <vector>
class Shader;
class Camera;
struct aiScene;
struct aiMesh;
struct LightData;
class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  std::vector<Texture> normalMaps;
  std::vector<Texture> specularMaps;

  Mesh(std::vector<Vertex> verts, std::vector<unsigned int> ind, std::vector<Texture> tex);

  void Render(glm::vec3 pos, glm::vec3 rotation, Shader *shader, Camera *camera);
  void Render(glm::mat4 transform, Shader *shader, Camera *camera, LightData *lightData, unsigned int shadowTex,
              unsigned int normalTex);
  void RenderBasic();
  void setupMesh();
  unsigned int VAO, VBO, EBO;

private:
};
#endif
