#include "texture.h"

#include <array>
#include <filesystem>

#include <glad/glad.h>
#include <stb_image.h>

#include "errutils.h"
#include "u8tils.h"

GLuint load_texture(const std::filesystem::path& path, const TextureOpts& opts) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(opts.flip);
    unsigned char* data =
        stbi_load(u8::path_to_char(path), &width, &height, &channels, 0);
    err::check(data, "failed to load texture {}: {}", path.string(),
               stbi_failure_reason());

    GLenum format = std::array{GL_RED, GL_RG, GL_RGB, GL_RGBA}[channels - 1];
    GLint internal_format = std::array{
        GL_R8,
        GL_RG8,
        opts.srgb ? GL_SRGB8 : GL_RGB8,
        opts.srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8
    }[channels - 1];

    TextureHandle id;
    glGenTextures(1, &id.reset_as_ref());
    glBindTexture(GL_TEXTURE_2D, *id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    if (opts.gen_mipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, opts.min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, opts.mag_filter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, opts.anisotropy);
    // set swizzle mask for grayscale
    if (channels == 1) {
        GLint swizzle_mask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
    } else if (channels == 2) {
        GLint swizzle_mask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
    }
    return id.release();
}

std::ostream& operator<<(std::ostream& os, const Texture& texture) {
    os << "Texture { " << texture.id();
    if (texture)
        os << ": " << texture.filename_;
    return os << " }";
}
