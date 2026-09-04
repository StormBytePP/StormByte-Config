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

#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/serializable.hxx>
#include <StormByte/string.hxx>
#include <regex>
using namespace StormByte::Config::Item;
Base::Base(const std::string& name):m_name(name) {}
std::string Base::Serialize(const int& indent_level) const noexcept {
	std::string serialized = String::Indent(indent_level);
	if (m_name)
		serialized += *m_name + " = ";
	return serialized;
}
namespace StormByte::Config::Item {
	bool IsNameValid(const std::string& name) noexcept {
		static const std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*$)");
		return std::regex_match(name, name_regex);
	}
}
