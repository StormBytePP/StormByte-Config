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
	using BaseData = std::pair<Type, std::optional<std::string>>;	///< Base data for a config item

	/**
	 * @brief Deserialize basic data for a config item
	 * @param data Data to deserialize
	 * @param length Length of the data
	 * @return Expected<SerializedData, DeserializeError> Serialized data
	 */
	STORMBYTE_CONFIG_PRIVATE Expected<BaseData, Buffers::BufferOverflow> DeserializeBasicData(const Buffers::Simple& buffer) noexcept;

	/**
	 * @brief Deserialize basic data for a config item and checks for the item type correctness
	 * @param data Data to deserialize
	 * @param length Length of the data
	 * @param item_type Type of the item
	 * @return Expected<SerializedData, DeserializeError> Serialized data
	 */
	STORMBYTE_CONFIG_PRIVATE Expected<BaseData, Buffers::BufferOverflow> DeserializeBasicData(const Buffers::Simple& buffer, const Type& item_type) noexcept;

	/**
	 * @brief Deserialize a container.
	 * @param data Data to deserialize
	 * @param length Length of the data
	 * @param container Container to deserialize into
	 * @return Buffers::Simple Serialized data
	 */
	STORMBYTE_CONFIG_PRIVATE Expected<std::shared_ptr<Container>, Buffers::BufferOverflow> DeserializeContainer(const Buffers::Simple& buffer, std::shared_ptr<Container> container) noexcept;
}