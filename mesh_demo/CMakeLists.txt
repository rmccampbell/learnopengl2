add_executable(mesh_demo main.cpp)
target_link_libraries(mesh_demo PRIVATE common fmt::fmt glad::glad glfw glm::glm)

copy_resources_to_output(mesh_demo
    "${RESOURCES_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/resources")
