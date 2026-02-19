#include "SDL3/SDL_gamepad.h"
#include "SDL3/SDL_joystick.h"
#include "bane/core/scene.hpp"
#include "bane/primitives/model.hpp"
#include "game/controllers/droneController.hpp"
#include "game/controllers/fpsController.hpp"
#include "game/loadResources.hpp"
#include "game/renderStaticModels.hpp"
#include "game/settings.hpp"
#include "game/snowworm.hpp"
#include "game/testDroneCharacter.hpp"
#include "glm/geometric.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mixer.h>
#include <SDL3/SDL_timer.h>
#include <algorithm>
#include <bane/bane.hpp>
#include <bane/components/bobject.hpp>
#include <bane/components/staticmesh.hpp>
#include <bane/data/resource.hpp>
#include <bane/primitives/texture.hpp>
#include <bane/utility/AABB.hpp>
#include <bane/utility/ray.hpp>
#include <bane/utility/shader.hpp>
#include <cstdio>
#include <game/data.hpp>
#include <game/game.hpp>
#include <game/logic.hpp>
#include <game/render.hpp>
#include <glad/glad.h>
// Shaders
#include <bane/shaders/fbo_vert.glsl>
#include <bane/shaders/rgb_tri_frag.glsl>
#include <bane/shaders/rgb_tri_vertex.glsl>
#include <bane/shaders/shadow_anim_vert.glsl>
#include <bane/shaders/shadow_frag.glsl>
#include <bane/shaders/shadow_vert.glsl>
#include <bane/shaders/simple_light_frag.glsl>
#include <bane/shaders/simple_light_vert.glsl>
#include <bane/shaders/simple_pixel_frag.glsl>
#include <game/anim_vert.glsl>
#include <game/basic_frag.glsl>
#include <game/drone_frag.glsl>

// End Shaders

#include <iostream>
#include <memory>
SDL_Event e;

Camera testCamera = Camera(glm::vec3(0.f, 0.f, 0.f));
std::unique_ptr<GameData> gameData = std::make_unique<GameData>(
    GameData{.window = nullptr,
             .resources = std::make_unique<Resources>(Resources{}),
             .scenes = std::vector<Scene>(),
             .settings = std::make_unique<GameSettings>(GameSettings{})});

std::unique_ptr<DroneController> playerController;
std::unique_ptr<FpsController> fpsController;
std::unique_ptr<StaticMesh> testModel;
std::vector<std::unique_ptr<SnowWorm>> SnowWorms;
SDL_Gamepad *gamepad;
bool drawDebug = true;
unsigned char *snowPixels = new unsigned char[128 * 128 * 3];
int snowIndexTest = -1;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
float deltaTime = 0.f;
bool gameRunning = true;
int winWidth = 1920;
int winHeight = 1080;

void resize_callback(SDL_Window *window, int width, int height) {
  winWidth = width;
  winHeight = height;
  ResizeViewport(width, height);
  gameData->resources->camera.setProjection(width, height, 65.f);
}

void Init() {
  std::cout << "Version 0.11";
  SDL_Window *window = nullptr;
  SDL_Event e;
  SDL_zero(e);
  window = CreateWindow();
  if (window == nullptr) {
    std::cout << "Window creation failed\n";
  }

  SDL_AudioSpec spec = {
      .freq = MIX_DEFAULT_FREQUENCY,
      .format = MIX_DEFAULT_FORMAT,
      .channels = MIX_DEFAULT_CHANNELS,
  };

  if (!Mix_OpenAudio(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec)) {
    SDL_Log("Mixer not initialised properly!\n");
  }

  SDL_GL_SetSwapInterval(1);

  Mix_AllocateChannels(16);

  gameData->window = window;
  if (gameData->window == nullptr) {
    std::cout << "We have an issue!\n";
  }
  SetupFrameBuffer(gameData->window, 1920, 1080);
  SetupShadowBuffer(window, 2048, 2048);

  if (SDL_HasGamepad()) {
    std::cout << "----------------->Gamepad found!\n";
    int count = 0;
    SDL_JoystickID *ids = SDL_GetGamepads(&count);
    for (int i = 0; i < count; ++i) {
      SDL_Gamepad *gamepd = SDL_OpenGamepad(ids[i]);
      if (gamepd) {
        gamepad = gamepd;
      }

      if (i > 0) {
        SDL_CloseGamepad(gamepd);
      }
    }
  } else {
    std::cout << "----------------->NO Gamepad found!\n";
  }

  LoadGameAssets(gameData);
}
void InitGame() {
  SDL_zero(e);
  // Init SDL
  Shader pixelShader = Shader(fbo_vert, simple_pixel_frag, "simplePixelShader");
  Shader shadowShader(shadow_vert, shadow_frag, "shadowShader");
  Shader animShadowShader(shadow_anim_vert, shadow_frag, "animShadowShader");
  Shader simpleShader(simple_light_vert, basic_frag, "simpleShader");
  Shader simpleAnimShader(anim_vert_shader, basic_frag, "animShader");
  Shader droneShader(anim_vert_shader, drone_frag, "droneShader");
  Shader animDebugShader(anim_vert_shader, rgb_tri_frag, "animDebugShader");

  gameData->resources->Shaders.emplace_back(
      std::make_unique<Shader>(pixelShader));
  gameData->resources->Shaders.emplace_back(
      std::make_unique<Shader>(shadowShader));
  gameData->resources->Shaders.emplace_back(
      std::make_unique<Shader>(simpleShader));
  gameData->resources->Shaders.emplace_back(std::make_unique<Shader>(
      Shader(rgb_tri_vert_shader_src, rgb_tri_frag, "gridShader")));
  gameData->resources->Shaders.emplace_back(
      std::make_unique<Shader>(simpleAnimShader));

  gameData->resources->Shaders.emplace_back(
      std::make_unique<Shader>(animDebugShader));

  gameData->resources->Shaders.emplace_back(
      std::make_unique<Shader>(animShadowShader));

  gameData->resources->Shaders.emplace_back(
      std::make_unique<Shader>(droneShader));

  gameData->resources->camera.setPosition(glm::vec3(0.f, 1.2f, 3.f));
  gameData->resources->camera.setProjection(1920, 1080, 65.f);

  // Setting up controller, controller maybe needs more gamedata I don't know
  // at this point! TODO, implementation is prototype anyway.
  // testModel = std::make_unique<Bobject>(Bobject());
  // testModel->textureId = createTexture(
  //     (gameData->settings->directory +
  //     "../game/assets/textures/colormap.png")
  //         .c_str());

  // Load initial scene
  if (gameData->scenes.size() > 0) {
    // TODO: Should load a scene based on a project setting (default/starting
    // scene). 0 for now
    loadGameScene(gameData->scenes.at(0).sceneName, gameData);
    std::cout << "Scene loaded!\n";
    //   gameData->resources->Models.emplace_back(testModel);
  }

  playerController = std::make_unique<DroneController>(DroneController());
  playerController->SetCharacter(gameData->settings->directory.c_str(),
                                 gameData);
  for (int i = 0; i < gameData->resources->Colliders.size(); ++i) {
    for (const auto &tag : gameData->resources->Colliders.at(i)->tags) {
      if (tag == "drone") {
        playerController->setColliderIndex(i);
        break;
      }
    }
  }

  fpsController = std::make_unique<FpsController>(FpsController());
  // RIP Snow worms the animations would not work :(
  // SnowWorms.emplace_back(std::make_unique<SnowWorm>(SnowWorm(SnowWorm(
  //     gameData->settings->directory.c_str(), gameData, "snowworm_1", 600))));
  // GLubyte *ptr = (GLubyte *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER,
  // GL_WRITE_ONLY);

  for (int i = 0; i < gameData->resources->Models.size(); ++i) {
    if (gameData->resources->Models.at(i)->id == 1) {
      std::cout << "ABOUT TO UPDATE SHIT MATE!\n";
      snowIndexTest = i;
      for (size_t i = 0; i < 128 * 128 * 3; i += 3) {
        snowPixels[i] = 255; // rand() % 256;
        // std::cout << "snowpixels: " << i << ": " <<
        //       (int)snowPixels[i] <<
        // "\n";
        snowPixels[i + 1] = 255; // rand() % 256;
        snowPixels[i + 2] = 255; // rand() % 256;
        // snowPixels[i + 3] = 255;
      }

      //   std::cout << "ERROR: " << glGetError() << "\n";
      // }
      //    //   GLint format;
      //    //   glGetTexLevelParameteriv(GL_TEXTURE_2D, 0,
      //    //   GL_TEXTURE_INTERNAL_FORMAT,
      //    //                            &format);
      //    //   std::cout << "format: " << format << "\n";

      //    // GLint texLevel;
      //    // glGetTexLevelParameteriv(GL_TEXTURE_2D, 0,
      //    GL_TEXTURE_WIDTH,
      //    // &texLevel); std::cout << "texLevel: " << texLevel
      //    << "\n";

      //    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      //    //
      //    glTextureStorage2D(gameData->resources->Models.at(i)->heightMap2Id,
      //    0,
      //    //                    GL_RGB, 128, 128);
      //    //
      //    glTextureSubImage2D(gameData->resources->Models.at(i)->heightMap2Id,
      //    0,
      //    // 0,
      //    //                  0, 128, 128, GL_RGB,
      //    GL_UNSIGNED_BYTE, snowPixels);

      //    //   std::cout << "Error: " << glGetError() << "\n";

      //    //   //  std::cout << "snowPixels size: " <<
      //    sizeof(snowPixels) <<
      //    "\n";
      //    //  size_t elmes_per_line = 2048 * 3;
      //    //  size_t row = y * elmes_per_line;
      //    //  size_t col = x * 3;
      //    //  size_t index = row + col;
      //    //  index = (y * 2048 + x) * 3;
      //    //  // std::cout << "r channel at x, y: " << x << ",
      //    " << y << ":
      //    "
      //    //  //           << (int)snowPixels[index] <<
      //    "index: " << index
      //    <<
      //    "\n";
      //    //  snowPixels[index] = 0;
      //    //  snowPixels[index + 1] = 0;
      //    //  snowPixels[index + 2] = 0;
      //    // snowPixels[index + 3] = 0;

      //   gameData->resources->Shaders.at(ind)->setInt("heightMap2",
      //   4);
      break;
    }
  }
}

bool Loop() {
  LAST = NOW;
  NOW = SDL_GetPerformanceCounter();
  deltaTime = ((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_EVENT_QUIT) {
      gameRunning = false;
    } else if (e.type == SDL_EVENT_WINDOW_RESIZED) {
      resize_callback(gameData->window, winWidth, winHeight);
    } else if (e.type == SDL_EVENT_MOUSE_MOTION) {
      //  float mposx = e.motion.x;
      //  float mposy = e.motion.y;
      //  fpsController->processMouse(gameData->window, e.motion.xrel,
      //                              e.motion.yrel, gameData);

      //  glm::vec3 rayDir = getRayFromMouse(&gameData->resources->camera, 1920,
      //                                     1080, 1920 / 2.f, 1080 / 2.f);
      //  bool hovering_usable = false;
      //  unsigned int count = 0;
      //  float dist = 999.f;
      //  for (auto &collider : gameData->resources->Colliders) {
      //    glm::vec3 hitPoint;
      //    if (rayWithAABB(collider.get(), rayDir,
      //                    gameData->resources->camera.getPosition(),
      //                    hitPoint)) {
      //      float hit_dist = glm::distance(
      //          gameData->resources->camera.getPosition(), hitPoint);
      //      if (hit_dist < dist &&
      //          std::find(collider->tags.begin(), collider->tags.end(),
      //                    "cabinet") != collider->tags.end()) {
      //        dist = hit_dist;

      //        hovering_usable = true;
      //        for (int i = 0; i < gameData->resources->Models.size(); ++i) {
      //          if (collider->ownerId ==
      //          gameData->resources->Models.at(i)->id) {
      //            gameData->state.hoveredModelIndex = i;
      //          }
      //        }
      //      }
      //    }
      //    count++;
      //  }
      //  if (!hovering_usable) {
      //    gameData->state.hoveredModelIndex = -1;
      //  }
    }
    if (e.type == SDL_EVENT_KEY_UP) {
      playerController->processInput(gameData->window, e.key.key, false,
                                     gameData);
      std::cout << "Button pressed: "
                << SDL_GetGamepadStringForButton(
                       (SDL_GamepadButton)e.gbutton.button)
                << "\n";
    }
    if (e.type == SDL_EVENT_KEY_DOWN) {
      if (e.key.key == SDLK_ESCAPE) {
        gameRunning = false;
      }
      playerController->processInput(gameData->window, e.key.key, true,
                                     gameData);
    }

    if (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
      playerController->processGamepadButtonInput(
          gameData->window,
          SDL_GetGamepadStringForButton((SDL_GamepadButton)e.gbutton.button),
          true, gameData);
    }
    if (e.type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
      playerController->processGamepadButtonInput(
          gameData->window,
          SDL_GetGamepadStringForButton((SDL_GamepadButton)e.gbutton.button),
          false, gameData);
    }
    if (e.type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
      if (e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY) {
        playerController->processGamepadJoystickInput(gameData->window, "x",
                                                      "left", e.gaxis.value);
      }
      if (e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX) {
        playerController->processGamepadJoystickInput(gameData->window, "y",
                                                      "left", e.gaxis.value);
      }
      if (e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX) {
        playerController->processGamepadJoystickInput(gameData->window, "x",
                                                      "right", e.gaxis.value);
      }
      if (e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY) {
        playerController->processGamepadJoystickInput(gameData->window, "y",
                                                      "right", e.gaxis.value);
      }
      if (e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
        playerController->processGamepadTrigger(gameData->window, "left",
                                                e.gaxis.value);
      }
      if (e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
        playerController->processGamepadTrigger(gameData->window, "right",
                                                e.gaxis.value);
      }
    }
  }

  playerController->update(deltaTime, gameData);
  fpsController->update(deltaTime, gameData);

  Update(gameData, deltaTime);
  // Render(gameData, deltaTime);

  Poll();

  playerController->droneCharacter->droneModel->UpdateAnimation(deltaTime);
  Camera testCamera = Camera(glm::vec3(0.f, 0.f, 0.f));
  gameData->resources->lightData.lightSpaceMatrix = RenderShadow(
      gameData->window, 2048, 2048,
      playerController->droneCharacter->droneModel->transform.position +
          gameData->resources->lightData.dirLight,
      playerController->droneCharacter->droneModel->transform.position,
      gameData->resources->Shaders
          .at(GetShader(gameData->resources, "shadowShader"))
          .get(),
      &testCamera);

  renderShadowedModels(gameData->resources);

  playerController->droneCharacter->renderDroneShadow(gameData);

  SwitchBuffer();
  ClearColour();
  if (snowIndexTest >= 0) {

    playerController->setSnowPixels(snowPixels, gameData);

    int shaderIndex = GetShader(gameData->resources, "simpleShader");

    gameData->resources->Shaders.at(shaderIndex)->use();

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D,
                  gameData->resources->Models.at(snowIndexTest)->heightMap2Id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 128, 128, GL_RGB, GL_UNSIGNED_BYTE,
                    snowPixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // if (gameData->resources->Models.at(snowIndexTest)->hasHeightMap2) {
    //   glActiveTexture(GL_TEXTURE4);
    //   glBindTexture(
    //       GL_TEXTURE_2D,
    //       gameData->resources->Models.at(snowIndexTest)->heightMap2Id);
    //   gameData->resources->Shaders.at(shaderIndex)
    //       ->setBool(
    //           "hasHeightMap2",
    //           gameData->resources->Models.at(snowIndexTest)->hasHeightMap2);
    //   gameData->resources->Shaders.at(shaderIndex)
    //       ->setFloat("height2Intensity", 2.f);
    // }
  }
  renderModels(gameData);
  for (const auto &worm : SnowWorms) {
    worm->Render(gameData);
  }

  playerController->droneCharacter->Render(gameData);

  if (drawDebug) {
    RenderColliders(gameData);
  }

  // playerController->droneCharacter->Render(gameData);
  int shaderIndex = GetShader(gameData->resources, "simplePixelShader");
  gameData->resources->Shaders.at(shaderIndex)->use();
  gameData->resources->Shaders.at(shaderIndex)->setInt("size", 1);
  gameData->resources->Shaders.at(shaderIndex)->setFloat("width", 1920);
  gameData->resources->Shaders.at(shaderIndex)->setFloat("height", 1080);

  RenderToScreenTexture(gameData->resources->Shaders.at(shaderIndex).get());
  SwapBuffer(gameData->window);

  return gameRunning;
}
void Cleanup() {
  delete[] snowPixels;
  DestroyBane();
}
