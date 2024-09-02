/**
 * f_to_string.hpp - Provides compile-time floating-point-to-string conversion.
 * Written by Clyne Sullivan.
 * https://github.com/tcsullivan/constexpr-to-string
 */

#ifndef TCSULLIVAN_F_TO_STRING_HPP_
#define TCSULLIVAN_F_TO_STRING_HPP_

namespace constexpr_to_string {

struct double_wrapper {
    long long int whole = 0;
    long long int frac = 0;

    constexpr double_wrapper(double v, int prec = 5) {
        whole = static_cast<long long int>(v);
        v -= whole;
        for (int i = 0; i < prec; i++)
            v *= 10;
        frac = static_cast<long long int>(v);
    }
};

/**
 * @struct f_to_string_t
 * @brief Provides the ability to convert a floating-point number to a string at compile-time.
 * @tparam N Number to convert
 */
template<double_wrapper N, typename char_type>
class f_to_string_t {
    char_type buf[([]() constexpr noexcept {
                       unsigned int len = 2;
                       if (N.whole <= 0) len++;
                       for (auto n = N.whole; n; len++, n /= 10);
                       if (N.frac == 0 || (N.whole == 0 && N.frac < 0)) len++;
                       for (auto n = N.frac; n; len++, n /= 10);
                       return len;
                   }())] = {};

 public:
    /**
     * Constructs the object, filling `buf` with the string representation of N.
     */
    constexpr f_to_string_t() noexcept {
        auto append = [](auto V, auto& ptr) {
            if (V != 0) {
                for (auto n = V; n; n /= 10)
                    *--ptr = (V < 0 ? -1 : 1) * (n % 10) + '0';
            } else {
                *--ptr = '0';
            }
        };

        auto ptr = end();
        *--ptr = '\0';
        append(N.frac, ptr);
        *--ptr = '.';
        append(N.whole, ptr);
        if (N.frac < 0 || N.whole < 0)
            *--ptr = '-';
    }

    // Support implicit casting to `char *` or `const char *`.
    constexpr operator char_type *() noexcept { return buf; }
    constexpr operator const char_type *() const noexcept { return buf; }

    constexpr auto size() const noexcept { return sizeof(buf) / sizeof(buf[0]); }
    // Element access
    constexpr auto data() noexcept { return buf; }
    constexpr const auto data() const noexcept { return buf; }
    constexpr auto& operator[](unsigned int i) noexcept { return buf[i]; }
    constexpr const auto& operator[](unsigned int i) const noexcept { return buf[i]; }
    constexpr auto& front() noexcept { return buf[0]; }
    constexpr const auto& front() const noexcept { return buf[0]; }
    constexpr auto& back() noexcept { return buf[size() - 1]; }
    constexpr const auto& back() const noexcept { return buf[size() - 1]; }
    // Iterators
    constexpr auto begin() noexcept { return buf; }
    constexpr const auto begin() const noexcept { return buf; }
    constexpr auto end() noexcept { return buf + size(); }
    constexpr const auto end() const noexcept { return buf + size(); }
};

} // namespace constexpr_to_string

/**
 * Simplifies use of `f_to_string_t` from `f_to_string_t<N>()` to `f_to_string<N>`.
 */
template<constexpr_to_string::double_wrapper N, typename char_type = char>
constexpr constexpr_to_string::f_to_string_t<N, char_type> f_to_string;

#endif // TCSULLIVAN_F_TO_STRING_HPP_
