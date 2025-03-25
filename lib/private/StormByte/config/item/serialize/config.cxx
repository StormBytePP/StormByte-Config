#include <StormByte/config/item/serialize/base.hxx>
#include <StormByte/config/config.hxx>
#include <StormByte/serializable.hxx>

namespace StormByte {
	template<> STORMBYTE_CONFIG_PUBLIC
	Buffer Serializable<StormByte::Config::Config>::SerializeComplex() const noexcept {
		return
			Serializable<std::optional<StormByte::Config::OnExistingAction>>(m_data.m_on_existing_action).Serialize() <<
			Serializable<StormByte::Config::Item::Container>(m_data.m_root).Serialize();
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	std::size_t Serializable<StormByte::Config::Config>::SizeComplex(const StormByte::Config::Config& data) noexcept {
		return
			Serializable<std::optional<StormByte::Config::OnExistingAction>>::Size(data.m_on_existing_action) +
			Serializable<StormByte::Config::Item::Container>::Size(data.m_root);
	}

	template<> STORMBYTE_CONFIG_PUBLIC
	StormByte::Expected<StormByte::Config::Config, BufferOverflow> Serializable<StormByte::Config::Config>::DeserializeComplex(const Buffer& buffer) noexcept {
		StormByte::Config::Config config;
		auto expected_on_existing_action = Serializable<std::optional<StormByte::Config::OnExistingAction>>::Deserialize(buffer);
		if (!expected_on_existing_action) return StormByte::Unexpected(expected_on_existing_action.error());
		if (expected_on_existing_action.value().has_value())
			config.OnExistingAction(expected_on_existing_action.value().value());

		auto expected_root = Serializable<StormByte::Config::Item::Group>::Deserialize(buffer);
		if (!expected_root) return StormByte::Unexpected(expected_root.error());
		config.m_root = std::move(expected_root.value());

		return config;
	}
}