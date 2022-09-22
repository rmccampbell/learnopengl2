#include "shader.h"

#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

#include <glad/glad.h>

#include "cstring_view.h"
#include "errutils.h"

namespace {

std::string read_file(const std::filesystem::path& filename) {
    std::ifstream file(filename);
    err::check_errno(file, "failed to open file: {}: {}", filename.string());
    std::stringstream sstream;
    sstream << file.rdbuf();
    return sstream.str();
}

void check_compile_errors(GLuint shader, std::string_view type) {
    int success, len;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string log(len ? len - 1 : 0, '\0');
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        err::error("{} shader compilation error:\n{}", type, log);
    }
}

void check_link_errors(GLuint program) {
    int success, len;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::string log(len ? len - 1 : 0, '\0');
        glGetProgramInfoLog(program, len, nullptr, log.data());
        err::error("shader program linking error:\n{}", log);
    }
}

} // namespace

GLuint build_shader(cstring_view vs_source, cstring_view fs_source,
                    std::optional<cstring_view> gs_source) {
    ShaderHandle vshader{glCreateShader(GL_VERTEX_SHADER)};
    const char* vs_source_p = vs_source.c_str();
    glShaderSource(*vshader, 1, &vs_source_p, nullptr);
    glCompileShader(*vshader);
    check_compile_errors(*vshader, "vertex");
    ShaderHandle fshader{glCreateShader(GL_FRAGMENT_SHADER)};
    const char* fs_source_p = fs_source.c_str();
    glShaderSource(*fshader, 1, &fs_source_p, nullptr);
    glCompileShader(*fshader);
    check_compile_errors(*fshader, "fragment");
    ShaderHandle gshader;
    if (gs_source) {
        gshader.reset(glCreateShader(GL_GEOMETRY_SHADER));
        const char* gs_source_p = gs_source->c_str();
        glShaderSource(*gshader, 1, &gs_source_p, nullptr);
        glCompileShader(*gshader);
        check_compile_errors(*gshader, "geometry");
    }
    ProgramHandle prog{glCreateProgram()};
    glAttachShader(*prog, *vshader);
    glAttachShader(*prog, *fshader);
    if (gshader)
        glAttachShader(*prog, *gshader);
    glLinkProgram(*prog);
    check_link_errors(*prog);
    return prog.release();
}

GLuint load_shader(const std::filesystem::path& vs_path,
                   const std::filesystem::path& fs_path,
                   const std::filesystem::path& gs_path) {
    return build_shader(read_file(vs_path), read_file(fs_path),
                        !gs_path.empty() ? read_file(gs_path)
                                         : std::optional<cstring_view>{});
}
