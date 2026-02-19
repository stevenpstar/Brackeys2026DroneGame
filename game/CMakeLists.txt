cmake_minimum_required(VERSION 3.5)
project(dronebrackeys)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(${PROJECT_NAME} SHARED
  src/game.cpp
  src/logic.cpp
  src/render.cpp
  src/loadResources.cpp
  src/renderStaticModels.cpp
  src/glad.c
  src/controllers/fpsController.cpp
  src/controllers/droneController.cpp
  src/testDroneCharacter.cpp
  src/snowworm.cpp
 )

 target_compile_definitions(${PROJECT_NAME} PRIVATE BANE_GAME_EXPORTS)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)

target_link_directories(${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/lib
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  bane
  SDL3
  SDL3_ttf
  SDL3_image
  SDL3_mixer
  assimp-vc143-mt
)
