#include <StormByte/config/item/value.hxx>
#include <StormByte/base64.hxx>
#include <StormByte/string.hxx>

#include <string_view>

namespace StormByte::Config::Item {
	// ---------------------------------------------------------------------
	// std::string
	// ---------------------------------------------------------------------
	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Value<std::string>::Serialize(const int& indent_level) const noexcept {
		std::string escaped;
		escaped.reserve(m_value.size() + 8);
		for (char c : m_value) {
			switch (c) {
				case '"':  escaped += "\\\""; break;
				case '\\': escaped += "\\\\"; break;
				case '\n': escaped += "\\n";  break;
				case '\r': escaped += "\\r";  break;
				case '\t': escaped += "\\t";  break;
				default:   escaped += c;      break;
			}
		}
		return Base::Serialize(indent_level) + "\"" + escaped + "\"";
	}
	template class Value<std::string>;

	// ---------------------------------------------------------------------
	// int
	// ---------------------------------------------------------------------
	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Value<int>::Serialize(const int& indent_level) const noexcept {
		return Base::Serialize(indent_level) + std::to_string(m_value);
	}
	template class Value<int>;

	// ---------------------------------------------------------------------
	// double
	// ---------------------------------------------------------------------
	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Value<double>::Serialize(const int& indent_level) const noexcept {
		return Base::Serialize(indent_level) + std::to_string(m_value);
	}
	template class Value<double>;

	// ---------------------------------------------------------------------
	// bool
	// ---------------------------------------------------------------------
	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Value<bool>::Serialize(const int& indent_level) const noexcept {
		return Base::Serialize(indent_level) + (m_value ? "true" : "false");
	}
	template class Value<bool>;

	// ---------------------------------------------------------------------
	// Binary (std::vector<std::byte>)
	// ---------------------------------------------------------------------
	template<>
	std::string STORMBYTE_CONFIG_PUBLIC Value<std::vector<std::byte>>::Serialize(const int& indent_level) const noexcept {
		// Convert binary data to Base64 and wrap it with b"..."
		std::string base64 = StormByte::Base64Encode(m_value);
		return Base::Serialize(indent_level) + "b\"" + base64 + "\"";
	}
	template class Value<std::vector<std::byte>>;
}
