add_library(common stb_image.cpp shader.h texture.h mesh.h errutil.h util.h)
target_include_directories(common PUBLIC ..)
target_link_libraries(common PRIVATE assimp::assimp fmt::fmt glad::glad glfw glm::glm)
target_include_directories(common PRIVATE ${STB_IMAGE_INCLUDE_DIR})
