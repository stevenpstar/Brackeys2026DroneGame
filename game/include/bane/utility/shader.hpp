#ifndef BANE_SHADER
#define BANE_SHADER

#include <glm/fwd.hpp>
#include <string>

class Shader {
public:
  unsigned int ID;
  std::string name;
  Shader(const char *vertexPath, const char *fragmentPath, std::string name);
  Shader(std::string vertexString, std::string fragmentString, std::string name);

  void use();
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, glm::mat4 value) const;
  void setMat4(const std::string &name, glm::mat4 value, int count) const;
  void setVec3(const std::string &name, glm::vec3 value) const;
};

#endif
