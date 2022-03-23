#ifndef SHADER_H
#define SHADER_H

#include <filesystem>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "handle.h"

struct shader_deleter {
    void operator()(GLuint s) { glDeleteShader(s); }
};

struct program_deleter {
    void operator()(GLuint p) { glDeleteProgram(p); }
};

using ShaderHandle = Handle<GLuint, shader_deleter>;
using ProgramHandle = Handle<GLuint, program_deleter>;

GLuint build_shader(const char* vs_source, const char* fs_source,
                    const char* gs_source = nullptr);

GLuint load_shader(const std::filesystem::path& vs_path,
                   const std::filesystem::path& fs_path,
                   const std::filesystem::path& gs_path = {});

class Shader {
  public:
    explicit Shader(GLuint id) : id_(id) {}
    Shader(const char* vs_source, const char* fs_source, const char* gs_source = nullptr)
        : id_(build_shader(vs_source, fs_source, gs_source)) {}

    static Shader load(const std::filesystem::path& vs_path,
                       const std::filesystem::path& fs_path,
                       const std::filesystem::path& gs_path = {}) {
        return Shader(load_shader(vs_path, fs_path, gs_path));
    }

    GLuint id() const { return id_; }

    void use() const { glUseProgram(id_); }

    void bind_attribute_location(const char* name, GLuint index) const {
        glBindAttribLocation(id_, index, name);
    }

    GLint uniform_location(const char* name) const {
        return glGetUniformLocation(id_, name);
    }

    void set_bool(const char* name, bool value) const {
        glUniform1i(uniform_location(name), value);
    }
    void set_int(const char* name, int value) const {
        glUniform1i(uniform_location(name), value);
    }
    void set_uint(const char* name, unsigned int value) const {
        glUniform1ui(uniform_location(name), value);
    }
    void set_float(const char* name, float value) const {
        glUniform1f(uniform_location(name), value);
    }
    void set_vec2(const char* name, const glm::vec2& value) const {
        glUniform2fv(uniform_location(name), 1, glm::value_ptr(value));
    }
    void set_vec2(const char* name, float x, float y) const {
        glUniform2f(uniform_location(name), x, y);
    }
    void set_vec3(const char* name, const glm::vec3& value) const {
        glUniform3fv(uniform_location(name), 1, glm::value_ptr(value));
    }
    void set_vec3(const char* name, float x, float y, float z) const {
        glUniform3f(uniform_location(name), x, y, z);
    }
    void set_vec4(const char* name, const glm::vec4& value) const {
        glUniform4fv(uniform_location(name), 1, glm::value_ptr(value));
    }
    void set_vec4(const char* name, float x, float y, float z, float w) const {
        glUniform4f(uniform_location(name), x, y, z, w);
    }
    void set_mat2(const char* name, const glm::mat2& mat) const {
        glUniformMatrix2fv(uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void set_mat3(const char* name, const glm::mat3& mat) const {
        glUniformMatrix3fv(uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void set_mat4(const char* name, const glm::mat4& mat) const {
        glUniformMatrix4fv(uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

  private:
    ProgramHandle id_;
};

#endif // SHADER_H
