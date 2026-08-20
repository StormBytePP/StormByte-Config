/*
* Copyright (C) 2024-2026 David C. Manuelda (StormBytePP)
*
* This file is part of StormByte.
*
* StormByte is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* StormByte is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with StormByte. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <StormByte/config/binary/typedefs.hxx>
#include <StormByte/config/visibility.h>

/**
 * @namespace StormByte::Config::Binary
 * @brief Versioned binary wire format for configuration documents.
 */
namespace StormByte::Config::Binary {
	/**
	 * @class Writer
	 * @brief Serializes a @ref Config to the current binary format.
	 *
	 * Always writes @ref Magic and @ref CurrentVersion. Friend of @ref Config
	 * to access document internals without expanding the public API.
	 */
	class STORMBYTE_CONFIG_PRIVATE Writer {
		public:
			/**
			 * @brief Construct a writer for @p config.
			 * @param config Document to serialize (must outlive the writer).
			 */
			explicit Writer(const Config& config) noexcept;

			Writer(const Writer&) = delete;
			Writer(Writer&&) noexcept = delete;
			~Writer() noexcept = default;

			Writer& operator=(const Writer&) = delete;
			Writer& operator=(Writer&&) noexcept = delete;

			/**
			 * @brief Emit magic + CurrentVersion + payload.
			 * @return Binary buffer.
			 */
			Buffer Serialize() const noexcept;

		private:
			const Config& m_config; ///< Document being serialized.
	};
}
