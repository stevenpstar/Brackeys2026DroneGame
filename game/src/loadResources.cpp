#include <bane/scene/sceneLoader.hpp>
#include <filesystem>
#include <game/loadResources.hpp>
#include <iostream>
#include <sstream>
#include <string>

void loadTextures(std::unique_ptr<GameData> &gameData) {

  std::string textureDir =
      gameData->settings->directory + "../game/assets/textures/";
  gameData->resources->Textures.clear();
  std::filesystem::path target(textureDir);
  try {
    for (auto const &textureFile :
         std::filesystem::directory_iterator(target)) {
      if (std::filesystem::is_regular_file(textureFile.path())) {
        if (textureFile.path().filename() != "" &&
                (textureFile.path().filename().extension().string() ==
                 ".png") ||
            (textureFile.path().filename().extension().string() == ".jpg")) {
          Texture texture;
          texture.name = textureFile.path().filename().string().substr(
              0, textureFile.path().filename().string().find('.'));
          // TODO: THIS IS A TEST
          texture.id = createTexture(textureFile.path().string().c_str());

          gameData->resources->Textures.push_back(
              std::make_unique<Texture>(texture));
        }
      }
    }
  } catch (const std::filesystem::filesystem_error &error) {
  }
}

void LoadGameAssets(std::unique_ptr<GameData> &gameData) {
  // clear out data
  gameData->resources->Models.clear();
  gameData->resources->Shaders.clear();
  gameData->resources->Colliders.clear();
  gameData->resources->AnimatedModel.clear();
  gameData->scenes.clear();

  // load scenes
  std::string scenePath =
      gameData->settings->directory + "../game/assets/scenes/";
  std::filesystem::path target(scenePath);

  try {
    for (auto const &sceneFile : std::filesystem::directory_iterator(target)) {
      if (std::filesystem::is_regular_file(sceneFile.path())) {
        if (sceneFile.path().filename() != "" &&
            sceneFile.path().filename().extension().string() == ".json") {
          Scene loadedScene = loadScene(sceneFile.path().string());
          gameData->scenes.push_back(loadedScene);
        }
      }
    }

  } catch (const std::filesystem::filesystem_error &error) {
    std::cout << "Error loading scene files at path: " << scenePath
              << "error: " << error.what() << "\n";
  }
}

unsigned int
getTextureByName(std::string name,
                 const std::vector<std::unique_ptr<Texture>> &textures) {
  for (const auto &texture : textures) {
    if (texture->name == name) {
      return texture->id;
    }
  }
  std::cout << "could not find texture by name: " << name << "\n";
  return 0;
}

void loadGameScene(std::string sceneName, std::unique_ptr<GameData> &gameData) {

  Scene scene = loadScene(gameData->settings->directory +
                          "../game/assets/scenes/" + sceneName + ".json");
  gameData->resources->lightData.ambientStrength = scene.ambientStrength;
  gameData->resources->lightData.dirLight = scene.lightDirection;
  gameData->resources->lightData.sunIntensity = scene.sunlightIntensity;
  gameData->resources->Colliders.clear();
  gameData->resources->Models.clear();
  gameData->resources->AnimatedModel.clear();
  gameData->resources->Textures.clear();
  gameData->resources->lightData.pointLights.clear();

  loadTextures(gameData);
  for (auto bobj : scene.staticModels) {
    gameData->resources->Models.emplace_back(std::make_unique<StaticMesh>(
        StaticMesh((bobj.filePath).c_str(), bobj.name, bobj.id)));
    std::vector<std::string> substrings;
    std::string temp, fileName;
    std::stringstream stream(bobj.filePath);
    while (getline(stream, temp, '/')) {
      substrings.push_back(temp);
    }
    if (substrings.size() == 0) {
      fileName = "";
    } else {
      fileName = substrings.at(substrings.size() - 1);
    }

    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->fileName = fileName;
    //    bobj.file
    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->transform.position = bobj.position;
    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->transform.rotation = bobj.rotation;
    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->transform.scale = bobj.scale;
    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->textureId =
        getTextureByName(bobj.diffuse, gameData->resources->Textures);
    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->castShadow = bobj.castShadow;

    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->specularId =
        getTextureByName(bobj.specular, gameData->resources->Textures);

    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->heightMapId =
        getTextureByName(bobj.heightMap, gameData->resources->Textures);

    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->heightMap2Id =
        getTextureByName(bobj.heightMap2, gameData->resources->Textures);

    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->texture2Id =
        getTextureByName(bobj.diffuse2, gameData->resources->Textures);

    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->diffuseMix = bobj.diffuseMix;

    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->hasSpecularMap = bobj.hasSpecularMap;
    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->hasHeightMap = bobj.hasHeightMap;
    if (bobj.diffuse == "snowworm") {
      std::cout << "Does snow worm have a height map 1: " << bobj.hasHeightMap
                << "\n";
    }
    gameData->resources->Models.at(gameData->resources->Models.size() - 1)
        ->hasHeightMap2 = bobj.hasHeightMap2;
  }

  // TEST MANUAL MODEL GUIHEOHGOIEHGIOEHGOIEIHG

  //  gameData->resources->Models.emplace_back(
  //      std::make_unique<StaticMesh>(StaticMesh(
  //          "F:/Projects/BrackeysDroneGame/game/assets/static_models/cube.glb",
  //          "cubeunqiue", 555)));
  //  gameData->resources->Models.at(gameData->resources->Models.size() - 1)
  //      ->transform.position = glm::vec3(10.f, 0.f, 0.f);
  //  gameData->resources->Models.at(gameData->resources->Models.size() - 1)
  //      ->transform.rotation = glm::vec3(0.f, 0.f, 0.f);
  //  gameData->resources->Models.at(gameData->resources->Models.size() - 1)
  //      ->transform.scale = glm::vec3(1.f, 1.f, 1.f);
  //  gameData->resources->Models.at(gameData->resources->Models.size() - 1)
  //      ->textureId =
  //      getTextureByName("woodcrate", gameData->resources->Textures);
  //  gameData->resources->Models.at(gameData->resources->Models.size() - 1)
  //      ->castShadow = false;

  //

  for (auto light : scene.pointLights) {
    gameData->resources->lightData.pointLights.emplace_back(
        std::make_unique<PointLight>(light));
  }

  for (auto aabb : scene.colliders) {
    gameData->resources->Colliders.emplace_back(std::make_unique<AABB>(aabb));
  }
}
