#ifndef TEXTURE_H
#define TEXTURE_H

#include <filesystem>

#include <glad/glad.h>

#include "handle.h"

using TextureHandle = Handle<GLuint, gl_delete_array_functor<glDeleteTextures>>;

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
