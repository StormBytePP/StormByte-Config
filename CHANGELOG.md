# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-08-20

Initial public release of **StormByte-Config**: a hierarchical configuration library with text and versioned binary I/O, groups, lists, typed values and comment support.

### Added

- `Config` facade with stream/string I/O operators and path/index access
- Item model: `Base`, `Value<T>`, `Comment<CommentType>`, `Container`, `Group`, `List`
- Supported value types: `bool`, `int`, `double`, `std::string`, binary (`std::vector<std::byte>` via Base64 `b"..."` in text form)
- Comment styles: bash (`#`), C++ single-line (`//`), C multi-line (`/* */`)
- Nested groups and lists with path lookup (`a/b/c` and numeric list indices)
- `OnExistingAction` policies: `Keep`, `Overwrite`, `ThrowException` (propagated to nested containers)
- Pre/post-read hooks and optional parse-failure hook
- `Mode` enum (`Text`, `Binary`) and document API `Config::Save` / `Config::Load` on `std::ostream` / `std::istream`
- Versioned binary envelope (magic + format version + payload): readers understand supported older versions; writers always emit the current version; newer versions are rejected without guessing the layout
- Private binary `Reader` / `Writer` implementation (no public per-item binary serializers)
- Tokenizer + recursive-descent parser for human-readable config files
- Comprehensive unit tests (parse, binary golden/legacy/bad-magic/bad-version, corruption/stress, ASAN/UBSan-hardened paths)

### Fixed

- String text serialization now escapes `"`, `\`, `\n`, `\r`, `\t` for correct round-trips
- Binary `bool` handling relies on Base rejecting non-`0`/`1` wire bytes (avoids UB under sanitizers)
- Reader converts domain exceptions during load into structured errors instead of aborting
- Normalized header style (no column-aligned declarations)

### Changed

- Binary configuration I/O is document-oriented (`Save`/`Load` + `Mode::Binary`), not `StormByte::Serializable<Config>` specializations
- Binary documents without a valid magic are not accepted as Config binaries (including bare legacy payloads)

### Notes

- Requires C++23 and StormByte Base ≥ 1.0.0 (including safe `bool` deserialization)
- Binary items use Base64 in text form; invalid Base64 is rejected at parse time
- Lists reject named items; Groups require valid identifier names
- Format upgrades: load any still-supported binary revision, then save to upgrade on disk/network without losing logical content

[1.0.0]: https://github.com/StormBytePP/StormByte-Config/releases/tag/1.0.0