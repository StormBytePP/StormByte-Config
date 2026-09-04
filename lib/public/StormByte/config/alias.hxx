/*
 * Copyright (C) 2024-2026 David C. Manuelda (StormBytePP)
 *
 * This file is part of StormByte-Config.
 *
 * StormByte-Config is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * or later, as published by the Free Software Foundation.
 *
 * StormByte-Config is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with StormByte-Config. If not, see
 * <https://www.gnu.org/licenses/lgpl-3.0.html>.
 */

#pragma once

#include <functional>
#include <optional>
#include <vector>

/**
 * @brief Config module of the StormByte suite.
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
