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
