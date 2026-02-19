#ifndef BANE_TEXT_RENDERER
#define BANE_TEXT_RENDERER
#include <glm/glm.hpp>
struct Glyph {
  unsigned int textureId;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};


#endif
