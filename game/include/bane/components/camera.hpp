#ifndef BANE_CAMERA
#define BANE_CAMERA
#include <glm/glm.hpp>
class Camera {
public:
  Camera(glm::vec3 position);
  float drawStart = 0.1f;
  float drawEnd = 100.f;
  glm::mat4 lookAtTarget(glm::vec3 target);
  glm::mat4 getTransform() const;
  glm::mat4 projection;

  void setPosition(glm::vec3 position);
  void setCameraDirection(glm::vec3 dir);
  void setProjection(int width, int height, float fov);
  void setDrawDistance(float dist);
  glm::vec3 getPosition() const;
  glm::vec3 getDirection() const;
  glm::vec3 getCameraUp() const;
  glm::vec3 getCameraRight() const;

private:
  glm::mat4 cameraTransform;
  glm::vec3 cameraPos;
  glm::vec3 cameraTarget;
  glm::vec3 cameraDirection;
  glm::vec3 up;
  glm::vec3 cameraRight;
  glm::vec3 cameraUp;
};
#endif
