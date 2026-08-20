# StormByte
![Linux](https://img.shields.io/badge/Linux-Supported-1793D1?logo=linux&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-Supported-0078D6?logo=windows&logoColor=white)
![macOS](https://img.shields.io/badge/macOS-Supported-0078D6?logo=apple&logoColor=white)
![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.12+-064F8C?logo=cmake&logoColor=white)
![License: LGPL v3](https://img.shields.io/badge/License-LGPL_v3-blue.svg)
[![CI](https://github.com/StormBytePP/StormByte-Config/actions/workflows/ci.yml/badge.svg)](https://github.com/StormBytePP/StormByte-Config/actions/workflows/ci.yml)

StormByte is a comprehensive, cross-platform C++ library aimed at easing system programming, configuration management, logging, and database handling tasks. This library provides a unified API that abstracts away the complexities and inconsistencies of different platforms (Windows, Linux).

## Features

- **Configuration Management**: Provides an intuitive API for reading and writing configuration files.
- **Text and binary I/O**: `Config::Save` / `Config::Load` with `Mode::Text` or `Mode::Binary` on any `std::ostream` / `std::istream`.
- **Versioned binary format**: Magic header + format version so the library can evolve the wire layout without discarding documents written by older versions.
- **Binary data in config values**: Native `std::vector<std::byte>` values (Base64 in text form, raw bytes on the binary wire).

## Table of Contents

- [Repository](#Repository)
- [Installation](#Installation)
- [Modules](#Modules)
	- [Base](https://dev.stormbyte.org/StormByte)
	- [Buffer](https://dev.stormbyte.org/StormByte-Buffer)
	- **Config**
	- [Crypto](https://dev.stormbyte.org/StormByte-Crypto)
	- [Database](https://dev.stormbyte.org/StormByte-Database)
	- [Logger](https://dev.stormbyte.org/StormByte-Logger)
	- [Multimedia](https://dev.stormbyte.org/StormByte-Multimedia)
	- [Network](https://dev.stormbyte.org/StormByte-Network)
	- [System](https://dev.stormbyte.org/StormByte-System)
- [Contributing](#Contributing)
- [License](#License)

## Repository

You can visit the code repository at [GitHub](https://github.com/StormBytePP/StormByte-Config)

## Installation

### Prerequisites

Ensure you have the following installed:

- C++23 compatible compiler
- CMake 3.12 or higher

### Building

To build the library, follow these steps:

```sh
git clone https://github.com/StormBytePP/StormByte-Config.git
cd StormByte-Config
mkdir build
cd build
cmake ..
make
```

## Modules

StormByte Library is composed by several modules:

### Config

#### Overview

The `Config` module of StormByte provides a flexible and human-readable way to manage configuration files. It supports initialization from any `std::istream`, setting pre and post read hooks using `std::function`, handling different [data types](#data-types) and managing operation modes when items already exist before addition.

#### Initialization from `std::istream`

You can initialize the configuration from any `std::istream`, including `std::fstream`, `std::cin`, or even another `Config` object.

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <fstream>
#include <iostream>

using namespace StormByte::Config;

int main() {
	// Initialize from std::fstream
	Config config;
	std::ifstream file("config.cfg");
	file >> config;
	file.close();

	// Initialize from std::cin
	Config config2;
	std::cin >> config2;

	// Initialize from another Config object
	Config config3;
	config2 >> config3;

	return 0;
}
```

#### Hooks: Pre and Post Read

You can set pre and post read hooks using `std::function`. These hooks allow you to perform actions before and after reading the configuration, with the `Config` object passed as a reference argument.

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

void pre_read_hook(Item::Group& root) {
	std::cout << "Pre-read hook executed. Current config has " << root.Size() << " items." << std::endl;
}

void post_read_hook(Item::Group& root) {
	std::cout << "Post-read hook executed. Current config has " << root.Size() << " items." << std::endl;
}

int main() {
	Config config;
	config.AddHookBeforeRead(pre_read_hook);
	config.AddHookAfterRead(post_read_hook);

	// Read configuration (hooks will be called accordingly)
	std::ifstream file("config.cfg");
	file >> config;
	file.close();

	return 0;
}
```

#### Operation Modes for Existing Items

You can set the operation mode when an item already exists before adding a new one. Operation modes can include replace, ignore, or throw an exception (the default).

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
	Config config;

	// Set operation mode to replace existing items
	config.OnExistingAction(OnExistingAction::Overwrite);

	// Read configuration
	std::ifstream file("config.cfg");
	file >> config;
	file.close();

	return 0;
}
```

#### Data Types

The configuration supports various data types, including [string](#string), [integer](#integer), [double](#double), [boolean](#boolean), [binary](#binary), comments ([singleline](#singleline) and [multiline](#multiline)), and containers ([list](#list) and [group](#group)).

##### String

```plaintext
username = "example_user"
```

##### Integer

```plaintext
timeout = 30
```

##### Double

```plaintext
feature_timeout = 60.5
```

##### Boolean

```plaintext
enabled = true
debug = false
```

##### Binary

Binary data is stored internally as `std::vector<std::byte>`.  
In text form it is represented using Base64 encoding with the `b"..."` prefix.

```plaintext
payload = b"SGVsbG8gV29ybGQ="
secret  = b"U2VjcmV0RGF0YQ=="
```

###### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>
#include <vector>

using namespace StormByte::Config;

int main() {
	Config config;
	std::ifstream file("config.cfg");
	file >> config;
	file.close();

	const auto& payload = config["payload"].Value<std::vector<std::byte>>();

	std::cout << "Payload size: " << payload.size() << " bytes" << std::endl;
	// payload contains the raw binary data

	return 0;
}
```

When serializing to **binary** format, the raw `std::vector<std::byte>` is stored (no Base64 overhead).  
When serializing to **text**, it is automatically converted to Base64 with the `b"..."` prefix.

##### Comments

Configuration files can have comments!

###### Singleline

####### Bash like

Bash like comments start with `#` until the rest of the line

####### C/C++ like

C/C++ like comments start with `//` until the rest of the line

###### Multiline
Multiline comments are enclosed between `/*` and `*/` (like C/C++ style comments)

```plaintext
# This is a single-line comment
/**
* This is a multiline comment
*/
// int = 66; # Which is disabled
```

##### Containers

Configuration can have containers with subitems and also subcontainers

###### List

Lists are sequences of values enclosed in square brackets `[]` separated by spaces and can contain any other item (including nested lists and groups).

```plaintext
favorite_numbers = [3 14 42 "pi constant"]
```

###### Group

Groups are nested configurations that can contain other key-value pairs, groups, or lists.

```plaintext
settings = {
	username = "example_user"
	timeout = 30
}
```

#### Text and binary I/O (`Save` / `Load`)

Use `Config::Save` and `Config::Load` with a stream and a `Mode`:

| Mode | Meaning |
|------|---------|
| `Mode::Text` (default) | Human-readable config syntax (same path as `operator<<` / `operator>>`) |
| `Mode::Binary` | Versioned binary document (see below) |

##### Example: binary round-trip

```cpp
#include <StormByte/config/config.hxx>
#include <fstream>
#include <iostream>

using namespace StormByte::Config;

int main() {
	Config config;
	config.Add(Item::Value<std::string>("username", "example_user"));
	config.Add(Item::Value<int>("timeout", 30));

	{
		std::ofstream out("config.bin", std::ios::binary);
		config.Save(out, Mode::Binary);
	}

	std::ifstream in("config.bin", std::ios::binary);
	auto loaded = Config::Load(in, Mode::Binary);
	if (!loaded) {
		std::cerr << loaded.error()->what() << std::endl;
		return 1;
	}

	std::cout << loaded.value()["username"].Value<std::string>() << std::endl;
	return 0;
}
```

Stream operators remain the convenient path for **text** only (`file >> config`, `std::cout << config`, etc.).

#### Binary format and versioning

Binary documents are **not** a raw dump of C++ objects. They use a fixed envelope:

1. **Magic** (8 bytes): identifies a StormByte Config binary (`STBTCF` plus signature bytes).
2. **Format version** (`uint8_t`): layout revision of the payload that follows.
3. **Payload**: policy + root group and items for that version.

**Read policy**

- Older versions that this library still understands are **read fully** into a normal `Config` (no data loss for supported revisions).
- The current library **always writes** the latest format version.
- A version **newer** than this library is **rejected** with a clear error (upgrade the library; do not guess the layout).
- Missing or invalid magic, truncated headers, and unknown/unsupported versions are rejected instead of parsing garbage.

**Why this is safe**

- Format changes are intentional and versioned: readers can keep loading documents produced by previous releases of the same major product line as long as that version remains supported.
- You never “silently” reinterpret an unknown layout. Either the document loads completely, or `Load` fails with an error—so configuration is not partially applied or corrupted in place.
- After a successful load of an older file, saving again upgrades the on-disk/network form to the current version while preserving the logical content.

Buffers without a valid magic (including pure payload “legacy” blobs without the envelope) are **not** treated as Config binaries.

#### Sharing configuration

1. **Human-readable**: text file or stream (`Mode::Text` / stream operators). Binary values appear as Base64 with `b"..."`.
2. **Binary**: `Save` / `Load` with `Mode::Binary` for compact storage or network transfer (raw bytes for binary values, versioned envelope for the document).

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under LGPL v3 License - see the [LICENSE](LICENSE) file for details.
