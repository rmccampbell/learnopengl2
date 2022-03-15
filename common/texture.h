#ifndef TEXTURE_H
#define TEXTURE_H

#include <filesystem>

#include <glad/glad.h>

GLuint load_texture(const std::filesystem::path& path, bool flip = true);

#endif // TEXTURE_H
