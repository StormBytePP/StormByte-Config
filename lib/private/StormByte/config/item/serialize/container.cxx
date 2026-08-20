#include <StormByte/config/item/container.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

namespace StormByte {
	using namespace StormByte::Config::Item;

	template<> STORMBYTE_CONFIG_PRIVATE
	std::size_t Serializable<Container>::SizeComplex(const Container& data) noexcept {
		std::size_t size = Serializable<Base>::Size(data) +
			Serializable<ContainerType>::Size(data.ContainerType()) +
			Serializable<std::uint64_t>::Size(static_cast<std::uint64_t>(data.Items().size()));
		for (const auto& item : data.Items())
			size += Serializable<std::shared_ptr<Base>>::Size(item);
		return size;
	}

	template<> STORMBYTE_CONFIG_PRIVATE
	std::vector<std::byte> Serializable<Container>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer;
		buffer.reserve(SizeComplex(m_data));
		append_vector(buffer, Serializable<Base>(m_data).Serialize());
		append_vector(buffer, Serializable<ContainerType>(m_data.ContainerType()).Serialize());
		append_vector(buffer, Serializable<std::uint64_t>(static_cast<std::uint64_t>(m_data.Items().size())).Serialize());
		for (auto& item : m_data.Items())
			append_vector(buffer, Serializable<std::shared_ptr<Base>>(item).Serialize());
		return buffer;
	}
}
