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
namespace StormByte::Config::Parser {
	/**
	 * @enum Mode
	 * @brief Parsing mode
	 */
	enum class Mode: unsigned short {
		Named, 		///< Named items
		Unnamed 	///< Unnamed items
	};

	/**
	 * @enum CommentType
	 * @brief Comment type
	 */
	enum class CommentType: unsigned short {
		None, 			///< No comment
		SingleLineBash, ///< SingleLine bash like comment
		SingleLineC, 	///< SingleLine C/C++ comment
		MultiLineC 		///< MultiLine C/C++ comment
	};
};
