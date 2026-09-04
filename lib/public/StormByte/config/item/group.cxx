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

#include <StormByte/config/item/group.hxx>
using namespace StormByte::Config::Item;
Group::Group(const std::string& name) : Container(name) {}
Group::Group(std::string&& name) : Container(std::move(name)) {}
Base::PointerType Group::BeforeAdditionActions(Base::PointerType item, const StormByte::Config::OnExistingAction onexisting) {
	if (!item) {
		throw Exception("Tried to add a null pointer to Group!");
	}
	if (item->Type() != Type::Comment) {
		if (!item->Name().has_value())
			throw InvalidName("Groups require every item to have a name");
		else if (!item->IsNameValid()) {
			throw InvalidName("Invalid item name '{}' for Group (must start with a letter and contain only letters, digits or underscores)", item->Name().value());
		}
		const auto it = std::find_if(m_items.begin(), m_items.end(), [&item](const Base::PointerType& i) {
			return i->Type() != Type::Comment && i->Name().value() == item->Name().value();
		});
		if (it != m_items.end()) {
			switch (onexisting) {
				case OnExistingAction::Keep:
					return *it;
				case OnExistingAction::Overwrite:
					m_items.erase(it);
					break;
				case OnExistingAction::ThrowException:
					throw ItemNameAlreadyExists(*item->Name());
			}
		}
	}
	m_items.push_back(item);
	return m_items.back();
}
