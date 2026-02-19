cmake_minimum_required(VERSION 3.5)
project(dronebrackeysexec)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/glad.c
 )

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
  dronebrackeys
  SDL3
  SDL3_ttf
  SDL3_image
  SDL3_mixer
)
