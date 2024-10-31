/**
 * to_string.hpp - Provides compile-time integer-to-string conversion.
 * Written by Clyne Sullivan.
 * https://github.com/tcsullivan/constexpr-to-string
 */

#ifndef TCSULLIVAN_TO_STRING_HPP_
#define TCSULLIVAN_TO_STRING_HPP_

#include <cstdint>
#include <type_traits>

namespace constexpr_to_string {

constexpr char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/**
 * @struct to_string_t
 * @brief Provides the ability to convert any integral to a string at compile-time.
 * @tparam N Number to convert
 * @tparam base Desired base, can be from 2 to 36
 */
template<std::intmax_t N, int base, typename char_type,
    std::enable_if_t<(base > 1 && base < sizeof(digits)), int> = 0>
class to_string_t {

    constexpr static auto buflen() noexcept {
        unsigned int len = N > 0 ? 1 : 2;
        for (auto n = N; n; len++, n /= base);
        return len;
    }

    char_type buf[buflen()] = {};

 public:
    /**
     * Constructs the object, filling `buf` with the string representation of N.
     */
    constexpr to_string_t() noexcept {
        auto ptr = end();
        *--ptr = '\0';

        if (N != 0) {
            for (auto n = N; n; n /= base)
                *--ptr = digits[(N < 0 ? -1 : 1) * (n % base)];
            if (N < 0)
                *--ptr = '-';
        } else {
            buf[0] = '0';
        }
    }

    // Support implicit casting to `char *` or `const char *`.
    constexpr operator char_type *() noexcept { return buf; }
    constexpr operator const char_type *() const noexcept { return buf; }

    constexpr auto size() const noexcept { return sizeof(buf) / sizeof(buf[0]); }

    // Element access
    constexpr auto data() noexcept { return buf; }
    constexpr auto data() const noexcept { return buf; }
    constexpr auto& operator[](unsigned int i) noexcept { return buf[i]; }
    constexpr const auto& operator[](unsigned int i) const noexcept { return buf[i]; }
    constexpr auto& front() noexcept { return buf[0]; }
    constexpr const auto& front() const noexcept { return buf[0]; }
    constexpr auto& back() noexcept { return buf[size() - 1]; }
    constexpr const auto& back() const noexcept { return buf[size() - 1]; }

    // Iterators
    constexpr auto begin() noexcept { return buf; }
    constexpr auto begin() const noexcept { return buf; }
    constexpr auto end() noexcept { return buf + size(); }
    constexpr auto end() const noexcept { return buf + size(); }
};

} // namespace constexpr_to_string

/**
 * Simplifies use of `to_string_t` from `to_string_t<N>()` to `to_string<N>`.
 */
template<std::intmax_t N, int base = 10, typename char_type = char>
constexpr constexpr_to_string::to_string_t<N, base, char_type> to_string;

#endif // TCSULLIVAN_TO_STRING_HPP_

