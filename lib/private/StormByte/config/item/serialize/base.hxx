#pragma once

#include <StormByte/config/item/container.hxx>
#include <StormByte/config/item/type.hxx>
#include <StormByte/config/visibility.h>
#include <StormByte/exception.hxx>
#include <StormByte/expected.hxx>

#include <cstddef>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace StormByte::Config::Item {
	class Base;
}

namespace StormByte::Config::Item::Serialize {
	using BaseData = std::pair<Type, std::optional<std::string>>;

	STORMBYTE_CONFIG_PRIVATE Expected<BaseData, DeserializeError>
	DeserializeBasicData(std::span<const std::byte> data, std::size_t& offset) noexcept;

	STORMBYTE_CONFIG_PRIVATE Expected<BaseData, DeserializeError>
	DeserializeBasicData(std::span<const std::byte> data, std::size_t& offset, const Type& item_type) noexcept;

	STORMBYTE_CONFIG_PRIVATE Expected<std::shared_ptr<Base>, DeserializeError>
	DeserializeItem(std::span<const std::byte> data, std::size_t& offset) noexcept;

	STORMBYTE_CONFIG_PRIVATE Expected<std::shared_ptr<Container>, DeserializeError>
	DeserializeContainer(std::span<const std::byte> data, std::size_t& offset, std::shared_ptr<Container> container) noexcept;
}
