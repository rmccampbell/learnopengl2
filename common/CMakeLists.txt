add_library(common shader.cpp shader.h texture.cpp texture.h mesh.cpp mesh.h
    material.cpp material.h lights.h model.cpp model.h handle.h scope_guard.h
    cstring_view.h errutils.h glutils.h utils.h u8tils.h)
target_include_directories(common PUBLIC "..")
target_link_libraries(common PUBLIC fmt::fmt glad::glad glfw glm::glm)
target_link_libraries(common PRIVATE stb_image)

add_library(common_assimp assimp_loader.cpp assimp_loader.h)
target_include_directories(common_assimp PUBLIC "..")
target_link_libraries(common_assimp PUBLIC common assimp::assimp)
