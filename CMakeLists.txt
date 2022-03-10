cmake_minimum_required(VERSION 3.13.0)
project(learnopengl2 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable wmain on MinGW
if(MINGW)
    add_link_options(-municode)
endif()

# find_package(assimp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_IMAGE_INCLUDE_DIR "stb_image.h")

set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")

function(copy_resources_to_output target dir)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${dir}"
                "$<TARGET_FILE_DIR:${target}>/resources"
    )
endfunction(copy_resources_to_output)

add_subdirectory(common)
add_subdirectory(simple_demo)
add_subdirectory(texture_demo)
add_subdirectory(texture_demo2)
