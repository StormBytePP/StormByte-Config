# StormByte-Config

![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey)
![C++26](https://img.shields.io/badge/C%2B%2B-26-00599C?logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.28+-064F8C?logo=cmake&logoColor=white)
![License: LGPL v3](https://img.shields.io/badge/License-LGPL_v3-blue.svg)
[![CI](https://github.com/StormBytePP/StormByte-Config/actions/workflows/ci.yml/badge.svg)](https://github.com/StormBytePP/StormByte-Config/actions/workflows/ci.yml)
[![Sponsor](https://img.shields.io/badge/Sponsor-StormBytePP-ea4aaa?logo=githubsponsors)](https://github.com/sponsors/StormBytePP)

This repository is **StormByte Config**: human-readable text and versioned binary documents for the StormByte C++ suite.

It depends on [StormByte Base](https://github.com/StormBytePP/StormByte). Public headers live under `StormByte/config/`.

The suite is split on purpose. Base, Buffer, Crypto, Database, Logger, Multimedia, Network and System are **other repositories**. This one does not implement them.

## What this module does

- **Text and binary I/O** — `Save` / `Load` with `Mode::Text` or `Mode::Binary` on any `std::ostream` / `std::istream`. Stream operators stay text-only.
- **Versioned binary** — magic `STBTCF` + format version. Older layouts load; newer ones are rejected; save always writes the current version.
- **Values** — string, integer, double, boolean, binary (`std::vector<std::byte>`: Base64 `b"..."` in text, raw bytes on the wire).
- **Comments** — `#`, `//`, `/* */`.
- **Containers** — lists `[]` and groups `{}`.
- **Hooks** — `AddHookBeforeRead` / `AddHookAfterRead`.
- **On existing** — overwrite, ignore, or throw (default).

## The rest of the suite

| Module | Role |
| --- | --- |
| [Base](https://github.com/StormBytePP/StormByte) | Exceptions, `Expected`, little-endian serialization, strings, concepts — the suite root |
| [Buffer](https://github.com/StormBytePP/StormByte-Buffer) | FIFO, SharedFIFO, Ring, Producer/Consumer and multi-stage pipelines |
| [Config](https://github.com/StormBytePP/StormByte-Config) | This repository |
| [Crypto](https://github.com/StormBytePP/StormByte-Crypto) | Hash, compress, encrypt, sign and key agreement — Crypto++ never leaves the private tree |
| [Database](https://github.com/StormBytePP/StormByte-Database) | One API for SQLite, PostgreSQL and MariaDB: prepared statements and RAII transactions |
| [Logger](https://github.com/StormBytePP/StormByte-Logger) | Stream logger with levels, headers, human-readable sizes and redaction (`ThreadedLog`) |
| [Multimedia](https://github.com/StormBytePP/StormByte-Multimedia) | Decode, encode and containers without raw FFmpeg types; codecs enabled only if present |
| [Network](https://github.com/StormBytePP/StormByte-Network) | Framed packets, Client/Server, IPv4/IPv6 TCP and Buffer pipelines (compress/encrypt) |
| [System](https://github.com/StormBytePP/StormByte-System) | Processes, pipes and environment variables across Linux, Windows and macOS |

Docs sites (when published): [Base](https://dev.stormbyte.org/StormByte), [Buffer](https://dev.stormbyte.org/StormByte-Buffer), [Config](https://dev.stormbyte.org/StormByte-Config), [Crypto](https://dev.stormbyte.org/StormByte-Crypto), [Database](https://dev.stormbyte.org/StormByte-Database), [Logger](https://dev.stormbyte.org/StormByte-Logger), [Multimedia](https://dev.stormbyte.org/StormByte-Multimedia), [Network](https://dev.stormbyte.org/StormByte-Network), [System](https://dev.stormbyte.org/StormByte-System).

## Table of Contents

- [What this module does](#what-this-module-does)
- [The rest of the suite](#the-rest-of-the-suite)
- [Installation](#installation)
- [Usage](#usage)
  - [Load from a stream](#load-from-a-stream)
  - [Binary Save / Load](#binary-save--load)
  - [Values and containers](#values-and-containers)
- [Contributing](#contributing)
- [License](#license)

## Installation

Needs a C++26 compiler, CMake 3.28 or newer, and [StormByte Base](https://github.com/StormBytePP/StormByte) ≥ 1.0.0.

```sh
git clone --recursive https://github.com/StormBytePP/StormByte-Config.git
cd StormByte-Config
cmake -S . -B build
cmake --build build
```

## Usage

Headers are `#include <StormByte/config/….hxx>`. Namespace root is `StormByte::Config`.

### Load from a stream

```cpp
#include <StormByte/config/config.hxx>
#include <fstream>

using namespace StormByte::Config;

int main() {
	Config config;
	std::ifstream file("config.cfg");
	file >> config;
}
```

Hooks: `AddHookBeforeRead` / `AddHookAfterRead`. Existing keys: `OnExistingAction::Overwrite` (default is throw).

### Binary Save / Load

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
}
```

### Values and containers

```plaintext
username = "example_user"
timeout = 30
feature_timeout = 60.5
enabled = true
payload = b"SGVsbG8gV29ybGQ="
favorite_numbers = [3 14 42 "pi constant"]
settings = {
	username = "example_user"
	timeout = 30
}
# bash comment
// C++ comment
/* multiline */
```

Binary values are `std::vector<std::byte>`. Text form is Base64 with `b"..."`; the binary document stores raw bytes.

## Contributing

Issues only on this repository. Fork and open a pull request against `master`.

## License

GNU Lesser General Public License version 3 or later. See [LICENSE](LICENSE) and <https://www.gnu.org/licenses/lgpl-3.0.html>.
