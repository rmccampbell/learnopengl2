#ifndef UTILS_H
#define UTILS_H

#include <optional>

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
auto get_or(const M& map, const K& key, V def) {
    auto it = map.find(key);
    return it != map.end() ? it->second : std::move(def);
}

template <typename M, typename K>
auto get_or_null(M& map, const K& key) {
    auto it = map.find(key);
    return it != map.end() ? &it->second : nullptr;
}

} // namespace util

#endif // UTILS_H
