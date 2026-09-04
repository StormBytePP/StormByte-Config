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

#include <StormByte/config/binary/typedefs.hxx>
#include <StormByte/config/visibility.h>

/**
 * @namespace StormByte::Config::Binary
 * @brief Versioned binary wire format for configuration documents.
 */
namespace StormByte::Config::Binary {
	/**
	 * @class Reader
	 * @brief Deserializes a versioned binary buffer into a @ref Config.
	 *
	 * Rejects bad magic, truncated headers, and versions greater than
	 * @ref CurrentVersion. Friend of @ref Config.
	 */
	class STORMBYTE_CONFIG_PRIVATE Reader {
		public:
			/**
			 * @brief Construct a reader over @p data.
			 * @param data Binary input (must remain valid for Deserialize).
			 */
			explicit Reader(BufferView data) noexcept;

			Reader(const Reader&) = delete;
			Reader(Reader&&) noexcept = delete;
			~Reader() noexcept = default;

			Reader& operator=(const Reader&) = delete;
			Reader& operator=(Reader&&) noexcept = delete;

			/**
			 * @brief Parse header and payload.
			 * @return Config or DeserializeError.
			 */
			ExpectedConfig Deserialize() const noexcept;

		private:
			BufferView m_data; ///< Input buffer.
	};
}
