/*
* Copyright (C) 2024-2026 David C. Manuelda (StormBytePP)
*
* This file is part of StormByte.
*
* StormByte is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* StormByte is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with StormByte. If not, see <https://www.gnu.org/licenses/>.
*/

#include <StormByte/config/binary/reader.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/serializable.hxx>

#include <cstring>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace StormByte::Config::Binary {
	namespace {
		using namespace StormByte::Config::Item;

		bool MagicMatches(BufferView data) noexcept {
			if (data.size() < Magic.size())
				return false;
			return std::memcmp(data.data(), Magic.data(), Magic.size()) == 0;
		}

		Expected<std::pair<Type, std::optional<std::string>>, DeserializeError>
		ReadBase(BufferView data, std::size_t& offset) {
			if (offset >= data.size())
				return Unexpected<DeserializeError>("Insufficient data for item type");

			auto type = Serializable<Type>::Deserialize(data.subspan(offset));
			if (!type)
				return Unexpected(type.error());
			offset += Serializable<Type>::Size(type.value());

			if (offset >= data.size())
				return Unexpected<DeserializeError>("Insufficient data for item name");

			auto name = Serializable<std::optional<std::string>>::Deserialize(data.subspan(offset));
			if (!name)
				return Unexpected(name.error());
			offset += Serializable<std::optional<std::string>>::Size(name.value());

			return std::make_pair(type.value(), std::move(name.value()));
		}

		Expected<std::shared_ptr<Base>, DeserializeError>
		ReadItem(BufferView data, std::size_t& offset, std::uint8_t version);

		Expected<void, DeserializeError>
		ReadContainerInto(BufferView data, std::size_t& offset, Container& container, std::uint8_t version) {
			(void)version;
			auto basic = ReadBase(data, offset);
			if (!basic)
				return Unexpected(basic.error());

			auto [type, name] = std::move(basic.value());
			if (type != Type::Container)
				return Unexpected<DeserializeError>("Expected container item");
			if (name)
				container.Name(std::move(*name));

			if (offset >= data.size())
				return Unexpected<DeserializeError>("Insufficient data for container type");

			auto ctype = Serializable<ContainerType>::Deserialize(data.subspan(offset));
			if (!ctype)
				return Unexpected(ctype.error());
			offset += Serializable<ContainerType>::Size(ctype.value());

			if (ctype.value() != container.ContainerType())
				return Unexpected<DeserializeError>("Container type mismatch");

			if (offset >= data.size())
				return Unexpected<DeserializeError>("Insufficient data for container size");

			auto count = Serializable<std::uint64_t>::Deserialize(data.subspan(offset));
			if (!count)
				return Unexpected(count.error());
			offset += Serializable<std::uint64_t>::Size(count.value());

			if (count.value() > static_cast<std::uint64_t>(data.size() - offset))
				return Unexpected<DeserializeError>("Claimed container size exceeds buffer");

			for (std::uint64_t i = 0; i < count.value(); ++i) {
				auto child = ReadItem(data, offset, version);
				if (!child)
					return Unexpected(child.error());
				container.Add(std::move(child.value()), container.GetOnExistingAction());
			}
			return {};
		}

		Expected<std::shared_ptr<Base>, DeserializeError>
		ReadItem(BufferView data, std::size_t& offset, std::uint8_t version) {
			(void)version;
			const std::size_t start = offset;

			auto basic = ReadBase(data, offset);
			if (!basic)
				return Unexpected(basic.error());

			auto [type, name] = std::move(basic.value());

			auto apply_name = [&](Base& b) {
				if (name)
					b.Name(std::move(*name));
			};

			switch (type) {
				case Type::String: {
					auto value = Serializable<std::string>::Deserialize(data.subspan(offset));
					if (!value)
						return Unexpected(value.error());
					offset += Serializable<std::string>::Size(value.value());
					auto item = std::make_shared<Value<std::string>>(std::move(value.value()));
					apply_name(*item);
					return item;
				}
				case Type::Integer: {
					auto value = Serializable<int>::Deserialize(data.subspan(offset));
					if (!value)
						return Unexpected(value.error());
					offset += Serializable<int>::Size(value.value());
					auto item = std::make_shared<Value<int>>(value.value());
					apply_name(*item);
					return item;
				}
				case Type::Double: {
					auto value = Serializable<double>::Deserialize(data.subspan(offset));
					if (!value)
						return Unexpected(value.error());
					offset += Serializable<double>::Size(value.value());
					auto item = std::make_shared<Value<double>>(value.value());
					apply_name(*item);
					return item;
				}
				case Type::Bool: {
					auto value = Serializable<bool>::Deserialize(data.subspan(offset));
					if (!value)
						return Unexpected(value.error());
					offset += Serializable<bool>::Size(value.value());
					auto item = std::make_shared<Value<bool>>(value.value());
					apply_name(*item);
					return item;
				}
				case Type::Binary: {
					auto value = Serializable<std::vector<std::byte>>::Deserialize(data.subspan(offset));
					if (!value)
						return Unexpected(value.error());
					offset += Serializable<std::vector<std::byte>>::Size(value.value());
					auto item = std::make_shared<Value<std::vector<std::byte>>>(std::move(value.value()));
					apply_name(*item);
					return item;
				}
				case Type::Comment: {
					auto ct = Serializable<CommentType>::Deserialize(data.subspan(offset));
					if (!ct)
						return Unexpected(ct.error());
					offset += Serializable<CommentType>::Size(ct.value());

					auto text = Serializable<std::string>::Deserialize(data.subspan(offset));
					if (!text)
						return Unexpected(text.error());
					offset += Serializable<std::string>::Size(text.value());

					std::shared_ptr<Base> item;
					switch (ct.value()) {
						case CommentType::SingleLineBash:
							item = std::make_shared<Comment<CommentType::SingleLineBash>>(std::move(text.value()));
							break;
						case CommentType::SingleLineC:
							item = std::make_shared<Comment<CommentType::SingleLineC>>(std::move(text.value()));
							break;
						case CommentType::MultiLineC:
							item = std::make_shared<Comment<CommentType::MultiLineC>>(std::move(text.value()));
							break;
						default:
							return Unexpected<DeserializeError>("Unknown comment type");
					}
					apply_name(*item);
					return item;
				}
				case Type::Container: {
					offset = start;
					std::size_t peek = start;
					auto peek_basic = ReadBase(data, peek);
					if (!peek_basic)
						return Unexpected(peek_basic.error());
					if (peek >= data.size())
						return Unexpected<DeserializeError>("Insufficient data for container type");

					auto ctype = Serializable<ContainerType>::Deserialize(data.subspan(peek));
					if (!ctype)
						return Unexpected(ctype.error());

					std::shared_ptr<Container> container;
					if (ctype.value() == ContainerType::Group)
						container = std::make_shared<Group>();
					else if (ctype.value() == ContainerType::List)
						container = std::make_shared<List>();
					else
						return Unexpected<DeserializeError>("Unknown container type");

					auto filled = ReadContainerInto(data, offset, *container, version);
					if (!filled)
						return Unexpected(filled.error());
					return std::static_pointer_cast<Base>(container);
				}
				default:
					return Unexpected<DeserializeError>("Unhandled item type");
			}
		}

		Expected<std::pair<OnExistingAction, Group>, DeserializeError>
		ReadPayload(BufferView data, std::uint8_t version) {
			(void)version;
			std::size_t offset = 0;

			if (offset >= data.size())
				return Unexpected<DeserializeError>("Insufficient data for on_existing_action");

			auto policy = Serializable<std::optional<OnExistingAction>>::Deserialize(data.subspan(offset));
			if (!policy)
				return Unexpected(policy.error());
			offset += Serializable<std::optional<OnExistingAction>>::Size(policy.value());

			OnExistingAction resolved = OnExistingAction::ThrowException;
			if (policy.value())
				resolved = *policy.value();

			Group root;
			root.SetOnExistingAction(resolved);
			auto root_read = ReadContainerInto(data, offset, root, version);
			if (!root_read)
				return Unexpected(root_read.error());

			return std::make_pair(resolved, std::move(root));
		}
	}

	Reader::Reader(BufferView data) noexcept: m_data(data) {}

	ExpectedConfig Reader::Deserialize() const noexcept {
		try {
			if (m_data.size() < HeaderSize)
				return Unexpected<DeserializeError>("Truncated config binary header");

			if (!MagicMatches(m_data))
				return Unexpected<DeserializeError>("Not a StormByte config binary (bad magic)");

			auto version = Serializable<std::uint8_t>::Deserialize(m_data.subspan(Magic.size(), 1));
			if (!version)
				return Unexpected(version.error());

			if (version.value() > CurrentVersion) {
				return Unexpected<DeserializeError>(std::format(
					"Config binary version {} is newer than this library (max {})",
					version.value(), CurrentVersion));
			}
			if (version.value() < 1) {
				return Unexpected<DeserializeError>(std::format(
					"Unsupported config binary version {}", version.value()));
			}

			const BufferView payload = m_data.subspan(HeaderSize);

			Expected<std::pair<OnExistingAction, Item::Group>, DeserializeError> body;
			switch (version.value()) {
				case 1:
					body = ReadPayload(payload, version.value());
					break;
				default:
					return Unexpected<DeserializeError>(std::format(
						"Unsupported config binary version {}", version.value()));
			}

			if (!body)
				return Unexpected(body.error());

			Config cfg;
			cfg.m_on_existing_action = body.value().first;
			cfg.m_root = std::move(body.value().second);
			cfg.m_root.SetOnExistingAction(cfg.m_on_existing_action);
			return cfg;
		}
		catch (const StormByte::Exception& e) {
			return Unexpected<DeserializeError>(e.what());
		}
	}
}
