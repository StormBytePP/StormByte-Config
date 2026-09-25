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

#include <StormByte/config/item/base.hxx>
#include <StormByte/config/visibility.h>
#include <StormByte/string/string.hxx>
#include <StormByte/type_traits.hxx>

#include <cstddef>
#include <vector>

/**
 * @brief Configuration items (values, comments, groups, lists).
 */
namespace StormByte::Config::Item {
	/**
	 * @class Value
	 * @brief Named or unnamed typed value.
	 * @tparam T `StormByte::String::String`, `int`, `double`, `bool` or `std::vector<std::byte>`.
	 */
	template<AllowedValueType T>
	class STORMBYTE_CONFIG_PUBLIC Value: public Base {
		public:
			/**
			 * @name Construction
			 * @{
			 */
			/**
			 * @brief Constructs a Value with the given value.
			 * @param value The value of the item.
			 */
			Value(const T& value): Base(), m_value(value) {}

			/**
			 * @brief Constructs a string Value from a C string.
			 * @param value Source C string.
			 */
			template <typename U = T>
			Value(const char* value) requires StormByte::Type::SameAs<U, StormByte::String::String>
				: Base(), m_value(StormByte::String::String(value)) {}

			/**
			 * @brief Constructs a named string Value from C strings.
			 * @param name Item name.
			 * @param value Source C string.
			 */
			template <typename U = T>
			Value(const char* name, const char* value) requires StormByte::Type::SameAs<U, StormByte::String::String>
				: Base(StormByte::String::String(name)), m_value(StormByte::String::String(value)) {}

			/**
			 * @brief Move constructor from value.
			 * @param value Item value.
			 */
			Value(T&& value): Base(), m_value(std::move(value)) {}

			/**
			 * @brief Constructs a named Value.
			 * @param name Item name.
			 * @param value Item value.
			 */
			Value(const StormByte::String::String& name, const T& value): Base(name), m_value(value) {}

			/**
			 * @brief Constructs a named Value (move).
			 * @param name Item name.
			 * @param value Item value.
			 */
			Value(StormByte::String::String&& name, T&& value): Base(name), m_value(std::move(value)) {}

			/**
			 * @brief Named string constructor from C string value.
			 * @param name Item name.
			 * @param value C string value.
			 */
			Value(const StormByte::String::String& name, const char* value) requires StormByte::Type::SameAs<T, StormByte::String::String>
				: Base(name), m_value(StormByte::String::String(value)) {}

			/**
			 * @brief Named string constructor (move name) from C string value.
			 * @param name Item name.
			 * @param value C string value.
			 */
			Value(StormByte::String::String&& name, const char* value) requires StormByte::Type::SameAs<T, StormByte::String::String>
				: Base(name), m_value(StormByte::String::String(value)) {}

			/**
			 * @brief Copy constructor.
			 * @param single Item to copy.
			 */
			Value(const Value& single) = default;

			/**
			 * @brief Move constructor.
			 * @param single Item to move.
			 */
			Value(Value&& single) noexcept = default;

			/**
			 * @brief Copy assignment operator.
			 * @param single Item to copy.
			 * @return Reference to this Value.
			 */
			Value& operator=(const Value& single) = default;

			/**
			 * @brief Move assignment operator.
			 * @param single Item to move.
			 * @return Reference to this Value.
			 */
			Value& operator=(Value&& single) noexcept = default;

			/**
			 * @brief Destructor.
			 */
			virtual ~Value() noexcept override = default;
			/** @} */

			/**
			 * @name Access
			 * @{
			 */
			/**
			 * @brief Polymorphic equality comparison.
			 * @param other The other item to compare against.
			 * @return true if both items are of the same type and hold the same value.
			 */
			bool Equals(const Base& other) const noexcept override {
				if (this->Type() != other.Type())
					return false;
				if (this->Name() != other.Name())
					return false;
				const Value<T>& other_value = static_cast<const Value<T>&>(other);
				return m_value == other_value.m_value;
			}

			/**
			 * @brief Gets the type of the item.
			 * @return Item::Type The type of the item.
			 */
			constexpr virtual Item::Type Type() const noexcept override {
				if constexpr (StormByte::Type::SameAs<T, StormByte::String::String>) {
					return Item::Type::String;
				} else if constexpr (StormByte::Type::SameAs<T, int>) {
					return Item::Type::Integer;
				} else if constexpr (StormByte::Type::SameAs<T, double>) {
					return Item::Type::Double;
				} else if constexpr (StormByte::Type::SameAs<T, bool>) {
					return Item::Type::Bool;
				} else if constexpr (StormByte::Type::SameAs<T, std::vector<std::byte>>) {
					return Item::Type::Binary;
				}
			}

			/**
			 * @brief Checks if two Value objects are equal.
			 * @param single The Value object to compare.
			 * @return true if equal.
			 */
			bool operator==(const Value<T>& single) const noexcept {
				return Equals(single);
			}

			/**
			 * @brief Inequality operator.
			 * @param single Item to compare.
			 * @return true if not equal.
			 */
			bool operator!=(const Value<T>& single) const noexcept {
				return !operator==(single);
			}

			/**
			 * @brief Gets the item value (mutable).
			 * @return Item value.
			 */
			T& operator*() noexcept {
				return m_value;
			}

			/**
			 * @brief Gets the item value (const).
			 * @return Item value.
			 */
			const T& operator*() const noexcept {
				return m_value;
			}

			/**
			 * @brief Serializes the item to a string.
			 * @param indent_level The indentation level for serialization.
			 * @return The serialized text.
			 */
			StormByte::String::String Serialize(const int& indent_level) const noexcept override;

			/**
			 * @brief Clones the item.
			 * @return Cloned item.
			 */
			virtual PointerType Clone() const override {
				return MakePointer<Value<T>>(*this);
			}

			/**
			 * @brief Moves the item.
			 * @return Moved item.
			 */
			virtual PointerType Move() override {
				return MakePointer<Value<T>>(std::move(*this));
			}
			/** @} */

		protected:
			T m_value; ///< The value of the item.
	};

	Value(const char*) -> Value<StormByte::String::String>;
	Value(const char*, const char*) -> Value<StormByte::String::String>;

	/**
	 * @brief Serializes a string value.
	 * @param indent_level Indentation level.
	 * @return Serialized text.
	 */
	template<>
	STORMBYTE_CONFIG_PUBLIC StormByte::String::String Value<StormByte::String::String>::Serialize(const int& indent_level) const noexcept;

	/**
	 * @brief Serializes an integer value.
	 * @param indent_level Indentation level.
	 * @return Serialized text.
	 */
	template<>
	STORMBYTE_CONFIG_PUBLIC StormByte::String::String Value<int>::Serialize(const int& indent_level) const noexcept;

	/**
	 * @brief Serializes a double value.
	 * @param indent_level Indentation level.
	 * @return Serialized text.
	 */
	template<>
	STORMBYTE_CONFIG_PUBLIC StormByte::String::String Value<double>::Serialize(const int& indent_level) const noexcept;

	/**
	 * @brief Serializes a boolean value.
	 * @param indent_level Indentation level.
	 * @return Serialized text.
	 */
	template<>
	STORMBYTE_CONFIG_PUBLIC StormByte::String::String Value<bool>::Serialize(const int& indent_level) const noexcept;

	/**
	 * @brief Serializes a binary value.
	 * @param indent_level Indentation level.
	 * @return Serialized text.
	 */
	template<>
	STORMBYTE_CONFIG_PUBLIC StormByte::String::String Value<std::vector<std::byte>>::Serialize(const int& indent_level) const noexcept;

	extern template class STORMBYTE_CONFIG_PUBLIC Value<StormByte::String::String>;
	extern template class STORMBYTE_CONFIG_PUBLIC Value<int>;
	extern template class STORMBYTE_CONFIG_PUBLIC Value<double>;
	extern template class STORMBYTE_CONFIG_PUBLIC Value<bool>;
	extern template class STORMBYTE_CONFIG_PUBLIC Value<std::vector<std::byte>>;
}
