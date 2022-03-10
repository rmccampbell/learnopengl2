#ifndef ERRUTIL_H
#define ERRUTIL_H

#include <stdexcept>
#include <utility>

#include <fmt/format.h>

namespace err {

template <typename... Args>
void error(fmt::format_string<Args...> s, Args&&... args) {
    throw std::runtime_error(fmt::format(s, std::forward<Args>(args)...));
}

template <typename T, typename... Args>
decltype(auto) check(T&& val, fmt::format_string<Args...> s, Args&&... args) {
    if (!val)
        error(s, std::forward<Args>(args)...);
    return std::forward<T>(val);
}

} // namespace err

#endif // ERRUTIL_H
