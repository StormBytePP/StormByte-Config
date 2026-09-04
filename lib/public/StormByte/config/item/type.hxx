/*
 * Copyright (C) 2024-2026 David C. Manuelda (StormBytePP)
 *
 * This file is part of StormByte-Config.
 *
 * StormByte-Config is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * or later, as published by the Free Software Foundation.
 *
 * StormByte-Config is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with StormByte-Config. If not, see
 * <https://www.gnu.org/licenses/lgpl-3.0.html>.
 */

#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/visibility.h>

#include <concepts>
#include <cstddef>
#include <string>
#include <type_traits>
#include <vector>

/**
 * @brief Configuration items (values, comments, groups, lists).
 */
namespace StormByte::Config::Item {
	/**
	 * @enum Type
	 * @brief Represents the type of a configuration item.
	 */
	enum class Type: char {
		Bool,       ///< Boolean item.
		Comment,    ///< Comment item.
		Container,  ///< Container item.
		Double,     ///< Double-precision floating-point item.
		Integer,    ///< Integer item.
		String,     ///< String item.
		Binary      ///< Binary data (`std::vector<std::byte>`; text form is Base64 `b"..."`)
	};

	/**
	 * @brief Converts a Type to its string representation.
	 * @param t The Type to convert.
	 * @return The string representation of the Type.
	 */
	static constexpr std::string TypeToString(const Type& t) noexcept {
		switch(t) {
			case Type::String:		return "String";
			case Type::Integer:		return "Integer";
			case Type::Double:		return "Double";
			case Type::Comment:		return "Comment";
			case Type::Bool:		return "Bool";
			case Type::Container:	return "Container";
			case Type::Binary:		return "Binary";
			default:				return "Unknown";
		}
	}

	/**
	 * @enum CommentType
	 * @brief Comment syntax used in the text document.
	 */
	enum class STORMBYTE_CONFIG_PUBLIC CommentType: char {
		SingleLineBash = 2,		///< `#` until end of line
		SingleLineC = 5,		///< `//` until end of line
		MultiLineC = 8			///< `/*` … `*/`
	};

	/**
	 * @brief Converts a CommentType to a human-readable string.
	 * @param t Comment type to convert.
	 * @return String representation.
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC std::string TypeToString(const CommentType& t) noexcept {
		switch(t) {
			case CommentType::SingleLineBash:	return "Bash like single line comment";
			case CommentType::SingleLineC:		return "C++ like single line comment";
			case CommentType::MultiLineC:		return "C/C++ like multi line comment";
			default:							return "Unknown";
		}
	}

	/**
	 * @enum ContainerType
	 * @brief Group (`{}`) or list (`[]`).
	 */
	enum class STORMBYTE_CONFIG_PUBLIC ContainerType: char {
		Group,	///< Group of named items
		List	///< List of unnamed items
	};

	/**
	 * @brief Converts a ContainerType to a string.
	 * @param t Container type to convert.
	 * @return String representation.
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC std::string TypeToString(const ContainerType& t) noexcept {
		switch(t) {
			case ContainerType::Group:	return "Group";
			case ContainerType::List:	return "List";
			default:					return "Unknown";
		}
	}

	/**
	 * @brief Resolves a container type from its opening character.
	 * @param start `{` or `[`.
	 * @return Container type.
	 * @throw Exception if the character is not a container opener.
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC ContainerType TypeFromStartCharacter(const char& start) {
		switch(start) {
			case '{':	return ContainerType::Group;
			case '[':	return ContainerType::List;
			default:	throw Exception("Unknown start character " + std::string(1, start) + " for container");
		}
	}

	/**
	 * @brief Value types allowed in `Item::Value<T>`.
	 */
	template<typename T>
	concept AllowedValueType =
		std::is_same_v<T, int> ||
		std::is_same_v<T, double> ||
		std::is_same_v<T, bool> ||
		std::is_same_v<T, std::string> ||
		std::is_same_v<T, std::vector<std::byte>>;
}
