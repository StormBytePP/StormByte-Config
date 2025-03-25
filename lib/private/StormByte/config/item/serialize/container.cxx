#include <StormByte/config/item/container.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

namespace StormByte {
	using namespace StormByte::Config::Item;

	// Container
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer Serializable<Container>::SerializeComplex() const noexcept {
		Buffer buffer = Serializable<Base>(m_data).Serialize() <<
			Serializable<ContainerType>(m_data.ContainerType()).Serialize() <<
			Serializable<std::size_t>(m_data.Items().size()).Serialize();
		for (auto& item : m_data.Items()) {
			buffer << Serializable<std::shared_ptr<Base>>(item).Serialize();
		}
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<Container>::SizeComplex(const Container& data) noexcept {
		std::size_t size = Serializable<Base>::Size(data) +
			Serializable<ContainerType>::Size(data.ContainerType()) +
			Serializable<std::size_t>::Size(data.Items().size());
		for (const auto& item : data.Items()) {
			size += Serializable<std::shared_ptr<Base>>::Size(item);
		}
		return size;
	}
}