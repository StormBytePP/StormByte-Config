#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

namespace StormByte {
	using namespace StormByte::Config::Item;

	template<> STORMBYTE_CONFIG_PRIVATE
	std::vector<std::byte> Serializable<Group>::SerializeComplex() const noexcept {
		return Serializable<Container>(m_data).Serialize();
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	std::size_t Serializable<Group>::SizeComplex(const Group& data) noexcept {
		return Serializable<Container>::Size(data);
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	StormByte::Expected<Group, DeserializeError> Serializable<Group>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		std::shared_ptr<Container> group = std::make_shared<Group>();
		std::size_t offset = 0;
		auto expected_group = Serialize::DeserializeContainer(data, offset, group);
		if (!expected_group) return Unexpected(expected_group.error());
		return *std::static_pointer_cast<Group>(group);
	}
}
