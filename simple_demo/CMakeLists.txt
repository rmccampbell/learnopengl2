add_executable(simple_demo main.cpp)
target_link_libraries(simple_demo PRIVATE common fmt::fmt glad::glad glfw glm::glm)
