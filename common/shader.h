#ifndef SHADER_H
#define SHADER_H

#include <filesystem>
#include <format>
#include <optional>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cstring_view.h"
#include "raii.h"
#include "utils.h"

using ShaderHandle = Handle<GLuint, functor<glDeleteShader>>;
using ProgramHandle = Handle<GLuint, functor<glDeleteProgram>>;

GLuint build_shader(cstring_view vs_source, cstring_view fs_source,
                    std::optional<cstring_view> gs_source = {});

GLuint load_shader(const std::filesystem::path& vs_path,
                   const std::filesystem::path& fs_path,
                   const std::filesystem::path& gs_path = {});

class Shader {
  public:
    explicit Shader(GLuint id) : id_(id) { fetch_uniform_locations(); }
    Shader(cstring_view vs_source, cstring_view fs_source,
           std::optional<cstring_view> gs_source = {})
        : Shader(build_shader(vs_source, fs_source, gs_source)) {}

    static Shader load(const std::filesystem::path& vs_path,
                       const std::filesystem::path& fs_path,
                       const std::filesystem::path& gs_path = {}) {
        return Shader(load_shader(vs_path, fs_path, gs_path));
    }

    GLuint id() const { return id_.get(); }

    void use() const { glUseProgram(id()); }

    GLint uniform_location(cstring_view name) const {
        return glGetUniformLocation(id(), name.c_str());
        // return util::get_or(uniform_locations_, std::string(name), -1);
    }

    void set_bool(cstring_view name, bool value) const {
        glUniform1i(uniform_location(name), value);
    }
    void set_int(cstring_view name, int value) const {
        glUniform1i(uniform_location(name), value);
    }
    void set_uint(cstring_view name, unsigned int value) const {
        glUniform1ui(uniform_location(name), value);
    }
    void set_float(cstring_view name, float value) const {
        glUniform1f(uniform_location(name), value);
    }
    void set_vec2(cstring_view name, const glm::vec2& value) const {
        glUniform2fv(uniform_location(name), 1, glm::value_ptr(value));
    }
    void set_vec2(cstring_view name, float x, float y) const {
        glUniform2f(uniform_location(name), x, y);
    }
    void set_vec3(cstring_view name, const glm::vec3& value) const {
        glUniform3fv(uniform_location(name), 1, glm::value_ptr(value));
    }
    void set_vec3(cstring_view name, float x, float y, float z) const {
        glUniform3f(uniform_location(name), x, y, z);
    }
    void set_vec4(cstring_view name, const glm::vec4& value) const {
        glUniform4fv(uniform_location(name), 1, glm::value_ptr(value));
    }
    void set_vec4(cstring_view name, float x, float y, float z, float w) const {
        glUniform4f(uniform_location(name), x, y, z, w);
    }
    void set_mat2(cstring_view name, const glm::mat2& mat) const {
        glUniformMatrix2fv(uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void set_mat3(cstring_view name, const glm::mat3& mat) const {
        glUniformMatrix3fv(uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void set_mat4(cstring_view name, const glm::mat4& mat) const {
        glUniformMatrix4fv(uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

  private:
    void fetch_uniform_locations();

    ProgramHandle id_;
    std::unordered_map<std::string, GLint> uniform_locations_;
};

// Apply multiple apply-able objects to a Shader array and count variable
template <typename Container>
void apply_array(const Shader& shader, std::string_view array_name,
                 cstring_view count_name, const Container& objs) {
    shader.set_int(count_name, int(std::size(objs)));
    apply_array(shader, array_name, objs);
}

// Apply multiple apply-able objects to a Shader array
template <typename Container>
void apply_array(const Shader& shader, std::string_view array_name,
                 const Container& objs) {
    for (size_t i = 0; i < std::size(objs); i++) {
        objs[i].apply(shader, std::format("{}[{}]", array_name, i));
    }
}

#endif // SHADER_H
