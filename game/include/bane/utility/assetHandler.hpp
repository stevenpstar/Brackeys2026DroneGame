#ifndef BANE_ASSET_HANDLER
#define BANE_ASSET_HANDLER
#include <vector>
struct Material;
struct Texture;
class Shader;
class Model;
class AnimatedModel;
class AssetHandler {
private:
  std::vector<Material> materials;
  std::vector<Texture> textures;
  std::vector<Model> models;
  std::vector<AnimatedModel> animatedModels;
  std::vector<Shader> shaders;

public:
  AssetHandler();
  void AddMaterial(Material material);
};
#endif
