# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-08-20

Initial public release of **StormByte-Config**: a hierarchical configuration library with text and binary serialization, groups, lists, typed values and comment support.

### Added

- `Config` facade with stream/string I/O operators and path/index access
- Item model: `Base`, `Value<T>`, `Comment<CommentType>`, `Container`, `Group`, `List`
- Supported value types: `bool`, `int`, `double`, `std::string`, binary (`std::vector<std::byte>` via Base64 `b"..."`)
- Comment styles: bash (`#`), C++ single-line (`//`), C multi-line (`/* */`)
- Nested groups and lists with path lookup (`a/b/c` and numeric list indices)
- `OnExistingAction` policies: `Keep`, `Overwrite`, `ThrowException` (propagated to nested containers)
- Pre/post-read hooks and optional parse-failure hook
- Full binary serialization via StormByte `Serializable` (round-trip safe, hardened against truncated/malicious buffers)
- Tokenizer + recursive-descent parser for human-readable config files
- Comprehensive unit tests (parse, serialize, binary, corruption/stress)

### Fixed

- String text serialization now escapes `"`, `\`, `\n`, `\r`, `\t` for correct round-trips
- Removed unused `#include <iostream>` from group serialization
- Normalized header style (no column-aligned declarations)

### Notes

- Requires C++23 and StormByte Base ≥ 1.0.0
- Binary items use Base64 in text form; invalid Base64 is rejected at parse time
- Lists reject named items; Groups require valid identifier names

[1.0.0]: https://github.com/StormBytePP/StormByte-Config/releases/tag/1.0.0
