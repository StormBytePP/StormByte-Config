/*
* Copyright (C) 2024-2026 David C. Manuelda (StormBytePP)
*
* This file is part of StormByte-Config.
*
* StormByte-Config original source is dual-licensed:
*
* 1. GNU Lesser General Public License v3.0 (or later)
*    You may redistribute and/or modify this file under the terms of the
*    GNU Lesser General Public License as published by the Free Software
*    Foundation, either version 3 of the License, or (at your option)
*    any later version.
*
* 2. Commercial license
*    Alternatively, this file may be used under the terms of a commercial
*    license agreement with the copyright holder
*    (David C. Manuelda <StormByte@gmail.com>).
*
* Both licenses apply only to original StormByte-Config source in this
* repository. They do not cover other StormByte modules or any third-party
* material shipped with this repository (including everything under
* thirdparty/, and in particular the bundled StormByte-String tree and
* the StormByte Base tree it vendors), which remains under its own license.
*
* Neither license grants any patent rights. Any patent licenses required
* to use this software or third-party components must be obtained separately
* from the patent holders.
*
* StormByte-Config is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* version 3 along with StormByte-Config. If not, see
* <https://www.gnu.org/licenses/lgpl-3.0.html>.
*
* SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-StormByte-Commercial
*/

#pragma once

#include <StormByte/config/item/container.hxx>
#include <StormByte/string/string.hxx>

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
			List(const StormByte::String::String& name);

			/**
			 * @brief Constructs a List with the given name (move).
			 * @param name The name of the list.
			 */
			List(StormByte::String::String&& name);

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
