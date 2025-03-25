#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

namespace StormByte {
	using namespace StormByte::Config::Item;

	// List
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer Serializable<List>::SerializeComplex() const noexcept {
		return Serializable<Container>(m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<List>::SizeComplex(const List& data) noexcept {
		return Serializable<Container>::Size(data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	StormByte::Expected<List, BufferOverflow> Serializable<List>::DeserializeComplex(const Buffer& buffer) noexcept {
		std::shared_ptr<Container> list = std::make_shared<List>();
		auto expected_group = Serialize::DeserializeContainer(buffer, list);
		if (!expected_group) return Unexpected(expected_group.error());
		return *std::static_pointer_cast<List>(list);
	}
}