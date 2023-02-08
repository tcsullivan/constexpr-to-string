# constexpr-to-string

Requires C++14 or later.

**Features:**

* Convert any integral type to a string at compile-time
* Supports converting to bases 2 through 36
* No external dependencies
* Supports custom character types, e.g. `to_string<123, 10, wchar_t>`
* C++20: Supports floating-point-to-string conversion with `f_to_string`

**How to use:**

The file `to_string.hpp` provides a `to_string` utility, which may be used as below:

```cpp
const char *number = to_string<2147483648999954564, 16>; // produces "1DCD65003B9A1884"
puts(number);
puts(to_string<-42>); // produces "-42"
puts(to_string<30, 2>); // produces "11110"
```

With `to_string`, all that will be found in program disassembly are the resulting string literals, as if you wrote the strings yourself.

Try it [on Compiler Explorer](https://godbolt.org/z/T-MFoh).

`f_to_string.hpp`, requiring C++20, provides an `f_to_string` utility for floating-point conversion:

```cpp
puts(f_to_string<3.1415926>); // Defaults to 5-point precision: "3.14159"
puts(f_to_string<{3.1415926, 7}>); // Specify precision: "3.1415926"
```

# How it works

C++14 greatly expanded the capabilities of compile-time code execution through `constexpr`. In particular, it allows for non-trivial constructors to be `constexpr`.

`to_string` takes advantage of this by providing an object that converts a template-parameter integer to a string using a basic `itoa` implementation in the constructor. Through an additional `constexpr` member function, we can calculate the length of the resulting string; this can be used to size the object's string buffer for a perfect fit.

Beyond this, `to_string` simply provides familiar member functions that allow for iteration and data access. The expansion of the capabilities of `auto` in C++14 help make these definitions concise.

The floating-point implementation `f_to_string` takes a similar approach, but requires C++20 as it needs a `double_wrapper` object to capture the `double` value. `double` and `float` cannot directly be template parameters as of C++20, and a non-type template parameter like the `double_wrapper` structure was not allowed before C++20.

