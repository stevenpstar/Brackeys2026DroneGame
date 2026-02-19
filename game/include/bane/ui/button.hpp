#ifndef BANE_BUTTON
#define BANE_BUTTON
#include <array>
#include <functional>
class Shader;
class Camera;
class Button {
  private:
    Shader* shader;
    std::array<float, 32> vertices;
    std::array<unsigned int, 6> indices;

  public:
    std::function<void()> activateCallback;
    unsigned int textureId, VAO, VBO, EBO;
    float x, y, width, height;
    bool hovered = false;
    Button(unsigned int textureId, Shader* shdr, float x, float y, float width, float height);
    void SetupButton();
    void AttachShader(Shader* shdr);
    void AttachTexture(const char* path);
    void UpdateVertices(float x, float y, float width, float height);
    void Render(Camera* camera, Shader* shdr, int width, int height);
    bool IsHovered(double mx, double my);
    void Activate();
};
#endif
