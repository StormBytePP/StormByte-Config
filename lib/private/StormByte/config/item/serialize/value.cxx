#include <StormByte/config/item/value.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/util/serializable.hxx>
#include <iostream>
namespace StormByte::Util {
	using namespace StormByte::Config::Item;

	// Value<std::string>
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer Serializable<Value<std::string>>::SerializeComplex() const noexcept {
		return Serializable<Base>(m_data).Serialize() << Serializable<std::string>(*m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Value<std::string>>::SizeComplex(const Value<std::string>& data) noexcept {
		return Serializable<Base>::Size(data) + Serializable<std::string>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Value<std::string>, BufferOverflow> Serializable<Value<std::string>>::DeserializeComplex(const Buffer& buffer) noexcept {
		// Base data
		auto expected_base = Serialize::DeserializeBasicData(buffer, Type::String);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the string data
		auto expected_data = Serializable<std::string>::Deserialize(buffer);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Value
		Value<std::string> value(expected_data.value());
		if (name.has_value())
			value.Name(name.value());
			
		return value;
	}

	// Value<int>
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer Serializable<Value<int>>::SerializeComplex() const noexcept {
		return Serializable<Base>(m_data).Serialize() << Serializable<int>(*m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Value<int>>::SizeComplex(const Value<int>& data) noexcept {
		return Serializable<Base>::Size(data) + Serializable<int>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Value<int>, BufferOverflow> Serializable<Value<int>>::DeserializeComplex(const Buffer& buffer) noexcept {
		// Base data
		auto expected_base = Serialize::DeserializeBasicData(buffer, Type::Integer);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the int data
		auto expected_data = Serializable<int>::Deserialize(buffer);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Value
		Value<int> value(expected_data.value());
		if (name.has_value())
			value.Name(name.value());
		return value;
	}

	// Value<double>
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer Serializable<Value<double>>::SerializeComplex() const noexcept {
		return Serializable<Base>(m_data).Serialize() << Serializable<double>(*m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Value<double>>::SizeComplex(const Value<double>& data) noexcept {
		return Serializable<Base>::Size(data) + Serializable<double>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Value<double>, BufferOverflow> Serializable<Value<double>>::DeserializeComplex(const Buffer& buffer) noexcept {
		// Base data
		auto expected_base = Serialize::DeserializeBasicData(buffer, Type::Double);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the double data
		auto expected_data = Serializable<double>::Deserialize(buffer);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Value
		Value<double> value(expected_data.value());
		if (name.has_value())
			value.Name(name.value());
		return value;
	}

	// Value<bool>
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer Serializable<Value<bool>>::SerializeComplex() const noexcept {
		return Serializable<Base>(m_data).Serialize() << Serializable<bool>(*m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Value<bool>>::SizeComplex(const Value<bool>& data) noexcept {
		return Serializable<Base>::Size(data) + Serializable<bool>::Size(*data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	Expected<Value<bool>, BufferOverflow> Serializable<Value<bool>>::DeserializeComplex(const Buffer& buffer) noexcept {
		// Base data
		auto expected_base = Serialize::DeserializeBasicData(buffer, Type::Bool);
		if (!expected_base) return Unexpected(expected_base.error());
		auto [type, name] = expected_base.value();

		// Now the double data
		auto expected_data = Serializable<bool>::Deserialize(buffer);
		if (!expected_data) return Unexpected(expected_data.error());
		
		// Create the Value
		Value<bool> value(expected_data.value());
		if (name.has_value())
			value.Name(name.value());
		return value;
	}
}