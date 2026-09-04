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

#include <StormByte/config/binary/writer.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/helpers.hxx>
#include <StormByte/serializable.hxx>

#include <optional>
#include <string>

namespace StormByte::Config::Binary {
	namespace {
		using namespace StormByte::Config::Item;

		void WriteBase(Buffer& out, const Base& item) {
			append_vector(out, Serializable<Type>(item.Type()).Serialize());
			append_vector(out, Serializable<std::optional<std::string>>(item.Name()).Serialize());
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
					append_vector(out, Serializable<std::string>(item.Value<std::string>()).Serialize());
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
					append_vector(out, Serializable<std::string>(item.Value<std::string>()).Serialize());
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
