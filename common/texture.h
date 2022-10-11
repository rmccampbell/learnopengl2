#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <filesystem>

#include <glad/glad.h>

#include "raii.h"

using TextureHandle = Handle<GLuint, gl_delete_array_functor<glDeleteTextures>>;

struct TextureOpts {
    bool flip = true;
    bool srgb = false;
    bool gen_mipmaps = true;
    GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum mag_filter = GL_LINEAR;
    GLenum wrap = GL_REPEAT;
    float anisotropy = 8.f;
};

GLuint load_texture(const std::filesystem::path& path, const TextureOpts& opts = {});

class Texture {
  public:
    Texture() : Texture(0, {}) {}
    explicit Texture(GLuint id, const std::filesystem::path& filename = {})
        : id_(id), filename_(filename) {}
    explicit Texture(const std::filesystem::path& path, const TextureOpts& opts = {})
        : Texture(load_texture(path, opts), path) {}

    GLuint id() const { return id_.get(); }
    std::filesystem::path filename() const { return filename_; }

    explicit operator bool() const { return id() != 0; }
    bool empty() const { return id() == 0; }

    void bind() const { glBindTexture(GL_TEXTURE_2D, id()); }
    void bind(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        bind();
    }

    void set_param(GLenum pname, GLint param) {
        glTextureParameteri(id(), pname, param);
    }
    void set_param(GLenum pname, GLfloat param) {
        glTextureParameterf(id(), pname, param);
    }
    void set_param(GLenum pname, GLint* param) {
        glTextureParameteriv(id(), pname, param);
    }
    void set_param(GLenum pname, GLfloat* param) {
        glTextureParameterfv(id(), pname, param);
    }

    friend std::ostream& operator<<(std::ostream& os, const Texture& texture);

  private:
    TextureHandle id_;
    std::filesystem::path filename_;
};

#endif // TEXTURE_H
