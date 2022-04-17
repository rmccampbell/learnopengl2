#ifndef U8TILS_H
#define U8TILS_H

#include <filesystem>
#include <string>
#include <utility>

namespace u8 {

#ifdef __cpp_char8_t
using char8_type = char8_t;
#else
using char8_type = char;
#endif // __cpp_char8_t

using u8string_type = std::basic_string<char8_type>;
using u8string_view_type = std::basic_string_view<char8_type>;

inline u8string_type to_u8string(std::string_view s) {
    return u8string_type(s.begin(), s.end());
}

inline std::string to_string(u8string_view_type s) {
    return std::string(s.begin(), s.end());
}

inline std::string_view to_string_view(u8string_view_type s) {
    return std::string_view(reinterpret_cast<const char*>(s.data()), s.size());
}

inline const char* to_char(const char8_type* s) {
    return reinterpret_cast<const char*>(s);
}

inline char* to_char(char8_type* s) {
	return reinterpret_cast<char*>(s);
}

inline const char* to_char(const u8string_type& s) {
    return reinterpret_cast<const char*>(s.c_str());
}

#ifdef __cpp_char8_t
inline std::filesystem::path to_path(std::string_view s) {
    return to_u8string(s);
}

inline std::filesystem::path to_path(std::u8string_view s) {
    return s;
}
#else
inline std::filesystem::path to_path(std::string_view s) {
    return std::filesystem::u8path(s);
}
#endif // __cpp_char8_t

inline std::string path_to_string(const std::filesystem::path& p) {
    return to_string(p.u8string());
}

// This is needed to allow c string conversion without dangling pointers.
// If we returned to_char(p.u8string()) below the pointer would reference a
// function-local temporary. The wrapper class ensures the temporary lives
// for the full outer expression.
class _to_char {
  public:
    _to_char(u8string_type s) : s8(std::move(s)) {}
    operator const char*() { return to_char(s8); }

  private:
    u8string_type s8;
};

inline _to_char path_to_char(const std::filesystem::path& p) {
    return _to_char(p.u8string());
}

} // namespace u8

#endif // U8TILS_H
