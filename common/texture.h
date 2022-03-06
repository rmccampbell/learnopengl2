#ifndef TEXTURE_H
#define TEXTURE_H

#include <array>
#include <filesystem>

#include <glad/glad.h>
#include <stb_image.h>

#include "errutil.h"

inline GLuint load_texture(std::filesystem::path path, bool flip = true) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data =
        stbi_load(path.u8string().c_str(), &width, &height, &channels, 0);
    err::check(data, "failed to load texture {}: {}", path.string(),
               stbi_failure_reason());

    GLenum format = std::array{0, GL_RED, GL_RG, GL_RGB, GL_RGBA}[channels];
    GLenum iformat = std::array{0, GL_R8, GL_RG8, GL_RGB8, GL_RGBA8}[channels];

    GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
    // set swizzle mask for grayscale
    if (channels == 1) {
        GLint swizzle_mask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
    } else if (channels == 2) {
        GLint swizzle_mask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
    }
    return id;
}

#endif // TEXTURE_H
