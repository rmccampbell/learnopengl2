#include "shader.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include <glad/glad.h>

#include "errutils.h"

static void check_compile_errors(GLuint shader, std::string_view type) {
    int success, len;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string log(len ? len - 1 : 0, '\0');
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        err::error("{} shader compilation error:\n{}", type, log);
    }
}

static void check_link_errors(GLuint program) {
    int success, len;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::string log(len ? len - 1 : 0, '\0');
        glGetProgramInfoLog(program, len, nullptr, log.data());
        err::error("shader program linking error:\n{}", log);
    }
}

GLuint build_shader(const char* vs_source, const char* fs_source, const char* gs_source) {
    ShaderHandle vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vs_source, nullptr);
    glCompileShader(vshader);
    check_compile_errors(vshader, "vertex");
    ShaderHandle fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fs_source, nullptr);
    glCompileShader(fshader);
    check_compile_errors(fshader, "fragment");
    ShaderHandle gshader = 0;
    if (gs_source) {
        gshader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gshader, 1, &gs_source, nullptr);
        glCompileShader(gshader);
        check_compile_errors(gshader, "geometry");
    }
    ProgramHandle prog = glCreateProgram();
    glAttachShader(prog, vshader);
    glAttachShader(prog, fshader);
    if (gshader)
        glAttachShader(prog, gshader);
    glLinkProgram(prog);
    check_link_errors(prog);
    return prog.release();
}

static std::string read_file(const std::filesystem::path& filename) {
    std::ifstream file(filename);
    err::check_errno(file, "failed to open file: {}: {}", filename.string());
    std::stringstream sstream;
    sstream << file.rdbuf();
    return sstream.str();
}

GLuint load_shader(const std::filesystem::path& vs_path,
                   const std::filesystem::path& fs_path,
                   const std::filesystem::path& gs_path) {
    return build_shader(read_file(vs_path).c_str(), read_file(fs_path).c_str(),
                        gs_path.empty() ? nullptr : read_file(gs_path).c_str());
}
