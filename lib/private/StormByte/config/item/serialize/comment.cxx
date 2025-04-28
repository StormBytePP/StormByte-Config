#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

#include <format>

namespace StormByte {
	using namespace StormByte::Config::Item;
	
	// Single Line Bash
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer::Simple Serializable<Comment<CommentType::SingleLineBash>>::SerializeComplex() const noexcept {
		return
			Serializable<Base>(m_data).Serialize() <<
			Serializable<CommentType>(m_data.CommentType()).Serialize() <<
			Serializable<std::string>(*m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Comment<CommentType::SingleLineBash>>::SizeComplex(const Comment<CommentType::SingleLineBash>& data) noexcept {
		return
			Serializable<Base>::Size(data) +
			Serializable<CommentType>::Size(data.CommentType()) +
			Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Comment<CommentType::SingleLineBash>, Buffer::BufferOverflow> Serializable<Comment<CommentType::SingleLineBash>>::DeserializeComplex(const Buffer::Simple& buffer) noexcept {
		// Base data
		auto expected_base = Serialize::DeserializeBasicData(buffer, Type::Comment);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the comment type
		auto expected_comment_type = Serializable<CommentType>::Deserialize(buffer);
		if (!expected_comment_type) return Unexpected(expected_comment_type.error());
		if (expected_comment_type.value() != CommentType::SingleLineBash) {
			return Unexpected<Buffer::BufferOverflow>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(CommentType::SingleLineBash),
					TypeToString(expected_comment_type.value())
				)
			);
		}

		// Now the string data
		auto expected_data = Serializable<std::string>::Deserialize(buffer);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Comment
		Comment<CommentType::SingleLineBash> value(expected_data.value());
		return value;
	}

	// Single Line C
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer::Simple Serializable<Comment<CommentType::SingleLineC>>::SerializeComplex() const noexcept {
		return
			Serializable<Base>(m_data).Serialize() <<
			Serializable<CommentType>(m_data.CommentType()).Serialize() <<
			Serializable<std::string>(*m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Comment<CommentType::SingleLineC>>::SizeComplex(const Comment<CommentType::SingleLineC>& data) noexcept {
		return
			Serializable<Base>::Size(data) +
			Serializable<CommentType>::Size(data.CommentType()) +
			Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Comment<CommentType::SingleLineC>, Buffer::BufferOverflow> Serializable<Comment<CommentType::SingleLineC>>::DeserializeComplex(const Buffer::Simple& buffer) noexcept {
		// Base data
		auto expected_base = Serialize::DeserializeBasicData(buffer, Type::Comment);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the comment type
		auto expected_comment_type = Serializable<CommentType>::Deserialize(buffer);
		if (!expected_comment_type) return Unexpected(expected_comment_type.error());
		if (expected_comment_type.value() != CommentType::SingleLineC) {
			return Unexpected<Buffer::BufferOverflow>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(CommentType::SingleLineC),
					TypeToString(expected_comment_type.value())
				)
			);
		}

		// Now the string data
		auto expected_data = Serializable<std::string>::Deserialize(buffer);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Comment
		Comment<CommentType::SingleLineC> value(expected_data.value());
		return value;
	}

	// Multi Line C
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer::Simple Serializable<Comment<CommentType::MultiLineC>>::SerializeComplex() const noexcept {
		return
			Serializable<Base>(m_data).Serialize() <<
			Serializable<CommentType>(m_data.CommentType()).Serialize() <<
			Serializable<std::string>(*m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Comment<CommentType::MultiLineC>>::SizeComplex(const Comment<CommentType::MultiLineC>& data) noexcept {
		return
			Serializable<Base>::Size(data) +
			Serializable<CommentType>::Size(data.CommentType()) +
			Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Comment<CommentType::MultiLineC>, Buffer::BufferOverflow> Serializable<Comment<CommentType::MultiLineC>>::DeserializeComplex(const Buffer::Simple& buffer) noexcept {
		// Base data
		auto expected_base = Serialize::DeserializeBasicData(buffer, Type::Comment);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the comment type
		auto expected_comment_type = Serializable<CommentType>::Deserialize(buffer);
		if (!expected_comment_type) return Unexpected(expected_comment_type.error());
		if (expected_comment_type.value() != CommentType::MultiLineC) {
			return Unexpected<Buffer::BufferOverflow>(
				std::format(
					"Type mismatch: Tried {} but got {}",
					TypeToString(CommentType::MultiLineC),
					TypeToString(expected_comment_type.value())
				)
			);
		}

		// Now the string data
		auto expected_data = Serializable<std::string>::Deserialize(buffer);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Comment
		Comment<CommentType::MultiLineC> value(expected_data.value());
		return value;
	}
}