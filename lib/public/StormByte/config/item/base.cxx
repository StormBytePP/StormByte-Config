/*
* Copyright (C) 2024-2026 David C. Manuelda (StormBytePP)
*
* This file is part of StormByte-Config.
*
* StormByte-Config original source is dual-licensed:
*
* 1. GNU Lesser General Public License v3.0 (or later)
*    You may redistribute and/or modify this file under the terms of the
*    GNU Lesser General Public License as published by the Free Software
*    Foundation, either version 3 of the License, or (at your option)
*    any later version.
*
* 2. Commercial license
*    Alternatively, this file may be used under the terms of a commercial
*    license agreement with the copyright holder
*    (David C. Manuelda <StormByte@gmail.com>).
*
* Both licenses apply only to original StormByte-Config source in this
* repository. They do not cover other StormByte modules or any third-party
* material shipped with this repository (including everything under
* thirdparty/, and in particular the bundled StormByte-String tree and
* the StormByte Base tree it vendors), which remains under its own license.
*
* Neither license grants any patent rights. Any patent licenses required
* to use this software or third-party components must be obtained separately
* from the patent holders.
*
* StormByte-Config is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* version 3 along with StormByte-Config. If not, see
* <https://www.gnu.org/licenses/lgpl-3.0.html>.
*
* SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-StormByte-Commercial
*/

#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/serializable.hxx>
#include <StormByte/string/string.hxx>

#include <regex>
#include <string>
#include <string_view>

using namespace StormByte::Config::Item;
using StormByte::String::String;

namespace {
	String Indent(const int indent_level) {
		const int level = indent_level > 0 ? indent_level : 0;
		return String(std::string(static_cast<std::size_t>(level), '\t'));
	}
}

Base::Base(const class String& name): m_name(name) {}

String::String Base::Serialize(const int& indent_level) const noexcept {
	std::string serialized = static_cast<std::string>(Indent(indent_level));
	if (m_name) {
		serialized += static_cast<std::string>(*m_name);
		serialized += " = ";
	}
	return String::String(std::string_view(serialized));
}

namespace StormByte::Config::Item {
	bool IsNameValid(const class String& name) noexcept {
		static const std::regex name_regex(R"(^[A-Za-z][A-Za-z0-9_]*$)");
		return std::regex_match(static_cast<std::string>(name), name_regex);
	}
}
