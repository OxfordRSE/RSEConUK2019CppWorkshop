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

- type inference (`auto`{.cpp})
- move semantics
- uniform initialisation
- compile time programming (`constexpr`{.cpp})
- atomic operations
- variadic templates
- lambda expressions
- range-based `for`{.cpp} loops
- `<random>`{.cpp} number generation
- `<chrono>`{.cpp} time library
- *much, much more*

## A (very quick) overview of C++14

- more `constexpr`{.cpp}
- improved lambda support
- function return-type deduction
- digit separators
- standard user-defined literals

## A (very quick) overview of C++17

- more `constexpr`{.cpp}
- cross-platform filesystem library
- parallel STL algorithms
- structured bindings
- class template argument deduction
- mathematical special functions (`std::riemann_zeta`{.cpp}, ...)
- `std::string_view`{.cpp}
- `std::optional`{.cpp}, `std::any`{.cpp}, and `std::variant`{.cpp}

## A (very quick) overview of C++20

- more `constexpr`{.cpp}
- concepts
- modules
- ranges
- coroutines
- 'spaceship' `operator<=>`
- calendar and timezone support
- designated initializers
- `<version>`{.cpp} header
- `std::source_location`{.cpp}
 

## Is this all a bit overwhelming?

How are you supposed to know which features to use and how to use them well?

The trend in C++ has been to add features and then recommend a **reduced subset** of features and some **best practices** that will allow developers to write code that is:

- easier to write
- easier to read
- safer and less prone to errors
- with better performance by default
 

## Is this all a bit overwhelming?

To help navigate the labyrinth of new features and best practices, we have the [C++ core guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) together with a raft of static analysis tools such as [clang tidy](https://clang.llvm.org/extra/clang-tidy/).

But the best way to learn is to play around with new features, and that's what we're going to do today.

## Workshop overview

Today we are going to modernise some C++ code!

The code is broken into a number of checkpoints, and each practical session will get us from one checkpoint to the next.

First, let's:

- Log in to the VM with the details provided
- Grab the latest changes:

~~~bash
cd ~/RSEConUK2019CppWorkshop
git pull
~~~

## Workshop overview

Next, let's configure, build and run the first checkpoint to ensure everything is working for everyone in the room:

~~~bash
cd ~/RSEConUK2019CppWorkshop/build
cmake ..
cmake --build checkpoint_0
./checkpoint_0
~~~

Now, let's have a quick look through the code together.
Use your favourite text editor (`CLion`, `VSCode` and `Emacs` are all installed on the VM), open:

~~~bash
~/RSEConUK2019CppWorkshop/checkpoint_0/main.cpp
~~~

## Part 1 — The filesystem library

## Part 2 - the `for` loop

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

## Moving beyond the `for` loop: STL algorithms

Having just told you about...
