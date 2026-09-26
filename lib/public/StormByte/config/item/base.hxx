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

#include <StormByte/clonable.hxx>
#include <StormByte/config/exception.hxx>
#include <StormByte/config/item/type.hxx>
#include <StormByte/string/string.hxx>
#include <StormByte/type_traits.hxx>

#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

/**
 * @brief Configuration items (values, comments, groups, lists).
 */
namespace StormByte::Config::Item {
	class Container;
	class Group;
	class List;
	template<AllowedValueType T> class Value;
	bool STORMBYTE_CONFIG_PUBLIC IsNameValid(const StormByte::String::String&) noexcept;

	/**
	 * @class Base
	 * @brief The base class for all configuration items.
	 */
	class STORMBYTE_CONFIG_PUBLIC Base: public Clonable<Base, StormByte::Shared<Base>> {
		public:
			/**
			 * @brief Default constructor.
			 */
			Base() = default;

			/**
			 * @brief Constructs a Base item with a name.
			 * @param name The name of the item.
			 */
			Base(const StormByte::String::String& name);

			/**
			 * @brief Copy constructor.
			 * @param base Item to copy.
			 */
			Base(const Base& base) = default;

			/**
			 * @brief Move constructor.
			 * @param base Item to move.
			 */
			Base(Base&& base) noexcept = default;

			/**
			 * @brief Copy assignment operator.
			 * @param base Item to copy.
			 * @return Reference to this Base.
			 */
			Base& operator=(const Base& base) = default;

			/**
			 * @brief Move assignment operator.
			 * @param base Item to move.
			 * @return Reference to this Base.
			 */
			Base& operator=(Base&& base) noexcept = default;

			/**
			 * @brief Destructor.
			 */
			virtual ~Base() noexcept = default;

			/**
			 * @brief Equality operator.
			 * @param other Item to compare.
			 * @return true if both items are equal.
			 */
			bool operator==(const Base& other) const noexcept {
				return this->Equals(other);
			}

			/**
			 * @brief Inequality operator.
			 * @param other Item to compare.
			 * @return true if items are not equal.
			 */
			bool operator!=(const Base& other) const noexcept {
				return !(*this == other);
			}

			/**
			 * @brief Polymorphic equality comparison.
			 * @param other The other item to compare against.
			 * @return true if both items are semantically equal.
			 */
			virtual bool Equals(const Base& other) const noexcept = 0;

			/**
			 * @brief Gets the name of the item.
			 * @return The name of the item.
			 */
			constexpr const std::optional<StormByte::String::String>& Name() const noexcept {
				return m_name;
			}

			/**
			 * @brief Sets the item name.
			 * @param name New name.
			 */
			constexpr void Name(const StormByte::String::String& name) noexcept {
				m_name = name;
			}

			/**
			 * @brief Sets the item name from a view.
			 * @param name New name. Copied into a @ref StormByte::String::String.
			 */
			void Name(std::string_view name) noexcept {
				m_name = StormByte::String::String(name);
			}

			/**
			 * @brief Checks if the current name is valid.
			 * @return true if the name is valid.
			 */
			inline bool IsNameValid() const noexcept {
				return m_name.has_value() && Item::IsNameValid(m_name.value());
			}

			/**
			 * @brief Gets the item type.
			 * @return Item type.
			 */
			constexpr virtual Type Type() const noexcept = 0;

			/**
			 * @brief Returns the comment type if this item is a Comment, otherwise std::nullopt.
			 *
			 * Used by the serialization layer to determine the concrete Comment specialization
			 * without relying on RTTI / dynamic_cast.
			 *
			 * @return The CommentType if this is a comment, std::nullopt otherwise.
			 */
			virtual std::optional<CommentType> GetCommentType() const noexcept {
				return std::nullopt;
			}

			/**
			 * @brief Gets the item type as a string.
			 * @return Item type as string.
			 */
			constexpr std::string_view TypeToString() const noexcept {
				return Item::TypeToString(this->Type());
			}

			/**
			 * @brief Serializes the item.
			 * @param indent_level Indentation level.
			 * @return Serialized text owned by StormByte-String.
			 */
			virtual StormByte::String::String Serialize(const int& indent_level) const noexcept;

			/**
			 * @brief Converts the item to a string.
			 * @return Serialized representation on the caller heap.
			 */
			explicit operator std::string() const {
				return static_cast<std::string>(this->Serialize(0));
			}

			/**
			 * @brief Gets the item value (const).
			 * @tparam T Target value type.
			 * @return Const reference to the value.
			 * @throw WrongValueTypeConversion on type mismatch.
			 */
			template<typename T>
			const T& Value() const {
				if constexpr (StormByte::Type::DerivedFrom<T, std::remove_reference_t<decltype(*this)>>) {
					return static_cast<const T&>(*this);
				} else if constexpr (StormByte::Type::DerivedFrom<Item::Value<T>, std::remove_reference_t<decltype(*this)>>) {
					return *static_cast<const Item::Value<T>&>(*this);
				} else {
					throw WrongValueTypeConversion("Wrong value type {} while expecting {}", this->TypeToString(), typeid(T).name());
				}
			}

			/**
			 * @brief Gets the item value (mutable).
			 * @tparam T Target value type.
			 * @return Reference to the value.
			 * @throw WrongValueTypeConversion on type mismatch.
			 */
			template<typename T>
			T& Value() {
				return const_cast<T&>(static_cast<const Base&>(*this).Value<T>());
			}

		protected:
			std::optional<StormByte::String::String> m_name; ///< Item name
	};
}
