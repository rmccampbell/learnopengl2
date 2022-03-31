#ifndef TEXTURE_H
#define TEXTURE_H

#include <filesystem>

#include <glad/glad.h>

#include "handle.h"

struct texture_deleter {
    void operator()(GLuint t) { glDeleteTextures(1, &t); }
};

using TextureHandle = Handle<GLuint, texture_deleter>;

struct TextureConfig {
  bool flip = true;
  bool srgb = false;
  bool gen_mipmaps = true;
  GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR;
  GLenum mag_filter = GL_LINEAR;
  GLenum wrap = GL_REPEAT;
  float anisotropy = 8.f;
};

GLuint load_texture(const std::filesystem::path& path, const TextureConfig& config = {});

class Texture {
  public:
    explicit Texture(GLuint id = 0, const std::filesystem::path& filename = {})
        : id_(id), filename_(filename) {}
    explicit Texture(const std::filesystem::path& path, const TextureConfig& config = {})
        : id_(load_texture(path, config)), filename_(path) {}

    GLuint id() const { return id_; }
    std::filesystem::path filename() const { return filename_; }

    explicit operator bool() const { return id_ != 0; }
    bool empty() const { return id_ == 0; }

    void bind() const { glBindTexture(GL_TEXTURE_2D, id_); }
    void bind(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        bind();
    }

    void set_param(GLenum pname, GLint param) {
        glTextureParameteri(id_, pname, param);
    }
    void set_param(GLenum pname, GLfloat param) {
        glTextureParameterf(id_, pname, param);
    }
    void set_param(GLenum pname, GLint* param) {
        glTextureParameteriv(id_, pname, param);
    }
    void set_param(GLenum pname, GLfloat* param) {
        glTextureParameterfv(id_, pname, param);
    }

    friend std::ostream& operator<<(std::ostream& os, const Texture& texture);

  private:
    TextureHandle id_;
    std::filesystem::path filename_;
};

#endif // TEXTURE_H
