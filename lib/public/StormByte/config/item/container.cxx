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

#include <StormByte/config/item/container.hxx>
#include <StormByte/string/string.hxx>

#include <regex>
#include <string>
#include <string_view>

using namespace StormByte::Config::Item;
using StormByte::String::String;

namespace {
	String Indent(const int indent_level) {
		const int level = indent_level > 0 ? indent_level : 0;
		return String(std::string(static_cast<std::size_t>(level), '\t'));
	}
}

Container::Container(const String& name) : Base(name) {}

Container::Container(String&& name) : Base(name) {}

bool Container::Equals(const Base& other) const noexcept {
	if (this->Type() != other.Type())
		return false;
	if (this->Name() != other.Name())
		return false;
	const Container& other_container = static_cast<const Container&>(other);
	if (this->ContainerType() != other_container.ContainerType())
		return false;
	if (m_items.size() != other_container.m_items.size())
		return false;
	for (std::size_t i = 0; i < m_items.size(); ++i) {
		if (!m_items[i]->Equals(*other_container.m_items[i]))
			return false;
	}

	return true;
}

bool Container::operator==(const Container& container) const noexcept {
	return Equals(container);
}

Base& Container::operator[](const size_t& index) {
	return const_cast<Base&>(static_cast<const Container&>(*this)[index]);
}

const Base& Container::operator[](const size_t& index) const {
	if (index >= m_items.size())
		throw OutOfBounds("Index {} is out of bounds (max size is {})", index, m_items.size());
	return *m_items[index];
}

Base& Container::operator[](const String& path) {
	return const_cast<Base&>(static_cast<const Container&>(*this)[path]);
}

Base& Container::Add(Base::PointerType item, const StormByte::Config::OnExistingAction& on_existing) {
	if (!item) {
		throw Exception("Tried to add a null pointer to Container!");
	}

	if (item->Type() == Type::Container) {
		auto& child = static_cast<Container&>(*item);
		child.SetOnExistingAction(on_existing);
	}

	Base::PointerType i = this->BeforeAdditionActions(item, on_existing);
	if (i)
		return *i;
	m_items.push_back(item);
	return *m_items.back();
}

bool Container::Exists(const String& path) const {
	try {
		LookUp(path);
		return true;
	} catch (const Exception&) {
		return false;
	}
}

void Container::Remove(const size_t& index) {
	if (index >= m_items.size())
		throw OutOfBounds("Index {} is out of bounds (max size: {})", index, m_items.size());
	m_items.erase(m_items.begin() + index);
}

void Container::Remove(const String& path) {
	if (!IsPathValid(path)) {
		throw InvalidPath("Invalid path '{}'", static_cast<std::string_view>(path));
	}

	auto path_queue = path.Explode('/');
	Remove(path_queue);
}

String Container::Serialize(const int& indent_level) const noexcept {
	const auto enclosure_characters = EnclosureCharacters(ContainerType());
	std::string serial = static_cast<std::string>(Base::Serialize(indent_level));
	serial += enclosure_characters.first;
	serial += '\n';
	serial += static_cast<std::string>(ContentsToString(indent_level + 1));
	serial += static_cast<std::string>(Indent(indent_level));
	serial += enclosure_characters.second;
	return String(std::string_view(serial));
}

size_t Container::Count() const noexcept {
	size_t count = 0;
	for (const auto& item : m_items) {
		switch (item->Type()) {
			case Type::Container:
				count += 1 + item->Value<Container>().Count();
				break;
			default:
				count++;
				break;
		}
	}

	return count;
}

String Container::ContentsToString(const int& indent_level) const noexcept {
	std::string serial;
	for (const auto& item : m_items) {
		serial += static_cast<std::string>(item->Serialize(indent_level));
		serial += '\n';
	}
	return String(std::string_view(serial));
}

bool Container::IsPathValid(const String& name) noexcept {
	static const std::regex name_regex(R"(^[A-Za-z0-9_]+(/([A-Za-z0-9_]+))*$)");
	return std::regex_match(static_cast<std::string>(name), name_regex);
}

const Base& Container::LookUp(const String& path) const {
	if (!IsPathValid(path)) {
		throw InvalidPath("Invalid path '{}'", static_cast<std::string_view>(path));
	}

	auto path_queue = path.Explode('/');
	return LookUp(path_queue);
}

const Base& Container::LookUp(std::queue<String>& path) const {
	if (path.empty()) {
		throw InvalidPath("Empty path given for lookup");
	}

	const String item_path = path.front();
	path.pop();
	const std::string_view item_view = item_path;
	if (path.empty()) {
		if (item_path.IsInteger()) {
			try {
				int idx = std::stoi(std::string(item_view));
				if (idx < 0) {
					throw OutOfBounds("Index {} is out of bounds", idx);
				}

				return operator[](static_cast<size_t>(idx));
			} catch (const OutOfBounds&) {
				throw;
			} catch (const std::exception&) {
				throw OutOfBounds("Invalid or out of bounds index '{}'", item_view);
			}
		} else {
			const auto it = std::find_if(m_items.begin(), m_items.end(), [&item_path](const Base::PointerType& item) {
				const auto& name = item->Name();
				return name && name.value() == item_path;
			});
			if (it != m_items.end())
				return **it;
			throw ItemNotFound(std::string(item_view));
		}
	} else {
		size_t idx = 0;
		const bool is_num = item_path.IsInteger();
		if (is_num) {
			try {
				int num = std::stoi(std::string(item_view));
				if (num < 0) {
					throw OutOfBounds("Index {} is out of bounds", num);
				}

				idx = static_cast<size_t>(num);
			} catch (const OutOfBounds&) {
				throw;
			} catch (const std::exception&) {
				throw OutOfBounds("Invalid or out of bounds index '{}'", item_view);
			}
		}

		const Base& item = is_num ? operator[](idx) : operator[](item_path);
		if (item.Type() != Type::Container)
			throw Exception("Lookup path " + std::string(item_view) + " applied to non container item");
		return static_cast<const Container&>(item).LookUp(path);
	}
}

void Container::Remove(std::queue<String>& path) {
	if (path.empty()) {
		throw InvalidPath("Empty path given for remove");
	}

	String item_path = path.front();
	path.pop();
	const std::string_view item_view = item_path;
	if (path.empty()) {
		if (item_path.IsInteger()) {
			try {
				int idx = std::stoi(std::string(item_view));
				if (idx < 0) {
					throw OutOfBounds("Index {} is out of bounds", idx);
				}

				Remove(static_cast<size_t>(idx));
			} catch (const OutOfBounds&) {
				throw;
			} catch (const std::exception&) {
				throw OutOfBounds("Invalid or out of bounds index '{}'", item_view);
			}
		} else {
			const auto it = std::find_if(m_items.begin(), m_items.end(), [&item_path](const Base::PointerType& item) {
				const auto& name = item->Name();
				return name && name.value() == item_path;
			});
			if (it != m_items.end())
				m_items.erase(it);
			else
				throw ItemNotFound(std::string(item_view));
		}
	} else {
		size_t idx = 0;
		const bool is_num = item_path.IsInteger();
		if (is_num) {
			try {
				int num = std::stoi(std::string(item_view));
				if (num < 0) {
					throw OutOfBounds("Index {} is out of bounds", num);
				}

				idx = static_cast<size_t>(num);
			} catch (const OutOfBounds&) {
				throw;
			} catch (const std::exception&) {
				throw OutOfBounds("Invalid or out of bounds index '{}'", item_view);
			}
		}

		Base& item = is_num ? operator[](idx) : operator[](item_path);
		if (item.Type() != Type::Container)
			throw Exception("Lookup path " + std::string(item_view) + " applied to non container item");
		item.Value<Container>().Remove(path);
	}
}
