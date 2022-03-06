add_executable(texture_demo main.cpp)
target_link_libraries(texture_demo PRIVATE common fmt::fmt glad::glad glfw glm::glm)

copy_resources_dir(texture_demo)
