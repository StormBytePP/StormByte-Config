#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/visibility.h>

#include <cstddef>
#include <concepts>
#include <string>
#include <type_traits>
#include <vector>

/**
 * @namespace Item
 * @brief All the configuration item classes namespace
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
		Binary      ///< Binary data (stored as std::vector<std::byte>, text form is Base64 with b"..." prefix)
	};

	/**
	 * @brief Converts a Type to its string representation.
	 * @param t The Type to convert.
	 * @return The string representation of the Type.
	 */
	static constexpr std::string TypeToString(const Type& t) noexcept {
		switch(t) {
			case Type::String: 		return "String";
			case Type::Integer:		return "Integer";
			case Type::Double: 		return "Double";
			case Type::Comment:		return "Comment";
			case Type::Bool:		return "Bool";
			case Type::Container:	return "Container";
			case Type::Binary:		return "Binary";
			default:				return "Unknown";
		}
	}

	/**
	 * @enum CommentType
	 * @brief Comment type
	 */
	enum class STORMBYTE_CONFIG_PUBLIC CommentType: char {
		SingleLineBash = 2, 	///< Single line comment starting with # (bash style)
		SingleLineC = 5,		///< Single line comment starting with // (C++ style)
		MultiLineC = 8			///< Multi line comment starting with /* and ending with */ (C/C++ style)
	};

	/**
	 * Gets strings from Type
	 * @param t type to convert
	 * @return string
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC std::string TypeToString(const CommentType& t) noexcept {
		switch(t) {
			case CommentType::SingleLineBash: 	return "Bash like single line comment";
			case CommentType::SingleLineC: 		return "C++ like single line comment";
			case CommentType::MultiLineC: 		return "C/C++ like multi line comment";
			default: 							return "Unknown";
		}
	}

	/**
	 * @enum ContainerType
	 * @brief Container type
	 */
	enum class STORMBYTE_CONFIG_PUBLIC ContainerType: char {
		Group, 	///< Group of items
		List	///< List of items
	};

	/**
	 * Gets strings from Type
	 * @param t type to convert
	 * @return string
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC std::string TypeToString(const ContainerType& t) noexcept {
		switch(t) {
			case ContainerType::Group:	return "Group";
			case ContainerType::List:	return "List";
			default:					return "Unknown";
		}
	}

	/**
	 * Gets Type from start character
	 * @param start start character
	 * @return Type Container type
	 */
	constexpr STORMBYTE_CONFIG_PUBLIC ContainerType TypeFromStartCharacter(const char& start) {
		switch(start) {
			case '{': 			return ContainerType::Group;
			case '[': 			return ContainerType::List;
			default: 			throw Exception("Unknown start character " + std::string(1, start) + " for container");
		}
	}

	template<typename T>
	concept AllowedValueType =
		std::is_same_v<T, int> ||
		std::is_same_v<T, double> ||
		std::is_same_v<T, bool> ||
		std::is_same_v<T, std::string> ||
		std::is_same_v<T, std::vector<std::byte>>;

}