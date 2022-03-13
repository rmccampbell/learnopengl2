#ifndef ERRUTIL_H
#define ERRUTIL_H

#include <stdexcept>
#include <utility>

#include <GLFW/glfw3.h>
#include <fmt/format.h>

namespace err {

template <typename... Args>
[[ noreturn ]] void error(fmt::format_string<Args...> fmt, Args&&... args) {
    throw std::runtime_error(fmt::format(fmt, std::forward<Args>(args)...));
}

template <typename T, typename... Args>
decltype(auto) check(T&& val, fmt::format_string<Args...> fmt, Args&&... args) {
    if (!val)
        error(fmt, std::forward<Args>(args)...);
    return std::forward<T>(val);
}

template <typename T, typename... Args>
decltype(auto) check_glfw(T&& val, fmt::format_string<Args...> fmt,
                          Args&&... args) {
    if (!val) {
        const char* msg;
        glfwGetError(&msg);
        error(fmt, std::forward<Args>(args)..., msg ? msg : "(no GLFW error)");
    }
    return std::forward<T>(val);
}

} // namespace err

#endif // ERRUTIL_H
