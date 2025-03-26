#pragma once

#include <functional>
#include <optional>
#include <vector>

/**
 * @namespace Config
 * @brief Contains type aliases and utilities for configuration handling.
 */
namespace StormByte::Config {
	namespace Item {
		class Group; ///< Forward declaration of Group.
	}

	using HookFunction 			= std::function<void(Item::Group&)>;		///< Function executed as a hook.
	using HookFunctions 		= std::vector<HookFunction>;				///< List of hook functions.
	using OnFailureHook			= std::function<bool(const Item::Group&)>;	///< Function executed on failure.
	using OptionalFailureHook	= std::optional<OnFailureHook>;				///< Optional failure hook.
}