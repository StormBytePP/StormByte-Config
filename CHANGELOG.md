# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Summary]

StormByte Config is the configuration-document module of the StormByte C++ suite.

It depends on StormByte Base. This repository is not Base, Buffer, Crypto, Database, Logger, Multimedia, Network or System.

Public headers under `StormByte/config/` cover text and versioned binary documents: values, groups, lists, comments, hooks and `Save` / `Load`.

If you landed here from a release link and have not read the tree:

- What this module is, how to build it, and short examples: [README.md](https://github.com/StormBytePP/StormByte-Config/blob/master/README.md)
- License: GNU Lesser General Public License version 3 or later, [LICENSE](https://github.com/StormBytePP/StormByte-Config/blob/master/LICENSE)

## [Unreleased]

### Fixed

- **Container path exception safety**: Prevented stock C++ exception leaks (`std::out_of_range`, `std::invalid_argument`) when looking up or removing numeric or malformed container paths, throwing `StormByte::Config::OutOfBounds` or `StormByte::Config::InvalidPath` instead.
- **Double precision serialization**: Switched `Value<double>::Serialize` to `std::format` for full floating-point precision and ensured integer-like floating point values maintain a `.0` suffix so they are recognized as Double tokens by the parser.

### Changed

- **Exception handling**: Ported `StormByte::Config::Exception` constructors to pass `StormByte::Component("Config")` introduced in StormByte Base 1.1.0, automatically formatting exception messages with `StormByte::Config: `.
- **Dependencies**: Requires [StormByte Base](https://github.com/StormBytePP/StormByte/releases/tag/1.1.0) ≥ 1.1.0.
- **Type traits**: Replaced stock C++ type traits (`std::is_same_v`, `std::is_base_of_v`) with StormByte flavor equivalents (`StormByte::Type::SameAs`, `StormByte::Type::DerivedFrom`).

## [1.0.0] - 2026-09-05

Initial public release of StormByte Config.

### Added

- `Config` document with `operator<<` / `operator>>` for text
- `Save` / `Load` with `Mode::Text` and `Mode::Binary`
- Versioned binary envelope: magic `STBTCF` + format version + payload
- Values: string, integer, double, boolean, binary (`std::vector<std::byte>`)
- Text binary values as Base64 `b"..."`; raw bytes on the binary wire
- Comments: `#`, `//`, `/* */`
- Containers: lists `[]` and groups `{}`
- Pre/post read hooks (`AddHookBeforeRead` / `AddHookAfterRead`)
- `OnExistingAction` (`Keep`, `Overwrite`, `ThrowException`; default is throw)
- Project version read from the `VERSION` file
- CMake 3.28 floor

### Notes

- Stream operators are text-only. Use `Save` / `Load` for binary.
- Older supported binary versions load fully; newer versions are rejected.
- Save always writes the current format version.
- Needs a C++26 compiler and StormByte Base ≥ 1.0.0.

[1.0.0]: https://github.com/StormBytePP/StormByte-Config/releases/tag/1.0.0
