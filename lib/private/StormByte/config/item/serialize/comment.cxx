#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

#include <format>

namespace StormByte {
	using namespace StormByte::Config::Item;

	// ---------------------------------------------------------------------
	// SingleLineBash
	// ---------------------------------------------------------------------
	template<> STORMBYTE_CONFIG_PRIVATE
	std::size_t Serializable<Comment<CommentType::SingleLineBash>>::SizeComplex(const Comment<CommentType::SingleLineBash>& data) noexcept {
		return
			Serializable<Base>::Size(data) +
			Serializable<CommentType>::Size(data.CommentType()) +
			Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	std::vector<std::byte> Serializable<Comment<CommentType::SingleLineBash>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer;
		buffer.reserve(SizeComplex(m_data));
		append_vector(buffer, Serializable<Base>(m_data).Serialize());
		append_vector(buffer, Serializable<CommentType>(m_data.CommentType()).Serialize());
		append_vector(buffer, Serializable<std::string>(*m_data).Serialize());
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	Expected<Comment<CommentType::SingleLineBash>, DeserializeError> Serializable<Comment<CommentType::SingleLineBash>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, StormByte::Config::Item::Type::Comment);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment type");

		auto expected_comment_type = Serializable<CommentType>::Deserialize(data.subspan(offset));
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

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment string");

		auto expected_data = Serializable<std::string>::Deserialize(data.subspan(offset));
		if (!expected_data) return Unexpected(expected_data.error());

		Comment<CommentType::SingleLineBash> value(std::move(expected_data.value()));
		if (name.has_value())
			value.Name(std::move(name.value()));
		return value;
	}

	// ---------------------------------------------------------------------
	// SingleLineC
	// ---------------------------------------------------------------------
	template<> STORMBYTE_CONFIG_PRIVATE
	std::size_t Serializable<Comment<CommentType::SingleLineC>>::SizeComplex(const Comment<CommentType::SingleLineC>& data) noexcept {
		return
			Serializable<Base>::Size(data) +
			Serializable<CommentType>::Size(data.CommentType()) +
			Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	std::vector<std::byte> Serializable<Comment<CommentType::SingleLineC>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer;
		buffer.reserve(SizeComplex(m_data));
		append_vector(buffer, Serializable<Base>(m_data).Serialize());
		append_vector(buffer, Serializable<CommentType>(m_data.CommentType()).Serialize());
		append_vector(buffer, Serializable<std::string>(*m_data).Serialize());
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	Expected<Comment<CommentType::SingleLineC>, DeserializeError> Serializable<Comment<CommentType::SingleLineC>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, StormByte::Config::Item::Type::Comment);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment type");

		auto expected_comment_type = Serializable<CommentType>::Deserialize(data.subspan(offset));
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

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment string");

		auto expected_data = Serializable<std::string>::Deserialize(data.subspan(offset));
		if (!expected_data) return Unexpected(expected_data.error());

		Comment<CommentType::SingleLineC> value(std::move(expected_data.value()));
		if (name.has_value())
			value.Name(std::move(name.value()));
		return value;
	}

	// ---------------------------------------------------------------------
	// MultiLineC
	// ---------------------------------------------------------------------
	template<> STORMBYTE_CONFIG_PRIVATE
	std::size_t Serializable<Comment<CommentType::MultiLineC>>::SizeComplex(const Comment<CommentType::MultiLineC>& data) noexcept {
		return
			Serializable<Base>::Size(data) +
			Serializable<CommentType>::Size(data.CommentType()) +
			Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	std::vector<std::byte> Serializable<Comment<CommentType::MultiLineC>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer;
		buffer.reserve(SizeComplex(m_data));
		append_vector(buffer, Serializable<Base>(m_data).Serialize());
		append_vector(buffer, Serializable<CommentType>(m_data.CommentType()).Serialize());
		append_vector(buffer, Serializable<std::string>(*m_data).Serialize());
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	Expected<Comment<CommentType::MultiLineC>, DeserializeError> Serializable<Comment<CommentType::MultiLineC>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, StormByte::Config::Item::Type::Comment);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment type");

		auto expected_comment_type = Serializable<CommentType>::Deserialize(data.subspan(offset));
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

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for comment string");

		auto expected_data = Serializable<std::string>::Deserialize(data.subspan(offset));
		if (!expected_data) return Unexpected(expected_data.error());

		Comment<CommentType::MultiLineC> value(std::move(expected_data.value()));
		if (name.has_value())
			value.Name(std::move(name.value()));
		return value;
	}
}
