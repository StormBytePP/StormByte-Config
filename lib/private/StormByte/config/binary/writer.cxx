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

#include <StormByte/config/binary/writer.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/helpers.hxx>
#include <StormByte/serializable.hxx>
#include <StormByte/string/serializable.hxx>
#include <StormByte/string/string.hxx>

#include <optional>

namespace StormByte::Config::Binary {
	namespace {
		using namespace StormByte::Config::Item;

		void WriteBase(Buffer& out, const Base& item) {
			append_vector(out, Serializable<Type>(item.Type()).Serialize());
			append_vector(out, Serializable<std::optional<StormByte::String::String>>(item.Name()).Serialize());
		}

		void WriteItem(Buffer& out, const Base& item, std::uint8_t version);
		void WriteContainer(Buffer& out, const Container& container, std::uint8_t version);

		void WriteContainer(Buffer& out, const Container& container, std::uint8_t version) {
			(void)version;
			WriteBase(out, container);
			append_vector(out, Serializable<ContainerType>(container.ContainerType()).Serialize());
			const auto count = static_cast<std::uint64_t>(container.Items().size());
			append_vector(out, Serializable<std::uint64_t>(count).Serialize());
			for (const auto& child : container.Items())
				WriteItem(out, *child, version);
		}

		void WriteItem(Buffer& out, const Base& item, std::uint8_t version) {
			(void)version;
			switch (item.Type()) {
				case Type::String:
					WriteBase(out, item);
					append_vector(out, Serializable<StormByte::String::String>(item.Value<StormByte::String::String>()).Serialize());
					break;
				case Type::Integer:
					WriteBase(out, item);
					append_vector(out, Serializable<int>(item.Value<int>()).Serialize());
					break;
				case Type::Double:
					WriteBase(out, item);
					append_vector(out, Serializable<double>(item.Value<double>()).Serialize());
					break;
				case Type::Bool:
					WriteBase(out, item);
					append_vector(out, Serializable<bool>(item.Value<bool>()).Serialize());
					break;
				case Type::Binary:
					WriteBase(out, item);
					append_vector(out, Serializable<std::vector<std::byte>>(
						item.Value<std::vector<std::byte>>()).Serialize());
					break;
				case Type::Comment: {
					WriteBase(out, item);
					const CommentType ct = *item.GetCommentType();
					append_vector(out, Serializable<CommentType>(ct).Serialize());
					append_vector(out, Serializable<StormByte::String::String>(item.Value<StormByte::String::String>()).Serialize());
					break;
				}

				case Type::Container:
					WriteContainer(out, static_cast<const Container&>(item), version);
					break;
			}
		}

		void WriteConfig(Buffer& out, OnExistingAction policy, const Group& root, std::uint8_t version) {
			(void)version;
			std::optional<OnExistingAction> pol = policy;
			append_vector(out, Serializable<std::optional<OnExistingAction>>(pol).Serialize());
			WriteContainer(out, root, version);
		}
	}

	Writer::Writer(const Config& config) noexcept: m_config(config) {}

	Buffer Writer::Serialize() const noexcept {
		Buffer out;
		out.reserve(HeaderSize + 64);
		out.insert(out.end(), Magic.begin(), Magic.end());
		append_vector(out, Serializable<std::uint8_t>(CurrentVersion).Serialize());

		const OnExistingAction policy = m_config.m_on_existing_action;
		const Item::Group& root = m_config.m_root;
		WriteConfig(out, policy, root, CurrentVersion);
		return out;
	}
}
