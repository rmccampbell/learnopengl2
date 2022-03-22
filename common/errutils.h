#ifndef ERRUTILS_H
#define ERRUTILS_H

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <utility>

#include <GLFW/glfw3.h>
#include <fmt/format.h>

namespace err {

template <typename... Args>
[[noreturn]] void error(fmt::format_string<Args...> fmt, Args&&... args) {
    throw std::runtime_error(fmt::format(fmt, std::forward<Args>(args)...));
}

template <typename T, typename... Args>
T&& check(T&& val, fmt::format_string<Args...> fmt, Args&&... args) {
    if (!val)
        error(fmt, std::forward<Args>(args)...);
    return std::forward<T>(val);
}

template <typename T, typename... Args>
T&& check_errno(T&& val, fmt::format_string<Args...> fmt, Args&&... args) {
    if (!val) {
#pragma warning(suppress : 4996) // strerror is *technically* not thread-safe
        error(fmt, std::forward<Args>(args)..., strerror(errno));
    }
    return std::forward<T>(val);
}

template <typename T, typename... Args>
T&& check_glfw(T&& val, fmt::format_string<Args...> fmt, Args&&... args) {
    if (!val) {
        const char* msg;
        glfwGetError(&msg);
        error(fmt, std::forward<Args>(args)..., msg ? msg : "(no GLFW error)");
    }
    return std::forward<T>(val);
}

} // namespace err

#endif // ERRUTILS_H
