add_library(common shader.cpp shader.h texture.cpp texture.h mesh.h errutils.h utils.h u8tils.h)
target_include_directories(common PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(common PUBLIC fmt::fmt glad::glad glfw glm::glm)
target_link_libraries(common PRIVATE stb_image)
