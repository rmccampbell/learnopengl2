#ifndef SHADER_H
#define SHADER_H

#include <filesystem>

#include <glad/glad.h>

GLuint build_shader(const char* vs_source, const char* fs_source,
                    const char* gs_source = nullptr);

GLuint load_shader(const std::filesystem::path& vs_path,
                   const std::filesystem::path& fs_path,
                   const std::filesystem::path& gs_path = {});

#endif // SHADER_H
