#ifndef BANE_GAME_TEMPLATE_LOAD_RESOURCES
#define BANE_GAME_TEMPLATE_LOAD_RESOURCES

#include <game/data.hpp>
#include <memory>
void loadTextures(std::unique_ptr<GameData> &gameData);
void LoadGameAssets(std::unique_ptr<GameData> &gameData);
void loadGameScene(std::string sceneName, std::unique_ptr<GameData> &gameData);
unsigned int
getTextureByName(std::string name,
                 const std::vector<std::unique_ptr<Texture>> &textures);

#endif
