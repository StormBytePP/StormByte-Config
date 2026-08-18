#include <StormByte/config/item/value.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

namespace StormByte {
	using namespace StormByte::Config::Item;

	// ---------------------------------------------------------------------
	// Value<std::string>
	// ---------------------------------------------------------------------
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Value<std::string>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Base>(m_data).Serialize();
		std::vector<std::byte> value_data = Serializable<std::string>(*m_data).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(value_data.begin()), std::make_move_iterator(value_data.end()));
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Value<std::string>>::SizeComplex(const Value<std::string>& data) noexcept {
		return Serializable<Base>::Size(data) + Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Value<std::string>, DeserializeError> Serializable<Value<std::string>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, StormByte::Config::Item::Type::String);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for string value");

		auto expected_data = Serializable<std::string>::Deserialize(data.subspan(offset));
		if (!expected_data) return Unexpected(expected_data.error());

		Value<std::string> value(std::move(expected_data.value()));
		if (name.has_value())
			value.Name(std::move(name.value()));

		return value;
	}

	// ---------------------------------------------------------------------
	// Value<int>
	// ---------------------------------------------------------------------
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Value<int>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Base>(m_data).Serialize();
		std::vector<std::byte> value_data = Serializable<int>(*m_data).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(value_data.begin()), std::make_move_iterator(value_data.end()));
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Value<int>>::SizeComplex(const Value<int>& data) noexcept {
		return Serializable<Base>::Size(data) + Serializable<int>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Value<int>, DeserializeError> Serializable<Value<int>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, StormByte::Config::Item::Type::Integer);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for int value");

		auto expected_data = Serializable<int>::Deserialize(data.subspan(offset));
		if (!expected_data) return Unexpected(expected_data.error());

		Value<int> value(expected_data.value());
		if (name.has_value())
			value.Name(std::move(name.value()));

		return value;
	}

	// ---------------------------------------------------------------------
	// Value<double>
	// ---------------------------------------------------------------------
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Value<double>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Base>(m_data).Serialize();
		std::vector<std::byte> value_data = Serializable<double>(*m_data).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(value_data.begin()), std::make_move_iterator(value_data.end()));
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Value<double>>::SizeComplex(const Value<double>& data) noexcept {
		return Serializable<Base>::Size(data) + Serializable<double>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Value<double>, DeserializeError> Serializable<Value<double>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, StormByte::Config::Item::Type::Double);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for double value");

		auto expected_data = Serializable<double>::Deserialize(data.subspan(offset));
		if (!expected_data) return Unexpected(expected_data.error());

		Value<double> value(expected_data.value());
		if (name.has_value())
			value.Name(std::move(name.value()));

		return value;
	}

	// ---------------------------------------------------------------------
	// Value<bool>
	// ---------------------------------------------------------------------
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Value<bool>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Base>(m_data).Serialize();
		std::vector<std::byte> value_data = Serializable<bool>(*m_data).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(value_data.begin()), std::make_move_iterator(value_data.end()));
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Value<bool>>::SizeComplex(const Value<bool>& data) noexcept {
		return Serializable<Base>::Size(data) + Serializable<bool>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Value<bool>, DeserializeError> Serializable<Value<bool>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, StormByte::Config::Item::Type::Bool);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for bool value");

		auto expected_data = Serializable<bool>::Deserialize(data.subspan(offset));
		if (!expected_data) return Unexpected(expected_data.error());

		Value<bool> value(expected_data.value());
		if (name.has_value())
			value.Name(std::move(name.value()));

		return value;
	}

	// ---------------------------------------------------------------------
	// Value<std::vector<std::byte>>  (Binary)
	// ---------------------------------------------------------------------
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Value<std::vector<std::byte>>>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Base>(m_data).Serialize();
		std::vector<std::byte> value_data = Serializable<std::vector<std::byte>>(*m_data).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(value_data.begin()), std::make_move_iterator(value_data.end()));
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Value<std::vector<std::byte>>>::SizeComplex(const Value<std::vector<std::byte>>& data) noexcept {
		return Serializable<Base>::Size(data) + Serializable<std::vector<std::byte>>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Value<std::vector<std::byte>>, DeserializeError>
	Serializable<Value<std::vector<std::byte>>>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::size_t offset = 0;
		auto expected_base = Serialize::DeserializeBasicData(data, offset, StormByte::Config::Item::Type::Binary);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		if (offset >= data.size())
			return Unexpected<DeserializeError>("Insufficient data for binary value");

		auto expected_data = Serializable<std::vector<std::byte>>::Deserialize(data.subspan(offset));
		if (!expected_data) return Unexpected(expected_data.error());

		Value<std::vector<std::byte>> value(std::move(expected_data.value()));
		if (name.has_value())
			value.Name(std::move(name.value()));

		return value;
	}
}