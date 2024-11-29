#ifndef ERRUTILS_H
#define ERRUTILS_H

#include <cerrno>
#include <cstring>
#include <format>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <unordered_map>
#include <utility>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "utils.h"

namespace err {

template <typename... Args>
[[noreturn]] void error(std::format_string<Args...> fmt, Args&&... args) {
    throw std::runtime_error(std::format(fmt, std::forward<Args>(args)...));
}

template <typename T, typename... Args>
T&& check(T&& val, std::format_string<Args...> fmt, Args&&... args) {
    if (!val)
        error(fmt, std::forward<Args>(args)...);
    return std::forward<T>(val);
}

template <typename T, typename... Args>
T&& check_errno(T&& val, std::format_string<Args..., std::string_view> fmt, Args&&... args) {
    if (!val) {
        std::string msg = std::generic_category().message(errno);
        error(fmt, std::forward<Args>(args)..., std::string_view(msg));
    }
    return std::forward<T>(val);
}

template <typename T, typename... Args>
T&& check_posix(T&& val, std::format_string<Args..., std::string_view> fmt, Args&&... args) {
    check_errno(val >= 0, fmt, std::forward<Args>(args)...);
    return std::forward<T>(val);
}

template <typename T, typename... Args>
T&& check_glfw(T&& val, std::format_string<Args..., std::string_view> fmt, Args&&... args) {
    if (!val) {
        const char* msg;
        glfwGetError(&msg);
        error(fmt, std::forward<Args>(args)...,
              std::string_view(msg ? msg : "(no GLFW error)"));
    }
    return std::forward<T>(val);
}

template <typename... Args>
void check_glfw_error(std::format_string<Args..., std::string_view> fmt, Args&&... args) {
    const char* msg;
    if (glfwGetError(&msg)) {
        error(fmt, std::forward<Args>(args)..., std::string_view(msg));
    }
}

template <typename... Args>
void check_gl_error(std::format_string<Args..., std::string_view> fmt, Args&&... args) {
    if (GLenum err = glGetError()) {
        static const std::unordered_map<GLenum, std::string_view> enum_map{
            {GL_INVALID_ENUM, "INVALID_ENUM"},
            {GL_INVALID_VALUE, "INVALID_VALUE"},
            {GL_INVALID_OPERATION, "INVALID_OPERATION"},
            {GL_STACK_OVERFLOW, "STACK_OVERFLOW"},
            {GL_STACK_UNDERFLOW, "STACK_UNDERFLOW"},
            {GL_OUT_OF_MEMORY, "OUT_OF_MEMORY"},
            {GL_INVALID_FRAMEBUFFER_OPERATION, "INVALID_FRAMEBUFFER_OPERATION"},
        };
        std::string_view errstr = util::get_or(enum_map, err, "UNKNOWN");
        error(fmt, std::forward<Args>(args)..., errstr);
    }
}

} // namespace err

#endif // ERRUTILS_H
