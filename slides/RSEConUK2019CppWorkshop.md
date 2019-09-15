% Getting the most out of the modern C++ language and standard libraries
% **Oxford RSE @ RSEConUK 2019**
% Fergus Cooper ~ Graham Lee ~ Thibault Lestang ~ Martin Robinson

## C++ has changed a lot

C++ was first standardised as *ISO/IEC 14882:1998* (C++98), and since then:

- C++11 (huge update)
- C++14 (bug fixes, plus a bit)
- C++17 (fairly hefty)
- C++20 (huge update)

## A (very quick) overview of C++11

- Feature

## A (very quick) overview of C++14

- Feature

## A (very quick) overview of C++17

- Feature

## A (very quick) overview of C++20

- Feature

## Is this all a bit overwhelming?

How are you supposed to know which features to use?

- C++ core guidelines

## Part 1 — The filesystem library

## Index-based `for` loop

Let's say we have a vector.
The first way we were probably all taught to loop over the contents of a vector was with an index-based `for`{.cpp} loop:

~~~cpp
std::vector<double> v = {1.0, 2.0, 3.0, 4.0};

for (int i = 0; i < v.size(); ++i) {
   std::cout << v[i] << std::endl;
}
~~~

(Can you spot a subtle issue here?) 

## Iterator-based `for` loop

`std::vector` is a container in the Standard Template Library.
Every container defines its own **iterators**, so we can also loop over a vector in the following way:

~~~cpp
std::vector<double> v = {1.0, 2.0, 3.0, 4.0};

for (std::vector<double>::iterator i = v.begin();
     i != v.end(); ++i) {
   std::cout << *i << std::endl;
}
~~~

This can end up looking quite verbose...

## Keyword `auto`

The keyword `auto`{.Cpp} (C++11) tells the compiler to infer the type of a variable.

~~~cpp
auto j = 3;    // j is ???
auto x = 1.2;  // x is ???

std::vector v = {1, 2, 3};     // C++17

auto s = v.size();             // s is ???
auto i = v.begin();            // i is ???
auto d = v.end() - v.begin();  // d is ???
~~~

## Keyword `auto`

The keyword `auto`{.Cpp} (C++11) tells the compiler to infer the type of a variable.

~~~cpp
auto j = 3;    // j is int
auto x = 1.2;  // x is double

std::vector v = {1, 2, 3};     // C++17

auto s = v.size();             // s is ???
auto i = v.begin();            // i is ???
auto d = v.end() - v.begin();  // d is ???
~~~

## Keyword `auto`

The keyword `auto`{.Cpp} (C++11) tells the compiler to infer the type of a variable.

~~~cpp
auto j = 3;    // j is int
auto x = 1.2;  // x is double

std::vector v = {1, 2, 3};     // C++17

auto s = v.size();             // s is std::size_t
auto i = v.begin();            // i is std::vector<int>::iterator
auto d = v.end() - v.begin();  // d is std::ptrdiff_t
~~~

## Keyword `auto`

Use `auto`{.Cpp} when you **don't know** or when you **don't care** what the type of the variable is.

Often, we don't care what the type of a variable is - we're happy for the compiler to 'do the right thing'.
Replacing it with `auto`{.Cpp} *can* make our code easier to read and less prone to errors.

Sometimes *only* the compiler knows the type of a variable - we'll see this later with lambdas.


## Iterator-based `for` loop using `auto`

In the context of an iterator-based `for` loop, we can simplify the syntax by using `auto`{.Cpp} to infer the type returned by `v.begin()`:

~~~cpp
std::vector<double> v = {1.0, 2.0, 3.0, 4.0};

for (auto i = v.begin(); i != v.end(); ++i) {
   std::cout << *i << std::endl;
}
~~~

Not only does it look nicer, but it's **easier to read** and **less prone to errors**.

## Range-based loops

Range-based loops have the most compact syntax and are often the most intuitive to use.
They work with any container that defines `begin` and `end` methods.

~~~cpp
std::vector<double> v = {1.0, 2.0, 3.0, 4.0};

for (double x: v) {
   std::cout << x << std::endl;
}
~~~

## Range-based loops using `auto`

You can use `auto`{.Cpp} here if you don't care about the type...

~~~cpp
std::vector<double> v = {1.0, 2.0, 3.0, 4.0};

for (auto x: v) {
   std::cout << x << std::endl;  // x is a value
}
~~~

## Range-based loops using `auto` with qualifiers

You can use `auto&`{.Cpp} if you want a reference...

~~~cpp
std::vector<double> v = {1.0, 2.0, 3.0, 4.0};

for (auto& x: v) {
   x += 1.0;  // x is a reference
}
~~~

## Range-based loops using `auto` with qualifiers

You can use `const auto&`{.Cpp} if you want a const reference...

~~~cpp
std::vector<double> v = {1.0, 2.0, 3.0, 4.0};

for (const auto& x: v) {
   std::cout << x << std::endl;  // x is a const reference
}
~~~