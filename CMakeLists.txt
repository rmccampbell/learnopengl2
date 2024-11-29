cmake_minimum_required(VERSION 3.20.0)
project(learnopengl2 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable wmain on MinGW
if(MINGW)
    add_link_options(-municode)
endif()

find_package(assimp CONFIG)
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

if(assimp_FOUND)
    add_compile_definitions(HAS_ASSIMP)
endif()

set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")

function(symlink_to_output target path)
    get_filename_component(basename "${path}" NAME)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
                "${path}"
                "$<TARGET_FILE_DIR:${target}>/${basename}"
        VERBATIM
    )
endfunction()

add_subdirectory(third_party)
add_subdirectory(common)
add_subdirectory(simple_demo)
add_subdirectory(texture_demo)
add_subdirectory(mesh_demo)
add_subdirectory(model_demo)
add_subdirectory(temp)
