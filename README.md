# constexpr-to-string

**Features:**

* Convert any integral type to a string at compile-time
* String can be in binary, hexadecimal, or any base in-between
* No external dependencies, only includes `type_traits` for type-checking
* Works best in C++20 GCC or C++17/20 Clang

**How to use:**

This single header file provides a `to_string` utility, which may be used as below:

```cpp
const char *number = to_string<2147483648999954564, 16>; // produces "1DCD65003B9A1884"
puts(number);
puts(to_string<-42>); // produces "-42"
puts(to_string<30, 2>); // produces "11110"
```

With `to_string`, all that will be found in program disassembly are the resulting string literals, as if you wrote the strings yourself.

**Known issues:**

* With C++17 GCC, `to_string` must be used to initialize variables; otherwise, the integer-string conversion is done at run-time.