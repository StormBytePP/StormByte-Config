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

#include <StormByte/config/item/comment.hxx>
#include <StormByte/string.hxx>
#include <sstream>
namespace StormByte::Config::Item {
	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Comment<CommentType::SingleLineBash>::Serialize(const int& indent_level) const noexcept {
		return String::Indent(indent_level) + "#" + m_value; // It is expected to start already indented
	}
	template class Comment<CommentType::SingleLineBash>;
	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Comment<CommentType::SingleLineC>::Serialize(const int& indent_level) const noexcept {
		return String::Indent(indent_level) + "//" + m_value; // It is expected to start already indented
	}
	template class Comment<CommentType::SingleLineC>;
	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Comment<CommentType::MultiLineC>::Serialize(const int& indent_level) const noexcept {
		// The MultiLineC comments already have the indent
		std::stringstream ss(m_value);
		std::string item;
		std::string serial = String::Indent(indent_level) + "/*";
		std::getline(ss, item);
		serial += item;
		if (!ss.eof()) {
			serial += "\n";
			while (std::getline(ss, item)) {
				serial += item; 
				if (!ss.eof()) serial += "\n";
			}
		}
		return serial + "*/";
	}
	template class Comment<CommentType::MultiLineC>;
}
