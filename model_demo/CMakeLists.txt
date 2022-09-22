add_executable(model_demo main.cpp)
target_link_libraries(model_demo PRIVATE common common_assimp fmt::fmt
    glad::glad glfw glm::glm)

copy_resources_to_output(model_demo "${RESOURCES_DIR}")
