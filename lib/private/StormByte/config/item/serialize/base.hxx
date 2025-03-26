#pragma once

#include <StormByte/buffers/simple.hxx>
#include <StormByte/config/item/container.hxx>
#include <StormByte/config/item/type.hxx>
#include <StormByte/config/visibility.h>
#include <StormByte/exception.hxx>
#include <StormByte/expected.hxx>

#include <memory>
#include <optional>
#include <string>

namespace StormByte::Config::Item::Serialize {
	using BaseData = std::pair<Type, std::optional<std::string>>;	///< Base data for a configuration item

	/**
	 * @brief Deserializes basic data for a configuration item.
	 * @param buffer The buffer containing serialized data.
	 * @return Expected<BaseData, Buffers::BufferOverflow> The deserialized data or an error.
	 */
	STORMBYTE_CONFIG_PRIVATE Expected<BaseData, Buffers::BufferOverflow> DeserializeBasicData(const Buffers::Simple& buffer) noexcept;

	/**
	 * @brief Deserializes basic data for a configuration item and validates the item type.
	 * @param buffer The buffer containing serialized data.
	 * @param item_type The expected type of the item.
	 * @return Expected<BaseData, Buffers::BufferOverflow> The deserialized data or an error.
	 */
	STORMBYTE_CONFIG_PRIVATE Expected<BaseData, Buffers::BufferOverflow> DeserializeBasicData(const Buffers::Simple& buffer, const Type& item_type) noexcept;

	/**
	 * @brief Deserializes a container from the buffer.
	 * @param buffer The buffer containing serialized data.
	 * @param container The container to deserialize into.
	 * @return Expected<std::shared_ptr<Container>, Buffers::BufferOverflow> The deserialized container or an error.
	 */
	STORMBYTE_CONFIG_PRIVATE Expected<std::shared_ptr<Container>, Buffers::BufferOverflow> DeserializeContainer(const Buffers::Simple& buffer, std::shared_ptr<Container> container) noexcept;
}