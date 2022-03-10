#ifndef SHADER_H
#define SHADER_H

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>

#include "errutils.h"

inline void check_compile_errors(GLuint shader, const std::string& type) {
    int success, len;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string log(len ? len-1 : 0, '\0');
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        err::error("{} shader compilation error:\n{}", type, log);
    }
}

inline void check_link_errors(GLuint program) {
    int success, len;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::string log(len ? len-1 : 0, '\0');
        glGetProgramInfoLog(program, len, nullptr, log.data());
        err::error("shader program linking error:\n{}", log);
    }
}

inline GLuint build_shader(const char* vs_source, const char* fs_source) {
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vs_source, nullptr);
    glCompileShader(vshader);
    check_compile_errors(vshader, "vertex");
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fs_source, nullptr);
    glCompileShader(fshader);
    check_compile_errors(fshader, "fragment");
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vshader);
    glAttachShader(prog, fshader);
    glLinkProgram(prog);
    check_link_errors(prog);
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    return prog;
}

inline std::string read_string(std::filesystem::path filename) {
    std::ifstream file(filename);
    err::check(file, "failed to open file: {}", filename.string());
    std::stringstream sstream;
    sstream << file.rdbuf();
    return sstream.str();
}

inline GLuint load_shader(std::filesystem::path vs_path,
                          std::filesystem::path fs_path) {
    return build_shader(read_string(vs_path).c_str(),
                        read_string(fs_path).c_str());
}

#endif // SHADER_H
