#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

#include <format>

namespace StormByte {
	using namespace StormByte::Config::Item;
	
	// Single Line Bash
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Comment<CommentType::SingleLineBash>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Base>(m_data).Serialize();
		std::vector<std::byte> comment_type_data = Serializable<CommentType>(m_data.CommentType()).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(comment_type_data.begin()), std::make_move_iterator(comment_type_data.end()));
		std::vector<std::byte> string_data = Serializable<std::string>(*m_data).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(string_data.begin()), std::make_move_iterator(string_data.end()));
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Comment<CommentType::SingleLineBash>>::SizeComplex(const Comment<CommentType::SingleLineBash>& data) noexcept {
		return
			Serializable<Base>::Size(data) +
			Serializable<CommentType>::Size(data.CommentType()) +
			Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Comment<CommentType::SingleLineBash>, DeserializeError> Serializable<Comment<CommentType::SingleLineBash>>::DeserializeComplex(const std::vector<std::byte>& data) noexcept {
		// Base data
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, Type::Comment);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the comment type
		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment type");
		
		std::vector<std::byte> comment_type_data(data.begin() + offset, data.end());
		auto expected_comment_type = Serializable<CommentType>::Deserialize(comment_type_data);
		if (!expected_comment_type) return Unexpected(expected_comment_type.error());
		offset += Serializable<CommentType>::Size(expected_comment_type.value());
		
		if (expected_comment_type.value() != CommentType::SingleLineBash) {
			return Unexpected<DeserializeError>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(CommentType::SingleLineBash),
					TypeToString(expected_comment_type.value())
				)
			);
		}

		// Now the string data
		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment string");
		
		std::vector<std::byte> string_data(data.begin() + offset, data.end());
		auto expected_data = Serializable<std::string>::Deserialize(string_data);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Comment
		Comment<CommentType::SingleLineBash> value(std::move(expected_data.value()));
		return value;
	}

	// Single Line C
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Comment<CommentType::SingleLineC>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Base>(m_data).Serialize();
		std::vector<std::byte> comment_type_data = Serializable<CommentType>(m_data.CommentType()).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(comment_type_data.begin()), std::make_move_iterator(comment_type_data.end()));
		std::vector<std::byte> string_data = Serializable<std::string>(*m_data).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(string_data.begin()), std::make_move_iterator(string_data.end()));
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Comment<CommentType::SingleLineC>>::SizeComplex(const Comment<CommentType::SingleLineC>& data) noexcept {
		return
			Serializable<Base>::Size(data) +
			Serializable<CommentType>::Size(data.CommentType()) +
			Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Comment<CommentType::SingleLineC>, DeserializeError> Serializable<Comment<CommentType::SingleLineC>>::DeserializeComplex(const std::vector<std::byte>& data) noexcept {
		// Base data
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, Type::Comment);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the comment type
		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment type");
		
		std::vector<std::byte> comment_type_data(data.begin() + offset, data.end());
		auto expected_comment_type = Serializable<CommentType>::Deserialize(comment_type_data);
		if (!expected_comment_type) return Unexpected(expected_comment_type.error());
		offset += Serializable<CommentType>::Size(expected_comment_type.value());
		
		if (expected_comment_type.value() != CommentType::SingleLineC) {
			return Unexpected<DeserializeError>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(CommentType::SingleLineC),
					TypeToString(expected_comment_type.value())
				)
			);
		}

		// Now the string data
		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment string");
		
		std::vector<std::byte> string_data(data.begin() + offset, data.end());
		auto expected_data = Serializable<std::string>::Deserialize(string_data);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Comment
		Comment<CommentType::SingleLineC> value(std::move(expected_data.value()));
		return value;
	}

	// Multi Line C
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Comment<CommentType::MultiLineC>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Base>(m_data).Serialize();
		std::vector<std::byte> comment_type_data = Serializable<CommentType>(m_data.CommentType()).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(comment_type_data.begin()), std::make_move_iterator(comment_type_data.end()));
		std::vector<std::byte> string_data = Serializable<std::string>(*m_data).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(string_data.begin()), std::make_move_iterator(string_data.end()));
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Comment<CommentType::MultiLineC>>::SizeComplex(const Comment<CommentType::MultiLineC>& data) noexcept {
		return
			Serializable<Base>::Size(data) +
			Serializable<CommentType>::Size(data.CommentType()) +
			Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Comment<CommentType::MultiLineC>, DeserializeError> Serializable<Comment<CommentType::MultiLineC>>::DeserializeComplex(const std::vector<std::byte>& data) noexcept {
		// Base data
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, Type::Comment);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the comment type
		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment type");
		
		std::vector<std::byte> comment_type_data(data.begin() + offset, data.end());
		auto expected_comment_type = Serializable<CommentType>::Deserialize(comment_type_data);
		if (!expected_comment_type) return Unexpected(expected_comment_type.error());
		offset += Serializable<CommentType>::Size(expected_comment_type.value());
		
		if (expected_comment_type.value() != CommentType::MultiLineC) {
			return Unexpected<DeserializeError>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(CommentType::MultiLineC),
					TypeToString(expected_comment_type.value())
				)
			);
		}

		// Now the string data
		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment string");
		
		std::vector<std::byte> string_data(data.begin() + offset, data.end());
		auto expected_data = Serializable<std::string>::Deserialize(string_data);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Comment
		Comment<CommentType::MultiLineC> value(std::move(expected_data.value()));
		return value;
	}
}