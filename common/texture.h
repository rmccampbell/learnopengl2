#ifndef TEXTURE_H
#define TEXTURE_H

#include <filesystem>

#include <glad/glad.h>

#include "handle.h"

using TextureHandle = Handle<GLuint, decltype([](GLuint t) { glDeleteTextures(1, &t); })>;

GLuint load_texture(const std::filesystem::path& path, bool flip = true);

class Texture {
  public:
    explicit Texture(GLuint id = 0, const std::filesystem::path& filename = {})
        : id_(id), filename_(filename) {}
    explicit Texture(const std::filesystem::path& path, bool flip = true)
        : id_(load_texture(path, flip)), filename_(path) {}

    GLuint id() const { return id_; }
    std::filesystem::path filename() const { return filename_; }

    explicit operator bool() const { return id_ != 0; }
    bool empty() const { return id_ == 0; }

    void bind() const;
    void bind(unsigned int unit) const;

    void set_param(GLenum pname, GLint param);
    void set_param(GLenum pname, GLfloat param);
    void set_param(GLenum pname, GLint* param);
    void set_param(GLenum pname, GLfloat* param);

    friend std::ostream& operator<<(std::ostream& os, const Texture& texture);

  private:
    TextureHandle id_;
    std::filesystem::path filename_;
};

#endif // TEXTURE_H
