#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/config/config.hxx>
#include <StormByte/serializable.hxx>

namespace StormByte {

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<StormByte::Config::Config>::SizeComplex(const StormByte::Config::Config& data) noexcept {
		return
			Serializable<std::optional<StormByte::Config::OnExistingAction>>::Size(data.m_on_existing_action) +
			Serializable<StormByte::Config::Item::Container>::Size(data.m_root);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::vector<std::byte> Serializable<StormByte::Config::Config>::SerializeComplex() const noexcept {
		std::vector<std::byte> buffer;
		buffer.reserve(SizeComplex(m_data));
		append_vector(
			buffer,
			Serializable<std::optional<StormByte::Config::OnExistingAction>>(m_data.m_on_existing_action).Serialize()
		);
		append_vector(
			buffer,
			Serializable<StormByte::Config::Item::Container>(m_data.m_root).Serialize()
		);
		return buffer;
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	StormByte::Expected<StormByte::Config::Config, DeserializeError> Serializable<StormByte::Config::Config>::DeserializeComplex(std::span<const std::byte> data) noexcept {
		StormByte::Config::Config config;
		std::size_t offset = 0;

		if (offset >= data.size())
			return StormByte::Unexpected<DeserializeError>("Insufficient data for on_existing_action");

		auto expected_on_existing_action = Serializable<std::optional<StormByte::Config::OnExistingAction>>::Deserialize(data.subspan(offset));
		if (!expected_on_existing_action) return StormByte::Unexpected(expected_on_existing_action.error());
		offset += Serializable<std::optional<StormByte::Config::OnExistingAction>>::Size(expected_on_existing_action.value());

		if (expected_on_existing_action.value().has_value())
			config.OnExistingAction(expected_on_existing_action.value().value());

		if (offset >= data.size())
			return StormByte::Unexpected<DeserializeError>("Insufficient data for root group");

		auto expected_root = Serializable<StormByte::Config::Item::Group>::Deserialize(data.subspan(offset));
		if (!expected_root) return StormByte::Unexpected(expected_root.error());
		config.m_root = std::move(expected_root.value());

		return config;
	}
}
