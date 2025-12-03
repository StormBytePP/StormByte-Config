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
	StormByte::Expected<BaseData, DeserializeError> DeserializeBasicData(std::span<const std::byte> data, std::size_t& offset) noexcept {
		// Base data (can't call it directly as base is pure virtual)
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
		
		return std::make_pair<Type, std::optional<std::string>>(std::move(expected_type.value()), std::move(expected_name.value()));
	}

	StormByte::Expected<BaseData, DeserializeError> DeserializeBasicData(std::span<const std::byte> data, std::size_t& offset, const Type& expected_item_type) noexcept {
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

	StormByte::Expected<std::shared_ptr<Container>, DeserializeError> DeserializeContainer(std::span<const std::byte> data, std::size_t& offset, std::shared_ptr<Container> container) noexcept {
		// Base data
		auto expected_base = Serialize::DeserializeBasicData(data, offset, Type::Container);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();
		if (type != Type::Container) {
			return Unexpected<DeserializeError>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(container->ContainerType()),
					TypeToString(type)
				)
			);
		}
		
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
		
		auto items_size = Serializable<std::size_t>::Deserialize(data.subspan(offset));
		if (!items_size) return Unexpected(items_size.error());
		offset += Serializable<std::size_t>::Size(items_size.value());
		
		if (items_size.value() == 0) return container;

		for (size_t i = 0; i < items_size.value(); i++) {
			// We get basic data to know who to delegate the deserialization to
			// Do not look only at the type because Containers and Comments need to look its type afterwards
			std::size_t temp_offset = offset;
			auto expected_base_data = Serialize::DeserializeBasicData(data, temp_offset);
			if (!expected_base_data) return Unexpected(expected_base_data.error());
			// We need to store current position after to be able to look for Container or Comment type
			std::size_t pos_after_basic_data = temp_offset;
			
			switch(expected_base_data.value().first) {
				case Type::String:
					{
						auto expected_item = Serializable<Value<std::string>>::Deserialize(data.subspan(offset));
						if (!expected_item) return Unexpected(expected_item.error());
						offset += Serializable<Value<std::string>>::Size(expected_item.value());
						container->Add(std::move(expected_item.value()));
					}
					break;
				case Type::Integer:
					{
						auto expected_item = Serializable<Value<int>>::Deserialize(data.subspan(offset));
						if (!expected_item) return Unexpected(expected_item.error());
						offset += Serializable<Value<int>>::Size(expected_item.value());
						container->Add(std::move(expected_item.value()));
					}
					break;
				case Type::Double:
					{
						auto expected_item = Serializable<Value<double>>::Deserialize(data.subspan(offset));
						if (!expected_item) return Unexpected(expected_item.error());
						offset += Serializable<Value<double>>::Size(expected_item.value());
						container->Add(std::move(expected_item.value()));
					}
					break;
				case Type::Comment:
					{
						if (pos_after_basic_data >= data.size())
							return Unexpected<DeserializeError>("Insufficient data for comment type");
						
						std::vector<std::byte> comment_type_data(data.begin() + pos_after_basic_data, data.end());
						auto expected_comment_type = Serializable<CommentType>::Deserialize(comment_type_data);
						if (!expected_comment_type) return Unexpected(expected_comment_type.error());
						
						switch(expected_comment_type.value()) {
							case CommentType::SingleLineBash:
								{
									auto expected_item = Serializable<Comment<CommentType::SingleLineBash>>::Deserialize(data.subspan(offset));
									if (!expected_item) return Unexpected(expected_item.error());
									offset += Serializable<Comment<CommentType::SingleLineBash>>::Size(expected_item.value());
									container->Add(expected_item.value());
								}
								break;
							case CommentType::SingleLineC:
								{
									auto expected_item = Serializable<Comment<CommentType::SingleLineC>>::Deserialize(data.subspan(offset));
									if (!expected_item) return Unexpected(expected_item.error());
									offset += Serializable<Comment<CommentType::SingleLineC>>::Size(expected_item.value());
									container->Add(std::move(expected_item.value()));
								}
								break;
							case CommentType::MultiLineC:
								{
									auto expected_item = Serializable<Comment<CommentType::MultiLineC>>::Deserialize(data.subspan(offset));
									if (!expected_item) return Unexpected(expected_item.error());
									offset += Serializable<Comment<CommentType::MultiLineC>>::Size(expected_item.value());
									container->Add(std::move(expected_item.value()));
								}
								break;
						}
					}
					break;
				case Type::Bool:
					{
						auto expected_item = Serializable<Value<bool>>::Deserialize(data.subspan(offset));
						if (!expected_item) return Unexpected(expected_item.error());
						offset += Serializable<Value<bool>>::Size(expected_item.value());
						container->Add(std::move(expected_item.value()));
					}
					break;
				case Type::Container:
					{
						if (pos_after_basic_data >= data.size())
							return Unexpected<DeserializeError>("Insufficient data for container type");
						
						std::vector<std::byte> container_type_data(data.begin() + pos_after_basic_data, data.end());
						auto expected_container_type = Serializable<ContainerType>::Deserialize(container_type_data);
						if (!expected_container_type) return Unexpected(expected_container_type.error());
						
					switch (expected_container_type.value()) {
						case ContainerType::Group:
							{
								auto expected_item = Serializable<Group>::Deserialize(data.subspan(offset));
									if (!expected_item) return Unexpected(expected_item.error());
									offset += Serializable<Group>::Size(expected_item.value());
									container->Add(std::move(expected_item.value()));
								}
							break;
						case ContainerType::List:
							{
								auto expected_item = Serializable<List>::Deserialize(data.subspan(offset));
									if (!expected_item) return Unexpected(expected_item.error());
									offset += Serializable<List>::Size(expected_item.value());
									container->Add(std::move(expected_item.value()));
								}
								break;
						}
					}
					break;
				default:
					return StormByte::Unexpected<DeserializeError>("Unhandled item type"); // This should not happen
			}
		}
		return container;
	}
}

namespace StormByte {
	using namespace StormByte::Config::Item;

	// Base
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Base>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Type>(m_data.Type()).Serialize();
		std::vector<std::byte> name_data = Serializable<std::optional<std::string>>(m_data.Name()).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(name_data.begin()), std::make_move_iterator(name_data.end()));
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Base>::SizeComplex(const Base& data) noexcept {
		return Serializable<Type>::Size(data.Type()) + Serializable<std::optional<std::string>>::Size(data.Name());
	}

	// std::shared_ptr<Base>
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<std::shared_ptr<Base>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer; // Every item will serialize its base data
		switch(m_data->Type()) {
			case Type::String:
				buffer = Serializable<Value<std::string>>(static_cast<Value<std::string>&>(*m_data)).Serialize();
				break;
			case Type::Integer:
				buffer = Serializable<Value<int>>(static_cast<Value<int>&>(*m_data)).Serialize();
				break;
			case Type::Double:
				buffer = Serializable<Value<double>>(static_cast<Value<double>&>(*m_data)).Serialize();
				break;
			case Type::Comment:
				if (std::dynamic_pointer_cast<Comment<CommentType::SingleLineBash>>(m_data)) {
					buffer = Serializable<Comment<CommentType::SingleLineBash>>(static_cast<Comment<CommentType::SingleLineBash>&>(*m_data)).Serialize();
				} else if (std::dynamic_pointer_cast<Comment<CommentType::SingleLineC>>(m_data)) {
					buffer = Serializable<Comment<CommentType::SingleLineC>>(static_cast<Comment<CommentType::SingleLineC>&>(*m_data)).Serialize();
				} else if (std::dynamic_pointer_cast<Comment<CommentType::MultiLineC>>(m_data)) {
					buffer = Serializable<Comment<CommentType::MultiLineC>>(static_cast<Comment<CommentType::MultiLineC>&>(*m_data)).Serialize();
				}
				break;
			case Type::Bool:
				buffer = Serializable<Value<bool>>(static_cast<Value<bool>&>(*m_data)).Serialize();
				break;
			case Type::Container:
				buffer = Serializable<Container>(static_cast<Container&>(*m_data)).Serialize();
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
	StormByte::Expected<std::shared_ptr<Base>, DeserializeError> Serializable<std::shared_ptr<Base>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		// We need to look at the item type which is in base data
		// Do not look only at the type because Containers and Comments need to look its type afterwards
		std::size_t offset = 0;
		std::size_t temp_offset = offset;
		auto expected_base_data = Serialize::DeserializeBasicData(data, temp_offset);
		if (!expected_base_data) return Unexpected(expected_base_data.error());
		// We need to store current position after to be able to look for Container or Comment type
		std::size_t pos_after_basic_data = temp_offset;
		
		std::shared_ptr<Base> item_ptr;
		switch(expected_base_data.value().first) {
			case Type::String:
				{
					auto expected_item = Serializable<Value<std::string>>::Deserialize(data);
					if (!expected_item) return Unexpected(expected_item.error());
					item_ptr = std::make_shared<Value<std::string>>(std::move(expected_item.value()));
				}
				break;
			case Type::Integer:
				{
					auto expected_item = Serializable<Value<int>>::Deserialize(data);
					if (!expected_item) return Unexpected(expected_item.error());
					item_ptr = std::make_shared<Value<int>>(std::move(expected_item.value()));
				}
				break;
			case Type::Double:
				{
					auto expected_item = Serializable<Value<double>>::Deserialize(data);
					if (!expected_item) return Unexpected(expected_item.error());
					item_ptr = std::make_shared<Value<double>>(std::move(expected_item.value()));
				}
				break;
			case Type::Comment:
				{
					// We look at after item type pos to get the comment type
					if (pos_after_basic_data >= data.size())
						return Unexpected<DeserializeError>("Insufficient data for comment type");
					
					auto expected_comment_type = Serializable<CommentType>::Deserialize(data.subspan(pos_after_basic_data));
					if (!expected_comment_type) return Unexpected(expected_comment_type.error());
					
					switch(expected_comment_type.value()) {
						case CommentType::SingleLineBash:
							{
								auto expected_item = Serializable<Comment<CommentType::SingleLineBash>>::Deserialize(data);
								if (!expected_item) return Unexpected(expected_item.error());
								item_ptr = std::make_shared<Comment<CommentType::SingleLineBash>>(std::move(expected_item.value()));
							}
							break;
						case CommentType::SingleLineC:
							{
								auto expected_item = Serializable<Comment<CommentType::SingleLineC>>::Deserialize(data);
								if (!expected_item) return Unexpected(expected_item.error());
								item_ptr = std::make_shared<Comment<CommentType::SingleLineC>>(std::move(expected_item.value()));
							}
							break;
						case CommentType::MultiLineC:
							{
								auto expected_item = Serializable<Comment<CommentType::MultiLineC>>::Deserialize(data);
								if (!expected_item) return Unexpected(expected_item.error());
								item_ptr = std::make_shared<Comment<CommentType::MultiLineC>>(std::move(expected_item.value()));
							}
							break;
					}
				}
				break;
			case Type::Bool:
				{
					auto expected_item = Serializable<Value<bool>>::Deserialize(data);
					if (!expected_item) return Unexpected(expected_item.error());
					item_ptr = std::make_shared<Value<bool>>(std::move(expected_item.value()));
				}
				break;
			case Type::Container:
				{
					// We look at after item type pos to get the container type
					if (pos_after_basic_data >= data.size())
						return Unexpected<DeserializeError>("Insufficient data for container type");
					
					auto expected_container_type = Serializable<ContainerType>::Deserialize(data.subspan(pos_after_basic_data));
					if (!expected_container_type) return Unexpected(expected_container_type.error());
					
					switch(expected_container_type.value()) {
						case ContainerType::Group:
							{
								auto expected_item = Serializable<Group>::Deserialize(data);
								if (!expected_item) return Unexpected(expected_item.error());
								item_ptr = std::make_shared<Group>(std::move(expected_item.value()));
							}
							break;
						case ContainerType::List:
							{
								auto expected_item = Serializable<List>::Deserialize(data);
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
				item_ptr->Name(std::move(expected_base_data.value().second.value()));
			return item_ptr;
		}
		else
			return StormByte::Unexpected<DeserializeError>("Unknown error with item creation"); // This should not happen
	}
}