#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

namespace StormByte {
	using namespace StormByte::Config::Item;

	template<> STORMBYTE_CONFIG_PRIVATE
	std::vector<std::byte> Serializable<List>::SerializeComplex() const noexcept {
		return Serializable<Container>(m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	std::size_t Serializable<List>::SizeComplex(const List& data) noexcept {
		return Serializable<Container>::Size(data);
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	StormByte::Expected<List, DeserializeError> Serializable<List>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::shared_ptr<Container> list = std::make_shared<List>();
		std::size_t offset = 0;
		auto expected_list = Serialize::DeserializeContainer(data, offset, list);
		if (!expected_list) return Unexpected(expected_list.error());
		return *std::static_pointer_cast<List>(list);
	}
}
