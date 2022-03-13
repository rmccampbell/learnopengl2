add_executable(texture_demo main.cpp)
target_link_libraries(texture_demo PRIVATE common fmt::fmt glad::glad glfw glm::glm)

copy_resources_to_output(texture_demo "${RESOURCES_DIR}")
copy_resources_to_output(texture_demo "${CMAKE_CURRENT_SOURCE_DIR}/resources")
