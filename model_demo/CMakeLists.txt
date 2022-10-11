add_executable(model_demo main.cpp)
target_link_libraries(model_demo PRIVATE common common_assimp fmt::fmt
    glad::glad glfw glm::glm)

symlink_to_output(model_demo "${RESOURCES_DIR}")
symlink_to_output(model_demo "${CMAKE_CURRENT_SOURCE_DIR}")
