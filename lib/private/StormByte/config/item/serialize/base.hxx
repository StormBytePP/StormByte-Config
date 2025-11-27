#pragma once

#include <StormByte/config/item/container.hxx>
#include <StormByte/config/item/type.hxx>
#include <StormByte/config/visibility.h>
#include <StormByte/exception.hxx>
#include <StormByte/expected.hxx>

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace StormByte::Config::Item::Serialize {
	using BaseData = std::pair<Type, std::optional<std::string>>;	///< Base data for a configuration item

	/**
	 * @brief Deserializes basic data for a configuration item.
	 * @param data The byte vector containing serialized data.
	 * @param offset Reference to the current position in the data.
	 * @return Expected<BaseData, DeserializeError> The deserialized data or an error.
	 */
	STORMBYTE_CONFIG_PRIVATE Expected<BaseData, DeserializeError> DeserializeBasicData(const std::vector<std::byte>& data, std::size_t& offset) noexcept;

	/**
	 * @brief Deserializes basic data for a configuration item and validates the item type.
	 * @param data The byte vector containing serialized data.
	 * @param offset Reference to the current position in the data.
	 * @param item_type The expected type of the item.
	 * @return Expected<BaseData, DeserializeError> The deserialized data or an error.
	 */
	STORMBYTE_CONFIG_PRIVATE Expected<BaseData, DeserializeError> DeserializeBasicData(const std::vector<std::byte>& data, std::size_t& offset, const Type& item_type) noexcept;

	/**
	 * @brief Deserializes a container from the byte vector.
	 * @param data The byte vector containing serialized data.
	 * @param offset Reference to the current position in the data.
	 * @param container The container to deserialize into.
	 * @return Expected<std::shared_ptr<Container>, DeserializeError> The deserialized container or an error.
	 */
	STORMBYTE_CONFIG_PRIVATE Expected<std::shared_ptr<Container>, DeserializeError> DeserializeContainer(const std::vector<std::byte>& data, std::size_t& offset, std::shared_ptr<Container> container) noexcept;
}