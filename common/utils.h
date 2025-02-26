#ifndef UTILS_H
#define UTILS_H

#include <array>
#include <optional>
#include <tuple>
#include <utility>

namespace util {

template <typename M, typename K>
auto get_optional(const M& map, const K& key) {
    auto it = map.find(key);
    return it != map.end() ? std::make_optional(it->second) : std::nullopt;
}

template <typename M, typename K>
auto get_or_default(const M& map, const K& key) {
    auto it = map.find(key);
    return it != map.end() ? it->second : typename M::mapped_type{};
}

template <typename M, typename K, typename V>
auto get_or(const M& map, const K& key, V&& def) {
    auto it = map.find(key);
    return it != map.end() ? it->second : std::forward<V>(def);
}

template <typename M, typename K>
auto* get_or_null(M& map, const K& key) {
    auto it = map.find(key);
    return it != map.end() ? &it->second : nullptr;
}

template <typename... Arrays>
constexpr auto array_cat(Arrays&&... arrays) {
    return std::apply(
        []<typename... T>(T&&... elems) {
            return std::array{std::forward<T>(elems)...};
        },
        std::tuple_cat(std::forward<Arrays>(arrays)...));
}

} // namespace util

#endif // UTILS_H
