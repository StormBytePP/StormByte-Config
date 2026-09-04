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

/**
 * @brief Configuration items (values, comments, groups, lists).
 */
namespace StormByte::Config::Item {
	/**
	 * @class List
	 * @brief Unnamed-item container `[ … ]`.
	 * @code
	 * include_dirs = [
	 *     "/usr/include"
	 *     "/usr/local/include"
	 * ]
	 * @endcode
	 */
	class STORMBYTE_CONFIG_PUBLIC List final: public Container {
		public:
			/**
			 * @name Construction
			 * @{
			 */
			/**
			 * @brief Constructs an empty List.
			 */
			List() = default;

			/**
			 * @brief Constructs a List with the given name.
			 * @param name The name of the list.
			 */
			List(const std::string& name);

			/**
			 * @brief Constructs a List with the given name (move).
			 * @param name The name of the list.
			 */
			List(std::string&& name);

			/**
			 * @brief Copy constructor.
			 * @param list List to copy.
			 */
			List(const List& list) = default;

			/**
			 * @brief Move constructor.
			 * @param list List to move.
			 */
			List(List&& list) noexcept = default;

			/**
			 * @brief Copy assignment operator.
			 * @param list List to assign.
			 * @return Reference to this List.
			 */
			List& operator=(const List& list) = default;

			/**
			 * @brief Move assignment operator.
			 * @param list List to move.
			 * @return Reference to this List.
			 */
			List& operator=(List&& list) noexcept = default;

			/**
			 * @brief Destructor.
			 */
			~List() noexcept override = default;
			/** @} */

			/**
			 * @name Access
			 * @{
			 */
			/**
			 * @brief Clones the list.
			 * @return Cloned list.
			 */
			inline PointerType Clone() const override {
				return MakePointer<List>(*this);
			}

			/**
			 * @brief Moves the list.
			 * @return Moved list.
			 */
			inline PointerType Move() override {
				return MakePointer<List>(std::move(*this));
			}

			/**
			 * @brief Gets the container type.
			 * @return Container type.
			 */
			constexpr Item::ContainerType ContainerType() const noexcept override {
				return ContainerType::List;
			}
			/** @} */

		private:
			/**
			 * @brief Actions performed before adding an item.
			 * @param item Item to check.
			 * @param onexisting Action to take if item already exists.
			 * @throw ItemAlreadyExists if item already exists.
			 * @return Pointer to the kept/added item.
			 */
			Base::PointerType BeforeAdditionActions(Base::PointerType item, const StormByte::Config::OnExistingAction onexisting) override;
	};
}
