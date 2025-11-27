#include <StormByte/config/item/container.hxx>
#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/serializable.hxx>

namespace StormByte {
	using namespace StormByte::Config::Item;

	// Container
	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<Container>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer = Serializable<Base>(m_data).Serialize();
		std::vector<std::byte> container_type_data = Serializable<ContainerType>(m_data.ContainerType()).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(container_type_data.begin()), std::make_move_iterator(container_type_data.end()));
		std::vector<std::byte> size_data = Serializable<std::size_t>(m_data.Items().size()).Serialize();
		buffer.insert(buffer.end(), std::make_move_iterator(size_data.begin()), std::make_move_iterator(size_data.end()));
		for (auto& item : m_data.Items()) {
			std::vector<std::byte> item_data = Serializable<std::shared_ptr<Base>>(item).Serialize();
			buffer.insert(buffer.end(), std::make_move_iterator(item_data.begin()), std::make_move_iterator(item_data.end()));
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