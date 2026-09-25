/*
 * Copyright (C) 2024-2026 David C. Manuelda (StormBytePP)
 *
 * This file is part of StormByte-Config.
 *
 * StormByte-Config original source is dual-licensed:
 *
 * 1. GNU Lesser General Public License v3.0 (or later)
 *    You may redistribute and/or modify this file under the terms of the
 *    GNU Lesser General Public License as published by the Free Software
 *    Foundation, either version 3 of the License, or (at your option)
 *    any later version.
 *
 * 2. Commercial license
 *    Alternatively, this file may be used under the terms of a commercial
 *    license agreement with the copyright holder
 *    (David C. Manuelda <StormByte@gmail.com>).
 *
 * Both licenses apply only to original StormByte-Config source in this
 * repository. They do not cover other StormByte modules or any third-party
 * material shipped with this repository (including everything under
 * thirdparty/, and in particular the bundled StormByte-String tree and
 * the StormByte Base tree it vendors), which remains under its own license.
 *
 * Neither license grants any patent rights. Any patent licenses required
 * to use this software or third-party components must be obtained separately
 * from the patent holders.
 *
 * StormByte-Config is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with StormByte-Config. If not, see
 * <https://www.gnu.org/licenses/lgpl-3.0.html>.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-StormByte-Commercial
 */

#pragma once

#include <StormByte/exception.hxx>
#include <StormByte/config/visibility.h>

/**
 * @brief Config module of the StormByte suite.
 */
namespace StormByte::Config {
	/**
	 * @class Exception
	 * @brief Base class for all configuration-related exceptions.
	 */
	class STORMBYTE_CONFIG_PUBLIC Exception: public StormByte::Exception {
		public:
			/**
			 * @brief Constructs from a message string with default Component("Config").
			 * @param message Exception text.
			 */
			explicit Exception(const std::string& message)
				: StormByte::Exception(Component("Config"), "{}", message) {}

			/**
			 * @brief Constructs from a moved message string with default Component("Config").
			 * @param message Exception text.
			 */
			explicit Exception(std::string&& message)
				: StormByte::Exception(Component("Config"), "{}", std::move(message)) {}

			/**
			 * @brief Constructs with std::format using default Component("Config").
			 * @tparam Args Format argument types.
			 * @param fmt Format string.
			 * @param args Format arguments.
			 */
			template <typename... Args>
			Exception(std::format_string<Args...> fmt, Args&&... args)
				: StormByte::Exception(Component("Config"), fmt, std::forward<Args>(args)...) {}

			/**
			 * @brief Constructs with a custom Component and std::format.
			 * @tparam Args Format argument types.
			 * @param component Module component wrapper.
			 * @param fmt Format string.
			 * @param args Format arguments.
			 */
			template <typename... Args>
			Exception(Component component, std::format_string<Args...> fmt, Args&&... args)
				: StormByte::Exception(component, fmt, std::forward<Args>(args)...) {}

			/**
			 * @brief Copy constructor.
			 * @param other Exception to copy.
			 */
			Exception(const Exception& other) = default;

			/**
			 * @brief Move constructor.
			 * @param other Exception to move.
			 */
			Exception(Exception&& other) noexcept = default;

			/**
			 * @brief Copy assignment operator.
			 * @param other Exception to copy.
			 * @return Reference to this Exception.
			 */
			Exception& operator=(const Exception& other) = default;

			/**
			 * @brief Move assignment operator.
			 * @param other Exception to move.
			 * @return Reference to this Exception.
			 */
			Exception& operator=(Exception&& other) noexcept = default;

			/**
			 * @brief Destructor.
			 */
			~Exception() noexcept override = default;
	};

	/**
	 * @class WrongValueTypeConversion
	 * @brief Thrown when a value is converted to an incorrect type.
	 */
	class STORMBYTE_CONFIG_PUBLIC WrongValueTypeConversion final: public Exception {
		public:
			using Exception::Exception;
	};

	/**
	 * @class ValueFailure
	 * @brief Thrown when setting a value fails due to type mismatch.
	 */
	class STORMBYTE_CONFIG_PUBLIC ValueFailure final: public Exception {
		public:
			using Exception::Exception;
	};

	/**
	 * @class InvalidName
	 * @brief Thrown when an invalid name is used for an item.
	 */
	class STORMBYTE_CONFIG_PUBLIC InvalidName final: public Exception {
		public:
			using Exception::Exception;
	};

	/**
	 * @class InvalidPath
	 * @brief Thrown when an invalid path is used for an item.
	 */
	class STORMBYTE_CONFIG_PUBLIC InvalidPath final: public Exception {
		public:
			using Exception::Exception;
	};

	/**
	 * @class ParseError
	 * @brief Thrown when a parsing error occurs in a configuration file.
	 */
	class STORMBYTE_CONFIG_PUBLIC ParseError final: public Exception {
		public:
			using Exception::Exception;
	};

	/**
	 * @class ItemNotFound
	 * @brief Thrown when an item cannot be found by its path or name.
	 */
	class STORMBYTE_CONFIG_PUBLIC ItemNotFound final: public Exception {
		public:
			using Exception::Exception;
	};

	/**
	 * @class ItemAlreadyExists
	 * @brief Thrown when attempting to add an item that already exists.
	 */
	class STORMBYTE_CONFIG_PUBLIC ItemAlreadyExists final: public Exception {
		public:
			using Exception::Exception;
	};

	/**
	 * @class ItemNameAlreadyExists
	 * @brief Thrown when an item's name conflicts with an existing name.
	 */
	class STORMBYTE_CONFIG_PUBLIC ItemNameAlreadyExists final: public Exception {
		public:
			using Exception::Exception;
	};

	/**
	 * @class OutOfBounds
	 * @brief Thrown when an index is out of bounds.
	 */
	class STORMBYTE_CONFIG_PUBLIC OutOfBounds final: public Exception {
		public:
			using Exception::Exception;
	};
}
