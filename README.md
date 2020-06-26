# constexpr-to-string

**Features:**

* Convert any integral type to a string at compile-time
* Supports converting to any base between 2 and 36 inclusive
* No external dependencies, only includes `type_traits` for template parameter checking

**How to use:**

This single header file provides a `to_string` utility, which may be used as below:

```cpp
const char *number = to_string<2147483648999954564, 16>; // produces "1DCD65003B9A1884"
puts(number);
puts(to_string<-42>); // produces "-42"
puts(to_string<30, 2>); // produces "11110"
```

With `to_string`, all that will be found in program disassembly are the resulting string literals, as if you wrote the strings yourself.

Try it [on Compiler Explorer](https://godbolt.org/z/T-MFoh).

# How it works

The basic structure of `to_string` is shown below:

```cpp
template<auto N, unsigned int base, /* N type-check and base bounds-check */>
struct to_string_t {
    char buf[];                          // Size selection explained later.
    constexpr to_string_t() {}           // Converts the integer to a string stored in buf.
    constexpr operator char *() {}       // These allow for the object to be implicitly converted
    constexpr operator const char *() {} // to a character pointer.
};

template<auto N, unsigned int base = 10>
to_string_t<N, base> to_string;          // Simplifies usage: to_string_t<N, base>() becomes to_string<N, base>.
```

Since the number and base are template parameters, each differing `to_string` use will get its own character buffer.

The integer/string conversion is done using a simple method I learned over the years, where the string is built in reverse using `n % base` to calculate the value of the lowest digit:

```cpp
constexpr to_string_t() {
    auto ptr = buf + sizeof(buf) / sizeof(buf[0]);
    *--ptr = '\0';
    for (auto n = N < 0 ? -N : N; n; n /= base)
        *--ptr = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[n % base];
    if (N < 0)
        *--ptr = '-';
}
```

As you may have noticed, `buf` needs to be given a size for all this to work; in fact, the above code relies on the buffer having a size equal to the generated string (or else `buf[0]` would still be uninitialized). This is actually the case: a lambda is used within `buf`'s declaration to count how many characters long the string will ultimately be. This counting is done in a manner similar to conversion loop shown above:

```cpp
char buf[([] {
              unsigned int len = N >= 0 ? 1 : 2; // Need one byte for '\0', two if there'll be a minus
              for (auto n = N < 0 ? -N : N; n; len++, n /= base);
              return len;
          }())];
```
