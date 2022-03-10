add_library(common stb_image.cpp shader.h texture.h mesh.h errutils.h utils.h u8tils.h)
target_include_directories(common PUBLIC ..)
target_link_libraries(common PUBLIC fmt::fmt glad::glad glfw glm::glm)
target_include_directories(common PUBLIC ${STB_IMAGE_INCLUDE_DIR})
