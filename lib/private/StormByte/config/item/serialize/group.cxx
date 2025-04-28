#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>
#include <iostream>
namespace StormByte {
	using namespace StormByte::Config::Item;

	// Group
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer::Simple Serializable<Group>::SerializeComplex() const noexcept {
		return Serializable<Container>(m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Group>::SizeComplex(const Group& data) noexcept {
		return Serializable<Container>::Size(data);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	StormByte::Expected<Group, Buffer::BufferOverflow> Serializable<Group>::DeserializeComplex(const Buffer::Simple& buffer) noexcept {
		std::shared_ptr<Container> group = std::make_shared<Group>();
		auto expected_group = Serialize::DeserializeContainer(buffer, group);
		if (!expected_group) return Unexpected(expected_group.error());
		return *std::static_pointer_cast<Group>(group);
	}
}