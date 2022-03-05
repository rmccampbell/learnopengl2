cmake_minimum_required(VERSION 3.0.0)
project(learnopengl2 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(assimp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STBI_INCLUDE_DIR "stb_image.h")

add_executable(learnopengl2 main.cpp shader.h texture.h mesh.h errutil.h util.h)
target_link_libraries(learnopengl2 PRIVATE assimp::assimp fmt::fmt glad::glad glfw glm::glm)
target_include_directories(learnopengl2 PRIVATE ${STBI_INCLUDE_DIR})
