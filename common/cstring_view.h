#ifndef CSTRING_VIEW_H
#define CSTRING_VIEW_H

#include <string>
#include <string_view>

template <typename Char>
class basic_cstring_view {
  public:
    basic_cstring_view(const Char* s) : data_(s) {}

    basic_cstring_view(const std::basic_string<Char>& s) : data_(s.c_str()) {}

    operator std::basic_string_view<Char>() const { return data_; }

    const Char* c_str() const { return data_; }

  private:
    const Char* data_;
};

using cstring_view = basic_cstring_view<char>;
using wcstring_view = basic_cstring_view<wchar_t>;

#endif // CSTRING_VIEW_H
