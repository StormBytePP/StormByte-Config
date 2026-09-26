# StormByte coding style

This is the flavor used in Base. Other suite modules follow it unless their own file says otherwise. Match the files already in the tree when something here is silent.

## Files

Headers are `.hxx`, sources `.cxx`, template bodies `.txx` included at the bottom of the header. Start every C or C++ file with `#pragma once` in the header and with the license banner used in this repository, unchanged. CMake and Markdown do not take that banner.

Include `StormByte/…` first (alphabetical among themselves), then a blank line, then the standard library (alphabetical). Do not `using namespace` in a header. `using namespace StormByte::Config;` in a `.cxx` after the includes is fine. Nested namespaces in a `.hxx` stop at three levels; deeper names use `A::B::C::Name`.

Indent with tabs. Spaces for indentation are wrong. Do not mix them to line up code; Doxygen `///<` on members may share a column by using tabs.

A blank line follows every function definition.

## Shape

Braces are K&R: the `{` sits on the same line as `class`, `struct`, `enum`, `namespace`, `if`, `for`, `while` or the function signature. `public:` / `private:` are one tab in; members one more.

A single-statement `if` / `else` / `else if` has no braces. Put `else` and `else if` on their own line, not on the same line as a closing `}`.

```
if (unit == 0 || remainder == 0)
	std::snprintf(...);
else {
	...
}
```

Pointers and references bind to the type: `const char* str`, `CString& other`, `operator const char*()`. Not `char *str`.

Types, enumerations and functions are PascalCase (`WriteValue`, `BeginPayload`, `OnExistingAction`). Macros are `SCREAMING_SNAKE` (`STORMBYTE_CONFIG_PUBLIC`, `WINDOWS`). One statement per line.

## Language

C++26. RAII: no bare `new` / `delete` in new code.

Public templates use `StormByte::Type` concepts. Do not put `std::enable_if`, `void_t` or a raw `std::is_*` next to those concepts.

`enum class` only. Converting constructors are `explicit` unless the type already documents an implicit conversion. Mark `noexcept` only when it is true. Prefer `constexpr` when there is no heap and no I/O.

Platform tests are `#ifdef WINDOWS`, `#elifdef MACOS`, `#else`. Not `#if defined(WINDOWS)`.

No anonymous namespace in a public header. An anonymous namespace in a `.cxx` is only for helpers used in that translation unit. Shared helpers go in `lib/private`.

## DLL boundary

`STORMBYTE_CONFIG_PUBLIC` comes **first** on a function declaration. clang-cl rejects `__declspec` after a reference return type.

```
STORMBYTE_CONFIG_PUBLIC Config& operator>>(std::istream& istream, Config& file);
```

Do not write `Config& STORMBYTE_CONFIG_PUBLIC Foo();`.

A class keeps the attribute on the type: `class STORMBYTE_CONFIG_PUBLIC Config`.

Do not repeat the attribute on a member of a class that is already exported.

Exported templates are split:

```
// header — ELF export + MSVC import/export of the declaration
extern template class STORMBYTE_CONFIG_PUBLIC Comment<CommentType::SingleLineBash>;

// .cxx — the body lives only here; consumers must not instantiate
template class STORMBYTE_CONFIG_INSTANTIATE Comment<CommentType::SingleLineBash>;
```

`STORMBYTE_CONFIG_INSTANTIATE` is `dllexport` on Windows shared builds and empty on ELF (so GCC does not warn `-Wattributes`). Never put it on the `extern` line. Never write `STORMBYTE_CONFIG_PUBLIC extern template`.

Do not repeat `STORMBYTE_CONFIG_PUBLIC` on an ordinary `.cxx` definition. Private implementation types use `STORMBYTE_CONFIG_PRIVATE`.

Values that leave the shared library are `StormByte::String::String`, `StormByte::BinaryData`, `StormByte::Size`, `StormByte::Shared<T>`, or a `const char*` owned by this library. Do not return `std::string` as the object that crosses the boundary unless the conversion is forced into the caller (`STORMBYTE_FORCE_INLINE`). `std::string_view` over an owned buffer is only valid inside the same module.

## Doxygen

Document every public declaration except `= delete`. Namespaces in a header get a `@namespace` block. Large classes use `@name` groups. `@ref` uses the qualified name (`StormByte::String::String`, `StormByte::Config::Item::Value`). Align member `///<` comments to the same column when they fit.

Wrap `extern template` noise in `/// @cond` / `/// @endcond` so it does not show up as a page of instantiations.

Do not put another module's sources in Doxygen `INPUT`. Cross-module symbols resolve with `TAGFILES`.

## Commits and tests

Conventional Commits in English (`feat:`, `fix:`, `docs:`, `test:`, `refactor:`). One topic per commit.

Test section banners are identical in the test body and in `main`:

```
// -------------------
// Construct
// -------------------
```

Do not invent `=== Construct ===` or print section titles with `cout`. Sections and test names are alphabetical.
