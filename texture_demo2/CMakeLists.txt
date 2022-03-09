add_executable(texture_demo2 main.cpp)
target_link_libraries(texture_demo2 PRIVATE common fmt::fmt glad::glad glfw glm::glm)

copy_resources_to_output(texture_demo2 "${RESOURCES_DIR}")
copy_resources_to_output(texture_demo2 "${CMAKE_CURRENT_SOURCE_DIR}/resources")
