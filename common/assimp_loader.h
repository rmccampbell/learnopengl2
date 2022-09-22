#ifndef ASSIMP_LOADER_H
#define ASSIMP_LOADER_H

#include <assimp/postprocess.h>

#include "model.h"

inline constexpr int DEFAULT_FLAGS =
    aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate |
    aiProcess_PreTransformVertices;

Model load_model(const std::filesystem::path& path, unsigned int flags = DEFAULT_FLAGS);

#endif // ASSIMP_LOADER_H
