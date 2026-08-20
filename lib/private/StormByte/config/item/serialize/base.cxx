#include <StormByte/config/item/value.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

#include <format>
#include <memory>

namespace StormByte::Config::Item::Serialize {

	void Append(std::vector<std::byte>& out, std::vector<std::byte>&& part) noexcept {
		if (part.empty())
			return;
		out.reserve(out.size() + part.size());
		out.insert(out.end(), std::make_move_iterator(part.begin()), std::make_move_iterator(part.end()));
	}

	Expected<BaseData, DeserializeError> DeserializeBasicData(std::span<const std::byte> data, std::size_t& offset) noexcept {
		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for type");

		auto expected_type = Serializable<StormByte::Config::Item::Type>::Deserialize(data.subspan(offset));
		if (!expected_type) return Unexpected(expected_type.error());
		offset += Serializable<StormByte::Config::Item::Type>::Size(expected_type.value());

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for name");

		auto expected_name = Serializable<std::optional<std::string>>::Deserialize(data.subspan(offset));
		if (!expected_name) return Unexpected(expected_name.error());
		offset += Serializable<std::optional<std::string>>::Size(expected_name.value());

		return std::make_pair(std::move(expected_type.value()), std::move(expected_name.value()));
	}

	Expected<BaseData, DeserializeError> DeserializeBasicData(std::span<const std::byte> data, std::size_t& offset, const StormByte::Config::Item::Type& expected_item_type) noexcept {
		auto expected_base_data = DeserializeBasicData(data, offset);
		if (!expected_base_data) return Unexpected(expected_base_data.error());
		if (expected_base_data.value().first != expected_item_type) {
			return Unexpected<DeserializeError>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(expected_item_type),
					TypeToString(expected_base_data.value().first)
				)
			);
		}
		return expected_base_data.value();
	}

	Expected<std::shared_ptr<Base>, DeserializeError> DeserializeItem(std::span<const std::byte> data, std::size_t& offset) noexcept {
		const std::size_t start = offset;
		std::size_t peek = offset;
		auto expected_base_data = DeserializeBasicData(data, peek);
		if (!expected_base_data)
			return Unexpected(expected_base_data.error());

		const auto item_type = expected_base_data.value().first;
		const std::size_t pos_after_basic = peek;
		std::shared_ptr<Base> item_ptr;
		const auto slice = data.subspan(start);

		switch (item_type) {
			case StormByte::Config::Item::Type::String: {
				auto expected_item = Serializable<Value<std::string>>::Deserialize(slice);
				if (!expected_item) return Unexpected(expected_item.error());
				offset = start + Serializable<Value<std::string>>::Size(expected_item.value());
				item_ptr = std::make_shared<Value<std::string>>(std::move(expected_item.value()));
				break;
			}
			case StormByte::Config::Item::Type::Integer: {
				auto expected_item = Serializable<Value<int>>::Deserialize(slice);
				if (!expected_item) return Unexpected(expected_item.error());
				offset = start + Serializable<Value<int>>::Size(expected_item.value());
				item_ptr = std::make_shared<Value<int>>(std::move(expected_item.value()));
				break;
			}
			case StormByte::Config::Item::Type::Double: {
				auto expected_item = Serializable<Value<double>>::Deserialize(slice);
				if (!expected_item) return Unexpected(expected_item.error());
				offset = start + Serializable<Value<double>>::Size(expected_item.value());
				item_ptr = std::make_shared<Value<double>>(std::move(expected_item.value()));
				break;
			}
			case StormByte::Config::Item::Type::Bool: {
				auto expected_item = Serializable<Value<bool>>::Deserialize(slice);
				if (!expected_item) return Unexpected(expected_item.error());
				offset = start + Serializable<Value<bool>>::Size(expected_item.value());
				item_ptr = std::make_shared<Value<bool>>(std::move(expected_item.value()));
				break;
			}
			case StormByte::Config::Item::Type::Binary: {
				auto expected_item = Serializable<Value<std::vector<std::byte>>>::Deserialize(slice);
				if (!expected_item) return Unexpected(expected_item.error());
				offset = start + Serializable<Value<std::vector<std::byte>>>::Size(expected_item.value());
				item_ptr = std::make_shared<Value<std::vector<std::byte>>>(std::move(expected_item.value()));
				break;
			}
			case StormByte::Config::Item::Type::Comment: {
				if (pos_after_basic >= data.size())
					return Unexpected<DeserializeError>("Insufficient data for comment type");

				auto expected_comment_type = Serializable<CommentType>::Deserialize(data.subspan(pos_after_basic));
				if (!expected_comment_type) return Unexpected(expected_comment_type.error());

				switch (expected_comment_type.value()) {
					case CommentType::SingleLineBash: {
						auto expected_item = Serializable<Comment<CommentType::SingleLineBash>>::Deserialize(slice);
						if (!expected_item) return Unexpected(expected_item.error());
						offset = start + Serializable<Comment<CommentType::SingleLineBash>>::Size(expected_item.value());
						item_ptr = std::make_shared<Comment<CommentType::SingleLineBash>>(std::move(expected_item.value()));
						break;
					}
					case CommentType::SingleLineC: {
						auto expected_item = Serializable<Comment<CommentType::SingleLineC>>::Deserialize(slice);
						if (!expected_item) return Unexpected(expected_item.error());
						offset = start + Serializable<Comment<CommentType::SingleLineC>>::Size(expected_item.value());
						item_ptr = std::make_shared<Comment<CommentType::SingleLineC>>(std::move(expected_item.value()));
						break;
					}
					case CommentType::MultiLineC: {
						auto expected_item = Serializable<Comment<CommentType::MultiLineC>>::Deserialize(slice);
						if (!expected_item) return Unexpected(expected_item.error());
						offset = start + Serializable<Comment<CommentType::MultiLineC>>::Size(expected_item.value());
						item_ptr = std::make_shared<Comment<CommentType::MultiLineC>>(std::move(expected_item.value()));
						break;
					}
				}
				break;
			}
			case StormByte::Config::Item::Type::Container: {
				if (pos_after_basic >= data.size())
					return Unexpected<DeserializeError>("Insufficient data for container type");

				auto expected_container_type = Serializable<ContainerType>::Deserialize(data.subspan(pos_after_basic));
				if (!expected_container_type) return Unexpected(expected_container_type.error());

				switch (expected_container_type.value()) {
					case ContainerType::Group: {
						auto expected_item = Serializable<Group>::Deserialize(slice);
						if (!expected_item) return Unexpected(expected_item.error());
						offset = start + Serializable<Group>::Size(expected_item.value());
						item_ptr = std::make_shared<Group>(std::move(expected_item.value()));
						break;
					}
					case ContainerType::List: {
						auto expected_item = Serializable<List>::Deserialize(slice);
						if (!expected_item) return Unexpected(expected_item.error());
						offset = start + Serializable<List>::Size(expected_item.value());
						item_ptr = std::make_shared<List>(std::move(expected_item.value()));
						break;
					}
				}
				break;
			}
			default:
				return Unexpected<DeserializeError>("Unhandled item type");
		}

		if (!item_ptr)
			return Unexpected<DeserializeError>("Unknown error with item creation");
		if (offset <= start)
			return Unexpected<DeserializeError>("Invalid serialization: offset did not advance while deserializing item");

		return item_ptr;
	}

	Expected<std::shared_ptr<Container>, DeserializeError> DeserializeContainer(std::span<const std::byte> data, std::size_t& offset, std::shared_ptr<Container> container) noexcept {
		try {
			auto expected_base = DeserializeBasicData(data, offset, StormByte::Config::Item::Type::Container);
			if (!expected_base) return Unexpected(expected_base.error());
			auto [type, name] = expected_base.value();

			if (name.has_value())
				container->Name(std::move(name.value()));

			if (offset >= data.size())
				return Unexpected<DeserializeError>("Insufficient data for container type");

			auto container_type = Serializable<ContainerType>::Deserialize(data.subspan(offset));
			if (!container_type) return Unexpected(container_type.error());
			offset += Serializable<ContainerType>::Size(container_type.value());

			if (container_type.value() != container->ContainerType()) {
				return Unexpected<DeserializeError>(
					std::format(
						"Type mismatch: Tried {} but got {}",
						TypeToString(container->ContainerType()),
						TypeToString(container_type.value())
					)
				);
			}

			if (offset >= data.size())
				return Unexpected<DeserializeError>("Insufficient data for items size");

			auto items_size = Serializable<std::uint64_t>::Deserialize(data.subspan(offset));
			if (!items_size) return Unexpected(items_size.error());
			offset += Serializable<std::uint64_t>::Size(items_size.value());

			const std::size_t remaining = data.size() - offset;
			if (items_size.value() > remaining) {
				return Unexpected<DeserializeError>(
					std::format(
						"Claimed items_size ({}) exceeds remaining buffer size ({})",
						items_size.value(), remaining
					)
				);
			}

			for (std::uint64_t i = 0; i < items_size.value(); ++i) {
				auto expected_item = DeserializeItem(data, offset);
				if (!expected_item) return Unexpected(expected_item.error());
				container->Add(std::move(expected_item.value()), container->GetOnExistingAction());
			}

			return container;
		}
		catch (const StormByte::Config::Exception& ex) {
			return Unexpected<DeserializeError>(ex.what());
		}
		catch (const std::exception& ex) {
			return Unexpected<DeserializeError>(std::string("Unexpected exception during container deserialization: ") + ex.what());
		}
		catch (...) {
			return Unexpected<DeserializeError>("Unknown exception during container deserialization");
		}
	}

}

namespace StormByte {
	using namespace StormByte::Config::Item;

	// SizeComplex BEFORE SerializeComplex (avoids "specialization after instantiation")

	template<> STORMBYTE_CONFIG_PRIVATE
	std::size_t Serializable<Base>::SizeComplex(const Base& data) noexcept {
		return Serializable<StormByte::Config::Item::Type>::Size(data.Type()) +
			Serializable<std::optional<std::string>>::Size(data.Name());
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	std::vector<std::byte> Serializable<Base>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer;
		buffer.reserve(SizeComplex(m_data));
		append_vector(buffer, Serializable<StormByte::Config::Item::Type>(m_data.Type()).Serialize());
		append_vector(buffer, Serializable<std::optional<std::string>>(m_data.Name()).Serialize());
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	std::size_t Serializable<std::shared_ptr<Base>>::SizeComplex(const std::shared_ptr<Base>& data) noexcept {
		if (!data)
			return 0;
		switch (data->Type()) {
			case StormByte::Config::Item::Type::String:
				return Serializable<Value<std::string>>::Size(static_cast<Value<std::string>&>(*data));
			case StormByte::Config::Item::Type::Integer:
				return Serializable<Value<int>>::Size(static_cast<Value<int>&>(*data));
			case StormByte::Config::Item::Type::Double:
				return Serializable<Value<double>>::Size(static_cast<Value<double>&>(*data));
			case StormByte::Config::Item::Type::Bool:
				return Serializable<Value<bool>>::Size(static_cast<Value<bool>&>(*data));
			case StormByte::Config::Item::Type::Binary:
				return Serializable<Value<std::vector<std::byte>>>::Size(static_cast<Value<std::vector<std::byte>>&>(*data));
			case StormByte::Config::Item::Type::Comment: {
				auto opt = data->GetCommentType();
				if (opt) {
					switch (*opt) {
						case CommentType::SingleLineBash:
							return Serializable<Comment<CommentType::SingleLineBash>>::Size(static_cast<Comment<CommentType::SingleLineBash>&>(*data));
						case CommentType::SingleLineC:
							return Serializable<Comment<CommentType::SingleLineC>>::Size(static_cast<Comment<CommentType::SingleLineC>&>(*data));
						case CommentType::MultiLineC:
							return Serializable<Comment<CommentType::MultiLineC>>::Size(static_cast<Comment<CommentType::MultiLineC>&>(*data));
					}
				}
				break;
			}
			case StormByte::Config::Item::Type::Container:
				return Serializable<Container>::Size(static_cast<Container&>(*data));
		}
		return 0;
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	std::vector<std::byte> Serializable<std::shared_ptr<Base>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer;
		if (!m_data)
			return buffer;
		buffer.reserve(SizeComplex(m_data));

		switch (m_data->Type()) {
			case StormByte::Config::Item::Type::String:
				append_vector(buffer, Serializable<Value<std::string>>(static_cast<Value<std::string>&>(*m_data)).Serialize());
				break;
			case StormByte::Config::Item::Type::Integer:
				append_vector(buffer, Serializable<Value<int>>(static_cast<Value<int>&>(*m_data)).Serialize());
				break;
			case StormByte::Config::Item::Type::Double:
				append_vector(buffer, Serializable<Value<double>>(static_cast<Value<double>&>(*m_data)).Serialize());
				break;
			case StormByte::Config::Item::Type::Bool:
				append_vector(buffer, Serializable<Value<bool>>(static_cast<Value<bool>&>(*m_data)).Serialize());
				break;
			case StormByte::Config::Item::Type::Binary:
				append_vector(buffer, Serializable<Value<std::vector<std::byte>>>(static_cast<Value<std::vector<std::byte>>&>(*m_data)).Serialize());
				break;
			case StormByte::Config::Item::Type::Comment: {
				auto opt = m_data->GetCommentType();
				if (opt) {
					switch (*opt) {
						case CommentType::SingleLineBash:
							append_vector(buffer, Serializable<Comment<CommentType::SingleLineBash>>(static_cast<Comment<CommentType::SingleLineBash>&>(*m_data)).Serialize());
							break;
						case CommentType::SingleLineC:
							append_vector(buffer, Serializable<Comment<CommentType::SingleLineC>>(static_cast<Comment<CommentType::SingleLineC>&>(*m_data)).Serialize());
							break;
						case CommentType::MultiLineC:
							append_vector(buffer, Serializable<Comment<CommentType::MultiLineC>>(static_cast<Comment<CommentType::MultiLineC>&>(*m_data)).Serialize());
							break;
					}
				}
				break;
			}
			case StormByte::Config::Item::Type::Container:
				append_vector(buffer, Serializable<Container>(static_cast<Container&>(*m_data)).Serialize());
				break;
		}
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	Expected<std::shared_ptr<Base>, DeserializeError> Serializable<std::shared_ptr<Base>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::size_t offset = 0;
		return Serialize::DeserializeItem(data, offset);
	}
}
