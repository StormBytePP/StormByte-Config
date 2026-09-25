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

#include <StormByte/base64.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/string/string.hxx>

#include <format>
#include <string>
#include <string_view>

namespace StormByte::Config::Item {
	template<>
	StormByte::String::String Value<StormByte::String::String>::Serialize(const int& indent_level) const noexcept {
		std::string escaped;
		const std::string_view raw = m_value;
		escaped.reserve(raw.size() + 8);
		for (char c : raw) {
			switch (c) {
				case '"':  escaped += "\\\""; break;
				case '\\': escaped += "\\\\"; break;
				case '\n': escaped += "\\n";  break;
				case '\r': escaped += "\\r";  break;
				case '\t': escaped += "\\t";  break;
				default:   escaped += c;      break;
			}
		}

		std::string out = static_cast<std::string>(Base::Serialize(indent_level));
		out += '"';
		out += escaped;
		out += '"';
		return StormByte::String::String(std::string_view(out));
	}

	template<>
	StormByte::String::String Value<int>::Serialize(const int& indent_level) const noexcept {
		std::string out = static_cast<std::string>(Base::Serialize(indent_level));
		out += std::to_string(m_value);
		return StormByte::String::String(std::string_view(out));
	}

	template<>
	StormByte::String::String Value<double>::Serialize(const int& indent_level) const noexcept {
		std::string str = std::format("{}", m_value);
		if (str.find('.') == std::string::npos && str.find('e') == std::string::npos && str.find('E') == std::string::npos) {
			str += ".0";
		}

		std::string out = static_cast<std::string>(Base::Serialize(indent_level));
		out += str;
		return StormByte::String::String(std::string_view(out));
	}

	template<>
	StormByte::String::String Value<bool>::Serialize(const int& indent_level) const noexcept {
		std::string out = static_cast<std::string>(Base::Serialize(indent_level));
		out += m_value ? "true" : "false";
		return StormByte::String::String(std::string_view(out));
	}

	template<>
	StormByte::String::String Value<std::vector<std::byte>>::Serialize(const int& indent_level) const noexcept {
		const std::string base64 = StormByte::Base64Encode(m_value);
		std::string out = static_cast<std::string>(Base::Serialize(indent_level));
		out += "b\"";
		out += base64;
		out += '"';
		return StormByte::String::String(std::string_view(out));
	}

	template class STORMBYTE_CONFIG_INSTANTIATE Value<StormByte::String::String>;
	template class STORMBYTE_CONFIG_INSTANTIATE Value<int>;
	template class STORMBYTE_CONFIG_INSTANTIATE Value<double>;
	template class STORMBYTE_CONFIG_INSTANTIATE Value<bool>;
	template class STORMBYTE_CONFIG_INSTANTIATE Value<std::vector<std::byte>>;
}
