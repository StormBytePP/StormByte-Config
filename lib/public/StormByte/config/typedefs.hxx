#pragma once

#include <StormByte/exception.hxx>
#include <StormByte/expected.hxx>

/**
 * @namespace Config
 * @brief Contains all classes and enums for handling configuration files and items.
 */
namespace StormByte::Config {
	class Config; ///< Forward declaration of the configuration document.

	/**
	 * @enum OnExistingAction
	 * @brief Specifies the action to take when a name conflict occurs.
	 */
	enum class OnExistingAction: unsigned short {
		Keep,			///< Retain the existing item.
		Overwrite,		///< Replace the existing item with the new one.
		ThrowException	///< Throw an exception to indicate the conflict.
	};

	/**
	 * @enum Mode
	 * @brief Serialization / deserialization mode for configuration documents.
	 *
	 * Selects whether a document is exchanged as human-readable text (the classic
	 * StormByte config syntax) or as the versioned binary wire format.
	 *
	 * @see Config::Save
	 * @see Config::Load
	 */
	enum class Mode: unsigned short {
		Text,   ///< Human-readable configuration syntax (UTF-8 text).
		Binary  ///< Versioned binary format (magic + format version + payload).
	};

	/**
	 * @brief Result of loading a configuration document (text or binary).
	 *
	 * On failure, the error is a @ref StormByte::Exception derivative
	 * (for example @ref ParseError or @ref StormByte::DeserializeError).
	 */
	using ExpectedConfig = StormByte::Expected<Config, StormByte::Exception>;
}
