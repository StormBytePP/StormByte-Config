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
#include <StormByte/config/item/container.hxx>
namespace StormByte::Config::Item {
	/**
	 * @class Group
	 * @brief Represents a group in a configuration file that can hold other items, subgroups, and sublists recursively.
	 * @code
	 * settings = {
	 *     username = "StormByte"
	 *     credit = 66.5
	 * }
	 * @endcode
	 */
	class STORMBYTE_CONFIG_PUBLIC Group: public Container {
		public:
			/**
			 * @brief Constructs an empty Group.
			 */
			Group() = default;

			/**
			 * @brief Constructs a Group with the given name.
			 * @param name The name of the group.
			 */
			Group(const std::string& name);

			/**
			 * @brief Constructs a Group with the given name (move).
			 * @param name The name of the group.
			 */
			Group(std::string&& name);

			/**
			 * @brief Copy constructor.
			 * @param group Group to copy.
			 */
			Group(const Group& group) = default;

			/**
			 * @brief Move constructor.
			 * @param group Group to move.
			 */
			Group(Group&& group) noexcept = default;

			/**
			 * @brief Copy assignment operator.
			 * @param group Group to copy.
			 * @return Reference to this Group.
			 */
			Group& operator=(const Group& group) = default;

			/**
			 * @brief Move assignment operator.
			 * @param group Group to move.
			 * @return Reference to this Group.
			 */
			Group& operator=(Group&& group) noexcept = default;

			/**
			 * @brief Destructor.
			 */
			~Group() noexcept override = default;

			/**
			 * @brief Clones the group.
			 * @return Cloned group.
			 */
			inline PointerType Clone() const override {
				return MakePointer<Group>(*this);
			}

			/**
			 * @brief Moves the group.
			 * @return Moved group.
			 */
			inline PointerType Move() override {
				return MakePointer<Group>(std::move(*this));
			}

			/**
			 * @brief Gets the container type.
			 * @return Container type.
			 */
			constexpr Item::ContainerType ContainerType() const noexcept override {
				return ContainerType::Group;
			}

		private:
			/**
			 * @brief Actions performed before adding an item.
			 * @param item Item to check.
			 * @param onexisting Action to take if item name already exists.
			 * @throw ItemNameAlreadyExists if item name already exists.
			 * @return Pointer to the kept/added item.
			 */
			Base::PointerType BeforeAdditionActions(Base::PointerType item, const StormByte::Config::OnExistingAction onexisting) override;
	};
}
