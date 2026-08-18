#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/item/base.hxx>
#include <StormByte/config/type.hxx>

#include <queue>
#include <span>
#include <vector>

/**
 * @namespace Item
 * @brief All the configuration item classes namespace
 */
namespace StormByte::Config::Item {
	/**
	 * @class Container
	 * @brief Represents a container that can hold multiple configuration items.
	 */
	class STORMBYTE_CONFIG_PUBLIC Container: public Base {
		public:
			/**
			 * @brief Constructs an empty Container.
			 */
			Container() = default;

			/**
			 * @brief Constructs a Container with the given name.
			 * @param name The name of the container.
			 */
			Container(const std::string& name);

			/**
			 * @brief Constructs a Container with the given name.
			 * @param name The name of the container.
			 */
			Container(std::string&& name);

			/**
			 * @brief Copy constructor.
			 * @param base Container to copy.
			 */
			Container(const Container& base) = default;

			/**
			 * @brief Move constructor.
			 * @param base Container to move.
			 */
			Container(Container&& base) noexcept = default;

			/**
			 * @brief Copy assignment operator.
			 * @param base Container to copy.
			 */
			Container& operator=(const Container& base) = default;

			/**
			 * @brief Move assignment operator.
			 * @param base Container to move.
			 */
			Container& operator=(Container&& base) noexcept = default;

			/**
			 * @brief Destructor.
			 */
			virtual ~Container() noexcept override = default;

			/**
			 * @brief Polymorphic equality comparison.
			 * @param other The other item to compare against.
			 * @return true if both containers have the same type, name and contents.
			 */
			bool Equals(const Base& other) const noexcept override;

			/**
			 * @brief Gets a reference to an item by index.
			 * @param index Index of the item.
			 * @throw OutOfBounds if index is out of bounds.
			 * @return Reference to the item.
			 */
			Base& operator[](const size_t& index);

			/**
			 * @brief Gets a const reference to an item by index.
			 * @param index Index of the item.
			 * @throw OutOfBounds if index is out of bounds.
			 * @return Const reference to the item.
			 */
			const Base& operator[](const size_t& index) const;

			/**
			 * @brief Gets a reference to an item by path.
			 * @param path Path to the item.
			 * @throw InvalidPath if path is invalid.
			 * @throw ItemNotFound if item is not found.
			 * @return Reference to the item.
			 */
			Base& operator[](const std::string& path);

			/**
			 * @brief Gets a const reference to an item by path.
			 * @param path Path to the item.
			 * @throw InvalidPath if path is invalid.
			 * @throw ItemNotFound if item is not found.
			 * @return Const reference to the item.
			 */
			inline const Base& operator[](const std::string& path) const {
				return LookUp(path);
			}

			/**
			 * @brief Equality operator.
			 * @param container Container to compare.
			 * @return true if equal.
			 */
			bool operator==(const Container& container) const noexcept;

			/**
			 * @brief Inequality operator.
			 * @param container Container to compare.
			 * @return true if not equal.
			 */
			inline bool operator!=(const Container& container) const noexcept {
				return !operator==(container);
			}

			/**
			 * @brief Sets the action to take when an item with the same identity already exists.
			 * @param action The policy to apply on name/identity collision.
			 */
			void SetOnExistingAction(StormByte::Config::OnExistingAction action) noexcept {
				m_on_existing_action = action;
			}

			/**
			 * @brief Gets the current OnExistingAction policy.
			 * @return The current policy.
			 */
			StormByte::Config::OnExistingAction GetOnExistingAction() const noexcept {
				return m_on_existing_action;
			}

			/**
			 * @brief Adds an item to the container (const reference) using an explicit policy.
			 * @param item Item to add.
			 * @param on_existing Action to take if the item already exists.
			 * @return Reference to the added item.
			 */
			inline Base& Add(const Base& item, const StormByte::Config::OnExistingAction& on_existing) {
				return Add(std::move(*item.Clone()), on_existing);
			}

			/**
			 * @brief Adds an item to the container (rvalue) using an explicit policy.
			 * @param item Item to add.
			 * @param on_existing Action to take if the item already exists.
			 * @return Reference to the added item.
			 */
			inline Base& Add(Base&& item, const StormByte::Config::OnExistingAction& on_existing) {
				return Add(item.Move(), on_existing);
			}

			/**
			 * @brief Adds an item to the container using the container's current OnExistingAction policy.
			 * @param item Item to add.
			 * @return Reference to the added item.
			 */
			inline Base& Add(const Base& item) {
				return Add(std::move(*item.Clone()), m_on_existing_action);
			}

			/**
			 * @brief Adds an item to the container using the container's current OnExistingAction policy.
			 * @param item Item to add.
			 * @return Reference to the added item.
			 */
			inline Base& Add(Base&& item) {
				return Add(item.Move(), m_on_existing_action);
			}

			/**
			 * @brief Adds an item to the container.
			 * @param item The item to add.
			 * @param on_existing The action to take if the item already exists.
			 * @return A reference to the added item.
			 */
			Base& Add(Base::PointerType item, const StormByte::Config::OnExistingAction& on_existing);

			/**
			 * @brief Clears all items from the container.
			 */
			inline void Clear() noexcept {
				m_items.clear();
			}

			/**
			 * @brief Checks if an item exists by path.
			 * @param path Path to the item.
			 * @return true if the item exists.
			 */
			bool Exists(const std::string& path) const;

			/**
			 * @brief Removes an item by index.
			 * @param index Index of the item to remove.
			 * @throw OutOfBounds if index is out of bounds.
			 */
			void Remove(const size_t& index);

			/**
			 * @brief Removes an item by path.
			 * @param path Path to the item to remove.
			 * @throw InvalidPath if path is invalid.
			 * @throw ItemNotFound if item is not found.
			 */
			void Remove(const std::string& path);

			/**
			 * @brief Serializes the container to a string.
			 * @param indent_level Indentation level.
			 * @return Serialized string representation.
			 */
			std::string Serialize(const int& indent_level) const noexcept override;

			/**
			 * @brief Returns the enclosure characters for a given container type.
			 * @param type Container type.
			 * @return Pair of opening and closing characters.
			 */
			static constexpr std::pair<const char, const char> EnclosureCharacters(const ContainerType& type) noexcept {
				switch (type) {
					case ContainerType::Group: return {'{', '}'};
					case ContainerType::List:  return {'[', ']'};
					default:                   return {'\0', '\0'};
				}
			}

			/**
			 * @brief Returns the closing character for a given container type.
			 * @param type Container type.
			 * @return Closing character.
			 */
			static constexpr const char EndCharacter(const ContainerType& type) noexcept {
				switch (type) {
					case ContainerType::Group: return '}';
					case ContainerType::List:  return ']';
					default:                   return '\0';
				}
			}

			/**
			 * @brief Returns a span of all items in the container.
			 * @return Span of items.
			 */
			constexpr std::span<Base::PointerType> Items() noexcept {
				return std::span(m_items);
			}

			/**
			 * @brief Returns a const span of all items in the container.
			 * @return Const span of items.
			 */
			constexpr std::span<const Base::PointerType> Items() const noexcept {
				return std::span(m_items);
			}

			/**
			 * @brief Gets the container type.
			 * @return Container type.
			 */
			constexpr virtual Item::ContainerType ContainerType() const noexcept = 0;

			/**
			 * @brief Gets the container type as string.
			 * @return Container type as string.
			 */
			constexpr std::string ContainerTypeToString() const noexcept {
				return Item::TypeToString(this->ContainerType());
			}

			/**
			 * @brief Gets the item type.
			 * @return Item type.
			 */
			constexpr Item::Type Type() const noexcept override {
				return Item::Type::Container;
			}

			/**
			 * @brief Gets the number of items in the current level.
			 * @return Number of items.
			 */
			constexpr size_t Size() const noexcept {
				return m_items.size();
			}

			/**
			 * @brief Gets the total number of items including nested ones.
			 * @return Total number of items.
			 */
			size_t Count() const noexcept;

		protected:
			std::vector<Base::PointerType> m_items; ///< Items stored in the container
			StormByte::Config::OnExistingAction m_on_existing_action = StormByte::Config::OnExistingAction::ThrowException; ///< Policy for handling identity collisions

			/**
			 * @brief Actions performed before adding an item.
			 * @param item Item to check.
			 * @param onexisting Action to take if the item already exists.
			 * @return Pointer to the existing item if kept, otherwise nullptr.
			 */
			virtual Base::PointerType BeforeAdditionActions(Base::PointerType item, const StormByte::Config::OnExistingAction onexisting) = 0;

		private:
			/**
			 * @brief Internal helper that serializes the contents of the container.
			 * @param level Indentation level.
			 * @return Serialized contents.
			 */
			virtual std::string ContentsToString(const int& level) const noexcept;

			/**
			 * @brief Checks whether a path is syntactically valid.
			 * @param name Path to validate.
			 * @return true if the path is valid.
			 */
			static bool IsPathValid(const std::string& name) noexcept;

			/**
			 * @brief Looks up a child item by path.
			 * @param path Path to the child.
			 * @return Const reference to the found item.
			 */
			const Base& LookUp(const std::string& path) const;

			/**
			 * @brief Looks up a child item by path (queue version).
			 * @param path Path components.
			 * @return Const reference to the found item.
			 */
			const Base& LookUp(std::queue<std::string>& path) const;

			/**
			 * @brief Removes an item by path (queue version).
			 * @param path Path components.
			 */
			void Remove(std::queue<std::string>& path);
	};
}