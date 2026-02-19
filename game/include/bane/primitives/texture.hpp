#ifndef BANE_TEXTURE
#define BANE_TEXTURE
#include <string>
struct Texture {
  std::string name;
  std::string path;
  std::string type;
  unsigned int id;
};

unsigned int createTexture(const char *path);
unsigned int createSkyBoxTexture(const char *folder);

#endif
