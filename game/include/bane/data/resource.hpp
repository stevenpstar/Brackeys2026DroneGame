#ifndef BANE_RENDER_DATA
#define BANE_RENDER_DATA
#include <bane/components/bobject.hpp>
#include <bane/components/camera.hpp>
#include <bane/components/lightdata.hpp>
#include <bane/primitives/animatedModel.hpp>
#include <bane/primitives/texture.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <string>
#include <vector>

class Shader;
class Model;
class AABB;
class AnimatedModel;
struct Texture;

struct Resources {
  Camera camera;
  LightData lightData;
  std::vector<std::unique_ptr<Texture>> Textures;
  std::vector<std::unique_ptr<Shader>> Shaders;
  std::vector<std::unique_ptr<AABB>> Colliders;
  std::vector<std::unique_ptr<Bobject>> Models;
  std::vector<std::unique_ptr<AnimatedModel>> AnimatedModel;
  Resources() : camera(Camera(glm::vec3(0.f, 0.f, 0.f))) {};
};

int GetTexture(Resources &resources, std::string textureName);
std::string GetTextureName(std::unique_ptr<Resources> &resources, unsigned int textureID);
int GetCollider(Resources &resources, std::string collider);
int GetShader(const std::unique_ptr<Resources> &resources, std::string shaderName);
int GetModel(Resources &resources, std::string modelName);
int GetAnimatedModel(Resources &resources, std::string textureName);

#endif
