#ifndef UTIL_H
#define UTIL_H

#include <array>
#include <glad/glad.h>

namespace util {

namespace detail {
void gl_get_impl(GLenum pname, GLint* data) { glGetIntegerv(pname, data); }

void gl_get_impl(GLenum pname, GLint64* data) { glGetInteger64v(pname, data); }

void gl_get_impl(GLenum pname, GLfloat* data) { glGetFloatv(pname, data); }

void gl_get_impl(GLenum pname, GLdouble* data) { glGetDoublev(pname, data); }

void gl_get_impl(GLenum pname, GLboolean* data) { glGetBooleanv(pname, data); }
} // namespace detail

template <typename T, size_t N = 0>
auto gl_get(GLenum pname) {
    if constexpr (N == 0) {
        T data;
        detail::gl_get_impl(pname, &data);
        return data;
    } else {
        std::array<T, N> data;
        detail::gl_get_impl(pname, data.data());
        return data;
    }
}

} // namespace util

#endif // UTIL_H
