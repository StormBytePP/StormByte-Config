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

#include <StormByte/config/alias.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/config/typedefs.hxx>

#include <cstddef>
#include <istream>
#include <ostream>
#include <span>
#include <string>
#include <vector>

/**
 * @brief Config module of the StormByte suite.
 */
namespace StormByte::Config {
	namespace Binary {
		class Reader;
		class Writer;
	}

	/**
	 * @class Config
	 * @brief Configuration document (text or versioned binary).
	 *
	 * A document holds:
	 * - Boolean, double, integer, string and binary values
	 * - Comments (single-line or multi-line)
	 * - Groups and lists
	 */
	class STORMBYTE_CONFIG_PUBLIC Config {
		friend class Binary::Reader;
		friend class Binary::Writer;
		public:
			/**
			 * @name Construction
			 * @{
			 */
			/**
			 * @brief Default constructor.
			 */
			Config();

			/**
			 * @brief Copy constructor.
			 * @param config Configuration to copy.
			 */
			Config(const Config& config) = default;

			/**
			 * @brief Move constructor.
			 * @param config Configuration to move.
			 */
			Config(Config&& config) noexcept = default;

			/**
			 * @brief Copy assignment operator.
			 * @param config Configuration to assign.
			 * @return Reference to this Config.
			 */
			Config& operator=(const Config& config) = default;

			/**
			 * @brief Move assignment operator.
			 * @param config Configuration to move.
			 * @return Reference to this Config.
			 */
			Config& operator=(Config&& config) noexcept = default;

			/**
			 * @brief Destructor.
			 */
			virtual ~Config() = default;
			/** @} */

			/**
			 * @name Access
			 * @{
			 */
			/**
			 * @brief Gets a reference to an item by path.
			 * @param path Path to the item.
			 * @return Item reference.
			 */
			inline Item::Base& operator[](const std::string& path) {
				return m_root.operator[](path);
			}

			/**
			 * @brief Gets a const reference to an item by path.
			 * @param path Path to the item.
			 * @return Item const reference.
			 */
			inline const Item::Base& operator[](const std::string& path) const {
				return m_root.operator[](path);
			}

			/**
			 * @brief Gets a reference to an item by index.
			 * @param index Index of the item.
			 * @throw OutOfBounds if index is out of bounds.
			 * @return Item reference.
			 */
			Item::Base& operator[](const size_t& index) {
				return m_root[index];
			}

			/**
			 * @brief Gets a const reference to an item by index.
			 * @param index Index of the item.
			 * @throw OutOfBounds if index is out of bounds.
			 * @return Item const reference.
			 */
			const Item::Base& operator[](const size_t& index) const {
				return m_root[index];
			}

			/**
			 * @brief Equality operator.
			 * @param config Configuration to compare.
			 * @return true if equal.
			 */
			inline bool operator==(const Config& config) const noexcept {
				return m_root == config.m_root;
			}

			/**
			 * @brief Inequality operator.
			 * @param config Configuration to compare.
			 * @return true if not equal.
			 */
			inline bool operator!=(const Config& config) const noexcept {
				return !operator==(config);
			}
			/** @} */

			/**
			 * @name Input
			 * @{
			 */
			/**
			 * @brief Import data from another configuration.
			 * @param source Source configuration to import.
			 * @return Reference to this configuration.
			 */
			Config& operator<<(const Config& source);

			/**
			 * @brief Initialize configuration from an input stream (text mode).
			 * @param istream Input stream.
			 */
			void operator<<(std::istream& istream);

			/**
			 * @brief Initialize configuration from a string (text mode).
			 * @param str Input string.
			 */
			void operator<<(const std::string& str);

			/**
			 * @brief Initializes configuration when istream is on the left-hand side.
			 * @param istream Input stream.
			 * @param file Config to put data into.
			 * @return Reference to the Config.
			 */
			friend STORMBYTE_CONFIG_PUBLIC Config& operator>>(std::istream& istream, Config& file);

			/**
			 * @brief Initializes configuration when string is on the left-hand side.
			 * @param str Input string.
			 * @param file Config to put data into.
			 * @return Reference to the Config.
			 */
			friend STORMBYTE_CONFIG_PUBLIC Config& operator>>(const std::string& str, Config& file);
			/** @} */

			/**
			 * @name Output
			 * @{
			 */
			/**
			 * @brief Output current configuration into another configuration.
			 * @param dest Destination configuration.
			 * @return Reference to the destination configuration.
			 */
			Config& operator>>(Config& dest) const;

			/**
			 * @brief Output configuration serialized to an output stream (text).
			 * @param ostream Output stream.
			 * @return Reference to the output stream.
			 */
			std::ostream& operator>>(std::ostream& ostream) const;

			/**
			 * @brief Output configuration serialized to a string (text).
			 * @param str Output string.
			 * @return Reference to the string.
			 */
			std::string& operator>>(std::string& str) const;

			/**
			 * @brief Output configuration when ostream is on the left-hand side.
			 * @param ostream Output stream.
			 * @param file Config to get data from.
			 * @return Reference to the output stream.
			 */
			friend STORMBYTE_CONFIG_PUBLIC std::ostream& operator<<(std::ostream& ostream, const Config& file);

			/**
			 * @brief Output configuration when string is on the left-hand side.
			 * @param str Output string.
			 * @param file Config to get data from.
			 * @return Reference to the string.
			 */
			friend STORMBYTE_CONFIG_PUBLIC std::string& operator<<(std::string&, const Config&);

			/**
			 * @brief Converts the current configuration to a string (text form).
			 * @return Serialized configuration text.
			 */
			operator std::string() const;

			/**
			 * @brief Write this document to an output stream.
			 * @param stream Destination (file, stringstream, etc.).
			 * @param mode Text (default): config syntax; Binary: versioned wire format.
			 */
			void Save(std::ostream& stream, Mode mode = Mode::Text) const;

			/**
			 * @brief Read a document from an input stream.
			 * @param stream Source (file, stringstream, etc.).
			 * @param mode Text (default): config syntax; Binary: versioned wire format.
			 * @return Config on success, or a StormByte::Exception derivative on failure.
			 */
			static ExpectedConfig Load(std::istream& stream, Mode mode = Mode::Text);
			/** @} */

			/**
			 * @name Items
			 * @{
			 */
			/**
			 * @brief Adds an item to the configuration.
			 * @param item The item to add.
			 * @throw ItemNameAlreadyExists if the item's name already exists.
			 * @return A reference to the added item.
			 */
			inline Item::Base& Add(const Item::Base& item) {
				return m_root.Add(item.Clone(), m_on_existing_action);
			}

			/**
			 * @brief Adds an item to the configuration (move).
			 * @param item Item to add.
			 * @throw ItemNameAlreadyExists if item name already exists.
			 * @return Reference to the added item.
			 */
			Item::Base& Add(Item::Base&& item) {
				return m_root.Add(std::move(item.Move()), m_on_existing_action);
			}

			/**
			 * @brief Clears all configuration items.
			 */
			inline void Clear() noexcept {
				m_root.Clear();
			}

			/**
			 * @brief Checks if an item exists by path.
			 * @param path Path to the item.
			 * @return true if the item exists.
			 */
			inline bool Exists(const std::string& path) const {
				return m_root.Exists(path);
			}

			/**
			 * @brief Removes an item by path.
			 * @param path Item path.
			 * @throw ItemNotFound if item is not found.
			 */
			inline void Remove(const std::string& path) {
				m_root.Remove(path);
			}

			/**
			 * @brief Removes an item by index.
			 * @param index Index of the item.
			 * @throw OutOfBounds if index is out of bounds.
			 */
			inline void Remove(const size_t& index) {
				m_root.Remove(index);
			}

			/**
			 * @brief Gets the number of items in the current level.
			 * @return Number of items.
			 */
			constexpr virtual size_t Size() const noexcept {
				return m_root.Size();
			}

			/**
			 * @brief Gets the full number of items (including nested).
			 * @return Total number of items.
			 */
			inline virtual size_t Count() const noexcept {
				return m_root.Count();
			}

			/**
			 * @brief Gets the items in the current level.
			 * @return Span of items.
			 */
			constexpr std::span<Item::Base::PointerType> Items() noexcept {
				return m_root.Items();
			}

			/**
			 * @brief Gets the items in the current level (const).
			 * @return Const span of items.
			 */
			constexpr std::span<const Item::Base::PointerType> Items() const noexcept {
				return m_root.Items();
			}
			/** @} */

			/**
			 * @name Policy
			 * @{
			 */
			/**
			 * @brief Sets the action to take when an item name/identity collision occurs.
			 * The policy is applied to the root container and will be inherited by all nested containers.
			 * @param on_existing The policy to use.
			 */
			void OnExistingAction(const OnExistingAction& on_existing) {
				m_on_existing_action = on_existing;
				m_root.SetOnExistingAction(on_existing);
			}

			/**
			 * @brief Sets a function to execute on parse failure.
			 * @param hook Function to execute.
			 */
			constexpr void OnParseFailure(OnFailureHook hook) {
				m_on_parse_failure_hook = hook;
			}

			/**
			 * @brief Adds a hook executed before reading starts.
			 * Hooks run in order.
			 * @param hook Hook function.
			 */
			constexpr void AddHookBeforeRead(HookFunction hook) {
				m_before_read_hooks.push_back(hook);
			}

			/**
			 * @brief Adds a hook executed after a successful read.
			 * Hooks run in order.
			 * @param hook Hook function.
			 */
			constexpr void AddHookAfterRead(HookFunction hook) {
				m_after_read_hooks.push_back(hook);
			}
			/** @} */

		protected:
			Item::Group m_root; ///< Root group

			HookFunctions m_before_read_hooks;				///< Hooks executed before reading
			HookFunctions m_after_read_hooks;				///< Hooks executed after successful reading
			OptionalFailureHook m_on_parse_failure_hook;	///< Hook executed on failure
			StormByte::Config::OnExistingAction m_on_existing_action; ///< Collision policy
	};

	/**
	 * @brief Initializes configuration when istream is on the left-hand side.
	 * @param istream Input stream.
	 * @param file Config to put data into.
	 * @return Reference to the Config.
	 */
	STORMBYTE_CONFIG_PUBLIC Config& operator>>(std::istream& istream, Config& file);

	/**
	 * @brief Initializes configuration when string is on the left-hand side.
	 * @param str Input string.
	 * @param file Config to put data into.
	 * @return Reference to the Config.
	 */
	STORMBYTE_CONFIG_PUBLIC Config& operator>>(const std::string& str, Config& file);

	/**
	 * @brief Output configuration when ostream is on the left-hand side.
	 * @param ostream Output stream.
	 * @param file Config to get data from.
	 * @return Reference to the output stream.
	 */
	STORMBYTE_CONFIG_PUBLIC std::ostream& operator<<(std::ostream& ostream, const Config& file);

	/**
	 * @brief Output configuration when string is on the left-hand side.
	 * @param str Output string.
	 * @param file Config to get data from.
	 * @return Reference to the string.
	 */
	STORMBYTE_CONFIG_PUBLIC std::string& operator<<(std::string& str, const Config& file);
}
