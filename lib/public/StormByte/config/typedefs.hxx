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

#include <StormByte/exception.hxx>
#include <StormByte/expected.hxx>

/**
 * @brief Config module of the StormByte suite.
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
