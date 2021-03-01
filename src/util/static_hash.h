
#ifndef FSM_SRC_UTIL_STATIC_HASH_H
#define FSM_SRC_UTIL_STATIC_HASH_H

#include <cstdint>
#include <string_view>

namespace fsm::util {

using hash_t = uint32_t;

namespace internal {
/**
 * @brief FNV-1a 32bit hashing algorithm.
 * @param p_s 
 * @param p_count 
 * @return constexpr uint32_t 
 */
constexpr hash_t do_hash(char const *p_s, std::size_t p_len) 
{
    return ((p_len ? do_hash(p_s, p_len - 1) : 2166136261u) ^ p_s[p_len]) * 16777619u;
}

} // namespace internal

template <std::size_t N>
constexpr hash_t hash(char const (&p_s)[N]) 
{
    if constexpr (N-1u == 0) return 0;  // empty string
    return internal::do_hash(p_s, N-1u);
}

inline hash_t hash(std::string_view p_s) 
{
    return internal::do_hash(p_s.data(), p_s.size());
}


}  // namespace fsm::util

#endif  // FSM_SRC_UTIL_STATIC_HASH_H