add_executable(mesh_demo main.cpp)
target_link_libraries(mesh_demo PRIVATE common fmt::fmt glad::glad glfw glm::glm)

symlink_to_output(mesh_demo "${RESOURCES_DIR}")
symlink_to_output(mesh_demo "${CMAKE_CURRENT_SOURCE_DIR}")
