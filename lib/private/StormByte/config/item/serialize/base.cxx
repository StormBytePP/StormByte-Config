#include <StormByte/config/item/value.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

#include <format>
#include <memory>
#include <iostream>
namespace StormByte::Config::Item::Serialize {
	StormByte::Expected<BaseData, Buffers::BufferOverflow> DeserializeBasicData(const Buffers::Simple& buffer) noexcept {
		// Base data (can't call it directly as base is pure virtual)
		auto expected_type = Serializable<StormByte::Config::Item::Type>::Deserialize(buffer);
		if (!expected_type) return Unexpected(expected_type.error());
		
		auto expected_name = Serializable<std::optional<std::string>>::Deserialize(buffer);
		if (!expected_name) return Unexpected(expected_name.error());
		
		return std::make_pair<Type, std::optional<std::string>>(std::move(expected_type.value()), std::move(expected_name.value()));
	}

	StormByte::Expected<BaseData, Buffers::BufferOverflow> DeserializeBasicData(const Buffers::Simple& buffer, const Type& expected_item_type) noexcept {
		auto expected_base_data = DeserializeBasicData(buffer);
		if (!expected_base_data) return Unexpected(expected_base_data.error());
		if (expected_base_data.value().first != expected_item_type) {
			return Unexpected<Buffers::BufferOverflow>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(expected_item_type),
					TypeToString(expected_base_data.value().first)
				)
			);
		}
		
		return expected_base_data.value();
	}

	StormByte::Expected<std::shared_ptr<Container>, Buffers::BufferOverflow> DeserializeContainer(const Buffers::Simple& buffer, std::shared_ptr<Container> container) noexcept {
		// Base data
		auto expected_base = Serialize::DeserializeBasicData(buffer, Type::Container);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();
		if (type != Type::Container) {
			return Unexpected<Buffers::BufferOverflow>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(container->ContainerType()),
					TypeToString(type)
				)
			);
		}
		
		if (name.has_value())
			container->Name(name.value());
		
		auto container_type = Serializable<ContainerType>::Deserialize(buffer);
		if (!container_type) return Unexpected(container_type.error());
		if (container_type.value() != container->ContainerType()) {
			return Unexpected<Buffers::BufferOverflow>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(container->ContainerType()),
					TypeToString(container_type.value())
				)
			);
		}
		auto items_size = Serializable<std::size_t>::Deserialize(buffer);
		if (!items_size) return Unexpected(items_size.error());
		if (items_size.value() == 0) return container;

		for (size_t i = 0; i < items_size.value(); i++) {
			// We get basic data to know who to delegate the deserialization to
			// Do not look only at the type because Containers and Comments need to look its type afterwards
			std::size_t pos_before_basic_data = buffer.Position();
			auto expected_base_data = Serialize::DeserializeBasicData(buffer);
			if (!expected_base_data) return Unexpected(expected_base_data.error());
			// We need to restore offset and store current position before to be able to look for Container or Comment type
			std::size_t pos_after_basic_data = buffer.Position();
			buffer.Seek(pos_before_basic_data, Buffers::Read::Position::Absolute);
			switch(expected_base_data.value().first) {
				case Type::String:
					{
						auto expected_item = Serializable<Value<std::string>>::Deserialize(buffer);
						if (!expected_item) return Unexpected(expected_item.error());
						container->Add(std::move(expected_item.value()));
					}
					break;
				case Type::Integer:
					{
						auto expected_item = Serializable<Value<int>>::Deserialize(buffer);
						if (!expected_item) return Unexpected(expected_item.error());
						container->Add(std::move(expected_item.value()));
					}
					break;
				case Type::Double:
					{
						auto expected_item = Serializable<Value<double>>::Deserialize(buffer);
						if (!expected_item) return Unexpected(expected_item.error());
						container->Add(std::move(expected_item.value()));
					}
					break;
				case Type::Comment:
					{
						buffer.Seek(pos_after_basic_data, Buffers::Read::Position::Absolute);
						auto expected_comment_type = Serializable<CommentType>::Deserialize(buffer);
						if (!expected_comment_type) return Unexpected(expected_comment_type.error());
						// We restore the offset
						buffer.Seek(pos_before_basic_data, Buffers::Read::Position::Absolute);
						switch(expected_comment_type.value()) {
							case CommentType::SingleLineBash:
								{
									auto expected_item = Serializable<Comment<CommentType::SingleLineBash>>::Deserialize(buffer);
									if (!expected_item) return Unexpected(expected_item.error());
									container->Add(expected_item.value());
								}
								break;
							case CommentType::SingleLineC:
								{
									auto expected_item = Serializable<Comment<CommentType::SingleLineC>>::Deserialize(buffer);
									if (!expected_item) return Unexpected(expected_item.error());
									container->Add(std::move(expected_item.value()));
								}
								break;
							case CommentType::MultiLineC:
								{
									auto expected_item = Serializable<Comment<CommentType::MultiLineC>>::Deserialize(buffer);
									if (!expected_item) return Unexpected(expected_item.error());
									container->Add(std::move(expected_item.value()));
								}
								break;
						}
					}
					break;
				case Type::Bool:
					{
						auto expected_item = Serializable<Value<bool>>::Deserialize(buffer);
						if (!expected_item) return Unexpected(expected_item.error());
						container->Add(std::move(expected_item.value()));
					}
					break;
				case Type::Container:
					{
						buffer.Seek(pos_after_basic_data, Buffers::Read::Position::Absolute);
						auto expected_container_type = Serializable<ContainerType>::Deserialize(buffer);
						if (!expected_container_type) return Unexpected(expected_container_type.error());
						// We restore the offset
						buffer.Seek(pos_before_basic_data, Buffers::Read::Position::Absolute);
						switch (expected_container_type.value()) {
							case ContainerType::Group:
								{
									auto expected_item = Serializable<Group>::Deserialize(buffer);
									if (!expected_item) return Unexpected(expected_item.error());
									container->Add(std::move(expected_item.value()));
								}
								break;
							case ContainerType::List:
								{
									auto expected_item = Serializable<List>::Deserialize(buffer);
									if (!expected_item) return Unexpected(expected_item.error());
									container->Add(std::move(expected_item.value()));
								}
								break;
						}
					}
					break;
				default:
					return StormByte::Unexpected<Buffers::BufferOverflow>("Unhandled item type"); // This should not happen
			}
		}
		return container;
	}
}

namespace StormByte {
	using namespace StormByte::Config::Item;

	// Base
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffers::Simple Serializable<Base>::SerializeComplex() const noexcept {
		return Serializable<Type>(m_data.Type()).Serialize() << Serializable<std::optional<std::string>>(m_data.Name()).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Base>::SizeComplex(const Base& data) noexcept {
		return Serializable<Type>::Size(data.Type()) + Serializable<std::optional<std::string>>::Size(data.Name());
	}

	// std::shared_ptr<Base>
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffers::Simple Serializable<std::shared_ptr<Base>>::SerializeComplex() const noexcept {
		Buffers::Simple buffer; // Every item will serialize its base data
		switch(m_data->Type()) {
			case Type::String:
				buffer << Serializable<Value<std::string>>(static_cast<Value<std::string>&>(*m_data)).Serialize();
				break;
			case Type::Integer:
				buffer << Serializable<Value<int>>(static_cast<Value<int>&>(*m_data)).Serialize();
				break;
			case Type::Double:
				buffer << Serializable<Value<double>>(static_cast<Value<double>&>(*m_data)).Serialize();
				break;
			case Type::Comment:
				if (std::dynamic_pointer_cast<Comment<CommentType::SingleLineBash>>(m_data)) {
					buffer << Serializable<Comment<CommentType::SingleLineBash>>(static_cast<Comment<CommentType::SingleLineBash>&>(*m_data)).Serialize();
				} else if (std::dynamic_pointer_cast<Comment<CommentType::SingleLineC>>(m_data)) {
					buffer << Serializable<Comment<CommentType::SingleLineC>>(static_cast<Comment<CommentType::SingleLineC>&>(*m_data)).Serialize();
				} else if (std::dynamic_pointer_cast<Comment<CommentType::MultiLineC>>(m_data)) {
					buffer << Serializable<Comment<CommentType::MultiLineC>>(static_cast<Comment<CommentType::MultiLineC>&>(*m_data)).Serialize();
				}
				break;
			case Type::Bool:
				buffer << Serializable<Value<bool>>(static_cast<Value<bool>&>(*m_data)).Serialize();
				break;
			case Type::Container:
				buffer << Serializable<Container>(static_cast<Container&>(*m_data)).Serialize();
				break;
		}
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<std::shared_ptr<Base>>::SizeComplex(const std::shared_ptr<Base>& data) noexcept {
		// Each items already include its base size
		switch(data->Type()) {
			case Type::String:
				return Serializable<Value<std::string>>::Size(static_cast<Value<std::string>&>(*data));
			case Type::Integer:
				return Serializable<Value<int>>::Size(static_cast<Value<int>&>(*data));
			case Type::Double:
				return Serializable<Value<double>>::Size(static_cast<Value<double>&>(*data));
			case Type::Comment:
				if (std::dynamic_pointer_cast<Comment<CommentType::SingleLineBash>>(data)) {
					return Serializable<Comment<CommentType::SingleLineBash>>::Size(static_cast<Comment<CommentType::SingleLineBash>&>(*data));
				} else if (std::dynamic_pointer_cast<Comment<CommentType::SingleLineC>>(data)) {
					return Serializable<Comment<CommentType::SingleLineC>>::Size(static_cast<Comment<CommentType::SingleLineC>&>(*data));
				} else if (std::dynamic_pointer_cast<Comment<CommentType::MultiLineC>>(data)) {
					return Serializable<Comment<CommentType::MultiLineC>>::Size(static_cast<Comment<CommentType::MultiLineC>&>(*data));
				}
				break; // This should not happen but if I add a comment type without putting it here, it will break
			case Type::Bool:
				return Serializable<Value<bool>>::Size(static_cast<Value<bool>&>(*data));
			case Type::Container:
				return Serializable<Container>::Size(static_cast<Container&>(*data));
		}
		return 0; // For unhandled cases
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	StormByte::Expected<std::shared_ptr<Base>, Buffers::BufferOverflow> Serializable<std::shared_ptr<Base>>::DeserializeComplex(const Buffers::Simple& buffer) noexcept {
		// We need to look at the item type which is in base data
		// Do not look only at the type because Containers and Comments need to look its type afterwards
		std::size_t pos_before_basic_data = buffer.Position();
		auto expected_base_data = Serialize::DeserializeBasicData(buffer);
		if (!expected_base_data) return Unexpected(expected_base_data.error());
		// We need to restore offset and store current position before to be able to look for Container or Comment type
		std::size_t pos_after_basic_data = buffer.Position();
		buffer.Seek(pos_before_basic_data, Buffers::Read::Position::Absolute);
		std::shared_ptr<Base> item_ptr;
		switch(expected_base_data.value().first) {
			case Type::String:
				{
					auto expected_item = Serializable<Value<std::string>>::Deserialize(buffer);
					if (!expected_item) return Unexpected(expected_item.error());
					item_ptr = std::make_shared<Value<std::string>>(std::move(expected_item.value()));
				}
				break;
			case Type::Integer:
				{
					auto expected_item = Serializable<Value<int>>::Deserialize(buffer);
					if (!expected_item) return Unexpected(expected_item.error());
					item_ptr = std::make_shared<Value<int>>(std::move(expected_item.value()));
				}
				break;
			case Type::Double:
				{
					auto expected_item = Serializable<Value<double>>::Deserialize(buffer);
					if (!expected_item) return Unexpected(expected_item.error());
					item_ptr = std::make_shared<Value<double>>(std::move(expected_item.value()));
				}
				break;
			case Type::Comment:
				{
					// We place ourselves at after item type pos to get the comment type
					buffer.Seek(pos_after_basic_data, Buffers::Read::Position::Absolute);
					auto expected_comment_type = Serializable<CommentType>::Deserialize(buffer);
					if (!expected_comment_type) return Unexpected(expected_comment_type.error());
					// We need to restore offset
					buffer.Seek(pos_before_basic_data, Buffers::Read::Position::Absolute);
					switch(expected_comment_type.value()) {
						case CommentType::SingleLineBash:
							{
								auto expected_item = Serializable<Comment<CommentType::SingleLineBash>>::Deserialize(buffer);
								if (!expected_item) return Unexpected(expected_item.error());
								item_ptr = std::make_shared<Comment<CommentType::SingleLineBash>>(std::move(expected_item.value()));
							}
							break;
						case CommentType::SingleLineC:
							{
								auto expected_item = Serializable<Comment<CommentType::SingleLineC>>::Deserialize(buffer);
								if (!expected_item) return Unexpected(expected_item.error());
								item_ptr = std::make_shared<Comment<CommentType::SingleLineC>>(std::move(expected_item.value()));
							}
							break;
						case CommentType::MultiLineC:
							{
								auto expected_item = Serializable<Comment<CommentType::MultiLineC>>::Deserialize(buffer);
								if (!expected_item) return Unexpected(expected_item.error());
								item_ptr = std::make_shared<Comment<CommentType::MultiLineC>>(std::move(expected_item.value()));
							}
							break;
					}
				}
				break;
			case Type::Bool:
				{
					auto expected_item = Serializable<Value<bool>>::Deserialize(buffer);
					if (!expected_item) return Unexpected(expected_item.error());
					item_ptr = std::make_shared<Value<bool>>(std::move(expected_item.value()));
				}
				break;
			case Type::Container:
				{
					// We place ourselves at after item type pos to get the container type
					buffer.Seek(pos_after_basic_data, Buffers::Read::Position::Absolute);
					auto expected_container_type = Serializable<ContainerType>::Deserialize(buffer);
					if (!expected_container_type) return Unexpected(expected_container_type.error());
					// We need to restore offset
					buffer.Seek(pos_before_basic_data, Buffers::Read::Position::Absolute);
					switch(expected_container_type.value()) {
						case ContainerType::Group:
							{
								auto expected_item = Serializable<Group>::Deserialize(buffer);
								if (!expected_item) return Unexpected(expected_item.error());
								item_ptr = std::make_shared<Group>(std::move(expected_item.value()));
							}
							break;
						case ContainerType::List:
							{
								auto expected_item = Serializable<List>::Deserialize(buffer);
								if (!expected_item) return Unexpected(expected_item.error());
								item_ptr = std::make_shared<List>(std::move(expected_item.value()));
							}
							break;
					}
				}
				break;
		}
		if (item_ptr) {
			if (expected_base_data.value().second.has_value())
				item_ptr->Name(expected_base_data.value().second.value());
			return item_ptr;
		}
		else
			return StormByte::Unexpected<Buffers::BufferOverflow>("Unknown error with item creation"); // This should not happen
	}
}