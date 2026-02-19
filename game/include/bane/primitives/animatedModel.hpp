#ifndef BANE_ANIMATED_MODEL
#define BANE_ANIMATED_MODEL
#include "assimp/mesh.h"
#include "bane/primitives/transform.hpp"
#include "bane/utility/boneRotation.hpp"
#include <array>
#include <bane/primitives/mesh.hpp>
#include <bane/primitives/texture.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>
class Shader;
class Camera;
struct aiMesh;
struct aiNode;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

struct RotationKeyFrame {
  glm::quat rot;
  float timeStamp;
};

struct PositionKeyFrame {
  glm::vec3 pos;
  float timeStamp;
};

struct ScaleKeyFrame {
  glm::vec3 scale;
  float timeStamp;
};

struct AnimationMetaData {
  std::string name;
  float AnimDuration;
  int ticksPerSecond;
};

struct Animation {
  int AnimID;
  std::string AnimName; // maybe not needed here
  float AnimDuration;   // maybe not needed here
  int ticksPerSecond;
  std::vector<RotationKeyFrame> rotKeyFrames;
  std::vector<PositionKeyFrame> posKeyFrames;
  std::vector<ScaleKeyFrame> scaleKeyFrames;
};

struct Bone {
  int ID;
  std::string Name;
  glm::mat4 Offset;
  glm::mat4 AnimTransform;
  glm::mat4 LocalTransform;
  glm::mat4 InvTransform;
  std::vector<Bone *> Children;
  std::vector<Animation> AnimationData;
};

class AnimatedModel {
public:
  int id;
  unsigned int textureId;
  unsigned int specularId;
  unsigned int normalMapId;
  unsigned int emissiveMapId;
  std::string name;
  std::string fileName;
  bool castShadow = false;
  bool hasSpecularMap = false;
  bool hasNormalMap = false;
  bool hasEmissiveMap = false;
  glm::vec3 emissiveColour = glm::vec3(0.f);
  AnimatedModel(const char *path, glm::vec3 pos);
  AnimatedModel(const char *path, std::string objName, int id);
  Transform transform;
  glm::vec3 position;
  glm::mat4 rotation = glm::mat4(1.f);
  // TEST VARIABLE
  float torsoTilt = 0.f;
  bool looping = true;

  // The following should be moved to a character class or something
  glm::vec3 modelDir;
  glm::vec3 modelUp;
  glm::vec3 modelRight;
  glm::vec3 modelTarget;
  //
  //

  std::vector<BoneRotation> boneRotations;

  void Render(glm::vec3 pos, Shader *shader, Camera *cam);
  void RenderBasic();
  // new will overwrite old kinda
  void Render(glm::mat4 transform, Shader *shader, Camera *cam, unsigned int shadowTex);
  void SetBoneMatricesUnif(std::unique_ptr<Shader> &shader);
  void ApplyLighting(Shader *shader);
  void calcInverseTransform(Bone *bone, glm::mat4 parentTransform);
  // maybe by name instead but this works for now
  void PlayAnimation(int animIndex);
  void PlayAnimationOnce(int animIndex);
  void StopAnimation();
  bool IsPlayingAnimation();
  void UpdateAnimation(float dt);
  void calcAnimTransform(Bone *bone, glm::mat4 transform);
  std::array<glm::mat4, 100> boneMatrices;

  int currentAnimationIndex = -1;

private:
  Bone *rootBone = nullptr;
  glm::mat4 globalInverseMat4;
  std::map<unsigned int, Bone> boneMap;
  std::map<std::string, unsigned int> namedBoneMap;
  std::map<unsigned int, AnimationMetaData> animationMap;
  std::vector<Mesh> meshes;
  std::string directory;
  float animTime = 0.f;
  std::vector<Texture> textures_loaded;
  void loadAnimatedModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  void processBones(std::vector<Vertex> &vertices, aiMesh *mesh, const aiScene *scene, aiNode *node);
  void createHierarchy(aiMesh *mesh, aiNode *node, const aiScene *scene);
  void addAnimationData(const aiScene *scene);
  float getInterpDelta(float prevFrameTime, float nextFrameTime);
  int getCurrentPosFrameIndex(Bone *bone);
  int getCurrentRotFrameIndex(Bone *bone);
  int getCurrentScaleFrameIndex(Bone *bone);
  glm::mat4 interpPos(Bone *bone);
  glm::mat4 interpRot(Bone *bone);
  glm::mat4 interpScale(Bone *bone);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
#endif
