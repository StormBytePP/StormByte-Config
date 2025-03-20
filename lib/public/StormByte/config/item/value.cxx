#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/config/item/value.hxx>

#include <string_view>

namespace StormByte::Config::Item {
	template<> STORMBYTE_CONFIG_PUBLIC
	std::string Value<std::string>::Serialize(const int& indent_level) const noexcept {
		return Base::Serialize(indent_level) + "\"" + m_value + "\"";
	}
	template class Value<std::string>;

	template<> STORMBYTE_CONFIG_PUBLIC
	std::string Value<int>::Serialize(const int& indent_level) const noexcept {
		return Base::Serialize(indent_level) + std::to_string(m_value);
	}
	template class Value<int>;

	template<> STORMBYTE_CONFIG_PUBLIC
	std::string Value<double>::Serialize(const int& indent_level) const noexcept {
		return Base::Serialize(indent_level) + std::to_string(m_value);
	}
	template class Value<double>;

	template<> STORMBYTE_CONFIG_PUBLIC
	std::string Value<bool>::Serialize(const int& indent_level) const noexcept {
		return Base::Serialize(indent_level) + (m_value ? "true" : "false");
	}
	template class Value<bool>;
}
