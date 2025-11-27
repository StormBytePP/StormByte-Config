# StormByte

StormByte is a comprehensive, cross-platform C++ library aimed at easing system programming, configuration management, logging, and database handling tasks. This library provides a unified API that abstracts away the complexities and inconsistencies of different platforms (Windows, Linux).

## Features

- **Configuration Management**: Provides an intuitive API for reading and writing configuration files.
- **Serialization**: Specializations of `StormByte::Util::Serializable` for all items to enable serialization to raw buffers for network sending or binary writing.

## Table of Contents

- [Repository](#Repository)
- [Installation](#Installation)
- [Modules](#Modules)
	- [Base](https://dev.stormbyte.org/StormByte)
	- **Config**
	- [Crypto](https://dev.stormbyte.org/StormByte-Crypto)
	- [Database](https://dev.stormbyte.org/StormByte-Database)
	- [Memory](https://dev.stormbyte.org/StormByte-Memory)
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

The configuration supports various data types, including [string](#string), [integer](#integer), [double](#double), comments ([singleline](#singleline) and [multiline](#multiline)), and containers ([list](#list) and [group](#group)).

##### String

```plaintext
username = "example_user"
```

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const Item::Base& username = config["username"];
    std::cout << "Username: " << username.Value<std::string>() << std::endl;

    return 0;
}
```

##### Integer

```plaintext
timeout = 30
```

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const Item::Base& timeout = config["timeout"];
    std::cout << "Timeout: " << timeout.Value<int>() << std::endl;

    return 0;
}
```

##### Double

```plaintext
feature_timeout = 60.5
```

##### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const Item::Base& feature_timeout = config["feature_timeout"];
    std::cout << "Feature Timeout: " << feature_timeout.Value<double>() << std::endl;

    return 0;
}
```

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

####### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const Item::List& favorite_numbers = config["favorite_numbers"].Value<List>();
    std::cout << "Favorite Numbers: ";
    for (const auto& number : favorite_numbers)
		std::cout << (std::string)number << " ";
    std::cout << std::endl;

    return 0;
}
```

###### Group

Groups are nested configurations that can contain other key-value pairs, groups, or lists.

```plaintext
settings = {
    username = "example_user"
    timeout = 30
}
```

####### Example

```cpp
#include <StormByte/config/config.hxx>
#include <iostream>

using namespace StormByte::Config;

int main() {
    Config config;
    std::ifstream file("config.cfg");
    file >> config;
    file.close();

    const Item::Base& username = config["settings/username"];
    const Item::Base& timeout = config["settings/timeout"];
    
    std::cout << "Username: " << username.Value<std::string>() << std::endl;
    std::cout << "Timeout: " << timeout.Value<int>() << std::endl;

    return 0;
}
```

#### Serialization

The `Config` module supports serialization of configuration items to raw buffers, which can be useful for network transmission or binary storage. This is achieved through specializations of `StormByte::Util::Serializable`.

##### Example: Serialize and Deserialize a Configuration

```cpp
#include <StormByte/config/config.hxx>
#include <StormByte/util/serializable.hxx>
#include <fstream>
#include <iostream>

using namespace StormByte::Config;

int main() {
    // Create a configuration
    Config config;
    config["username"] = Item::Value<std::string>("username", "example_user");
    config["timeout"] = Item::Value<int>("timeout", 30);

    // Serialize the configuration to a buffer
    StormByte::Util::Serializable<Config> serializable(config);
    StormByte::Util::Buffer buffer = serializable.Serialize();

    // Deserialize the configuration from the buffer
    auto deserialized_config = StormByte::Util::Serializable<Config>::Deserialize(buffer);
    if (!deserialized_config) {
        std::cerr << "Failed to deserialize configuration: " << deserialized_config.error()->what() << std::endl;
        return 1;
    }

    // Access deserialized configuration items
    const Item::Base& username = deserialized_config.value()["username"];
    const Item::Base& timeout = deserialized_config.value()["timeout"];
    
    std::cout << "Username: " << username.Value<std::string>() << std::endl;
    std::cout << "Timeout: " << timeout.Value<int>() << std::endl;

    return 0;
}
```

#### Sharing Configuration

There are two options for sharing the configuration:

1. **Human-readable**: Share the configuration as a human-readable text file.
2. **Binary serialized**: Serialize the configuration to a binary format for network transmission or binary storage.

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under GPL v3 License - see the [LICENSE](LICENSE) file for details.
