#ifndef BANE_ENGINE
#define BANE_ENGINE
#include "SDL3/SDL_ttf.h"
#include "glm/fwd.hpp"
#include <bane/utility/shader.hpp>
#include <glm/glm.hpp>
#include <string>
class Camera;
struct SDL_Window;
void Print(std::string message);
SDL_Window *CreateWindow();
bool ShouldClose(SDL_Window *window);
float GetTime();
void Poll();
void ClearColour();
void SwapBuffer(SDL_Window *window);
void DestroyBane();
void SetCursorCallback(SDL_Window *window, void (*f)(SDL_Window *window, double xpos, double ypos));
void SetResizeCallback(SDL_Window *window, void (*f)(SDL_Window *window, int width, int height));
void SetMouseInputCallback(SDL_Window *window, void (*f)(SDL_Window *window, int button, int action, int mods));
void ResizeViewport(int width, int height);
void CaptureMouse(SDL_Window *window);
void RenderColouredTriangle(Shader *shader, std::array<float, 18> &triangle);
void SetupShadowBuffer(SDL_Window *window, int width, int height);
void SetupGBuffer(SDL_Window *window, int width, int height);
glm::mat4 RenderShadow(SDL_Window *window, int width, int height, glm::vec3 lightPos, Shader *shader, Camera *camera);
glm::mat4 RenderShadow(SDL_Window *window, int width, int height, glm::vec3 lightPos, glm::vec3 lookAtPos, Shader *shader,
                       Camera *camera);
unsigned int getShadowTexture();
void SetupFrameBuffer(SDL_Window *window, int width, int height);
void UpdateFrameBufferSize(int width, int height);
void SwitchBuffer();
void RenderToScreenTexture(Shader *screenShader);
void GetScreenDimensions(SDL_Window *window, int &width, int &height);
void SetCursorOnOff(bool on);
void SetCursorImage(const char *path);
SDL_GLContext *ReturnSDLContext();
// TEST FUNCTIONS FOR TEXT, if works we move to other file
unsigned int CreateTextureFromString(std::string text, TTF_Font *font, int &width, int &height);

#endif
