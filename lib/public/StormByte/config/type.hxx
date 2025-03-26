#pragma once

/**
 * @namespace Config
 * @brief Contains all classes and enums for handling configuration files and items.
 */
namespace StormByte::Config {
	/**
	 * @enum OnExistingAction
	 * @brief Specifies the action to take when a name conflict occurs.
	 */
	enum class OnExistingAction: unsigned short {
		Keep,			///< Retain the existing item.
		Overwrite,		///< Replace the existing item with the new one.
		ThrowException	///< Throw an exception to indicate the conflict.
	};
}