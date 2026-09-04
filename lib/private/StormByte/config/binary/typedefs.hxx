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

#include <StormByte/config/config.hxx>
#include <StormByte/exception.hxx>
#include <StormByte/expected.hxx>

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

/**
 * @namespace StormByte::Config::Binary
 * @brief Versioned binary wire format for configuration documents.
 */
namespace StormByte::Config::Binary {
	/**
	 * @brief Wire format version written by this library; maximum supported on read.
	 */
	inline constexpr std::uint8_t CurrentVersion = 1;

	/**
	 * @brief File magic: "STBTCF" followed by 0x02 0x9A (0x029A == 666).
	 */
	inline constexpr std::array<std::byte, 8> Magic = {
		std::byte{0x53}, std::byte{0x54}, std::byte{0x42}, std::byte{0x54},
		std::byte{0x43}, std::byte{0x46}, std::byte{0x02}, std::byte{0x9A}
	};

	/**
	 * @brief Magic + format version size (bytes before payload).
	 */
	inline constexpr std::size_t HeaderSize = Magic.size() + sizeof(std::uint8_t);

	/**
	 * @brief Result of a binary load operation.
	 */
	using ExpectedConfig = StormByte::Expected<Config, StormByte::DeserializeError>;

	/**
	 * @brief Byte buffer used for binary payloads.
	 */
	using Buffer = std::vector<std::byte>;

	/**
	 * @brief Read-only view of a binary buffer.
	 */
	using BufferView = std::span<const std::byte>;
}
