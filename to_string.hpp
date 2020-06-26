/**
 * to_string.hpp - Provides compile-time integer-to-string conversion.
 * Written by Clyne Sullivan.
 * https://github.com/tcsullivan/constexpr-to-string
 */

#ifndef TCSULLIVAN_TO_STRING_HPP_
#define TCSULLIVAN_TO_STRING_HPP_

#include <type_traits>

/**
 * @struct to_string_t
 * @brief Provides the ability to convert any integral to a string at compile-time.
 * @tparam N Number to convert
 * @tparam base Desired base, can be from 2 to 36
 */
template<auto N, unsigned int base,
    std::enable_if_t<std::is_integral_v<decltype(N)>, int> = 0,
    std::enable_if_t<(base > 1 && base < 37), int> = 0>
struct to_string_t {
    // The lambda calculates what the string length of N will be, so that `buf`
    // fits to the number perfectly.
    char buf[([] {
                  unsigned int len = N >= 0 ? 1 : 2;
                  for (auto n = N; n; len++, n /= base);
                  return len;
             }())];

    /**
     * Constructs the object, filling `buf` with the string representation of N.
     */
    constexpr to_string_t() {
        auto ptr = buf + sizeof(buf) / sizeof(buf[0]);
        *--ptr = '\0';
        for (auto n = N; n; n /= base)
            *--ptr = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[(N < 0 ? -1 : 1) * (n % base)];
        if (N < 0)
            *--ptr = '-';
    }

    /**
     * Allows implicit conversion of this object to a `char *`.
     */
    constexpr operator char *() {
        return buf;
    }
    
    /**
     * Allows implicit conversion of this object to a `const char *`.
     */
    constexpr operator const char *() const {
        return buf;
    }
};

/**
 * Simplifies use of `to_string_t` from `to_string_t<N>()` to `to_string<N>`.
 */
template<auto N, unsigned int base = 10>
to_string_t<N, base> to_string;
 
#endif // TCSULLIVAN_TO_STRING_HPP_
