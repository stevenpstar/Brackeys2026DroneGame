#ifndef BANE_SKYBOX
#define BANE_SKYBOX
class Camera;
class Shader;
class Skybox
{
  private:
  unsigned int VAO, VBO;
  float cube[108] = {
      -0.5f, -0.5f,  0.5f,  // Bottom-left
     0.5f, -0.5f,  0.5f,  // Bottom-right
     0.5f,  0.5f,  0.5f,  // Top-right
    -0.5f, -0.5f,  0.5f,  // Bottom-left
     0.5f,  0.5f,  0.5f,  // Top-right
    -0.5f,  0.5f,  0.5f,  // Top-left

    // Back face
    -0.5f, -0.5f, -0.5f,  // Bottom-left
     0.5f,  0.5f, -0.5f,  // Top-right
     0.5f, -0.5f, -0.5f,  // Bottom-right
    -0.5f, -0.5f, -0.5f,  // Bottom-left
    -0.5f,  0.5f, -0.5f,  // Top-left
     0.5f,  0.5f, -0.5f,  // Top-right

    // Left face
    -0.5f,  0.5f,  0.5f,  // Top-front
    -0.5f,  0.5f, -0.5f,  // Top-back
    -0.5f, -0.5f, -0.5f,  // Bottom-back
    -0.5f,  0.5f,  0.5f,  // Top-front
    -0.5f, -0.5f, -0.5f,  // Bottom-back
    -0.5f, -0.5f,  0.5f,  // Bottom-front

    // Right face
     0.5f,  0.5f,  0.5f,  // Top-front
     0.5f, -0.5f, -0.5f,  // Bottom-back
     0.5f,  0.5f, -0.5f,  // Top-back
     0.5f,  0.5f,  0.5f,  // Top-front
     0.5f, -0.5f,  0.5f,  // Bottom-front
     0.5f, -0.5f, -0.5f,  // Bottom-back

    // Top face
    -0.5f,  0.5f,  0.5f,  // Front-left
     0.5f,  0.5f,  0.5f,  // Front-right
     0.5f,  0.5f, -0.5f,  // Back-right
    -0.5f,  0.5f,  0.5f,  // Front-left
     0.5f,  0.5f, -0.5f,  // Back-right
    -0.5f,  0.5f, -0.5f,  // Back-left

    // Bottom face
    -0.5f, -0.5f,  0.5f,  // Front-left
     0.5f, -0.5f, -0.5f,  // Back-right
     0.5f, -0.5f,  0.5f,  // Front-right
    -0.5f, -0.5f,  0.5f,  // Front-left
    -0.5f, -0.5f, -0.5f,  // Back-left
     0.5f, -0.5f, -0.5f   // Back-right  
  };

  public:
  unsigned int texture;
  const char* path;
  Skybox(const char* folderPath);
  void Render(Camera* camera, Shader* shader);
};
#endif
