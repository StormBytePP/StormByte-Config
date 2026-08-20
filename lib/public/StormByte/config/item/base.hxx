#pragma once

#include <StormByte/config/exception.hxx>
#include <StormByte/config/item/type.hxx>
#include <StormByte/clonable.hxx>

#include <optional>
#include <string>
#include <type_traits>

namespace StormByte::Config::Item {
	// Forwards
	class Container;
	class Group;
	class List;
	template<AllowedValueType T> class Value;

	/**
	 * @brief Checks if a name is valid.
	 * @param name Name to check.
	 * @return true if the name is valid.
	 */
	bool STORMBYTE_CONFIG_PUBLIC IsNameValid(const std::string&) noexcept;

	/**
	 * @class Base
	 * @brief The base class for all configuration items.
	 */
	class STORMBYTE_CONFIG_PUBLIC Base: public Clonable<Base, std::shared_ptr<Base>> {
		public:
			/**
			 * @brief Default constructor.
			 */
			Base() = default;

			/**
			 * @brief Constructs a Base item with a name.
			 * @param name The name of the item.
			 */
			Base(const std::string& name);

			/**
			 * @brief Copy constructor.
			 * @param base Item to copy.
			 */
			Base(const Base& base) = default;

			/**
			 * @brief Move constructor.
			 * @param base Item to move.
			 */
			Base(Base&& base) noexcept = default;

			/**
			 * @brief Copy assignment operator.
			 * @param base Item to copy.
			 * @return Reference to this Base.
			 */
			Base& operator=(const Base& base) = default;

			/**
			 * @brief Move assignment operator.
			 * @param base Item to move.
			 * @return Reference to this Base.
			 */
			Base& operator=(Base&& base) noexcept = default;

			/**
			 * @brief Destructor.
			 */
			virtual ~Base() noexcept = default;

			/**
			 * @brief Equality operator.
			 * @param other Item to compare.
			 * @return true if both items are equal.
			 */
			bool operator==(const Base& other) const noexcept {
				return this->Equals(other);
			}

			/**
			 * @brief Inequality operator.
			 * @param other Item to compare.
			 * @return true if items are not equal.
			 */
			bool operator!=(const Base& other) const noexcept {
				return !(*this == other);
			}

			/**
			 * @brief Polymorphic equality comparison.
			 * @param other The other item to compare against.
			 * @return true if both items are semantically equal.
			 */
			virtual bool Equals(const Base& other) const noexcept = 0;

			/**
			 * @brief Gets the name of the item.
			 * @return The name of the item.
			 */
			constexpr const std::optional<std::string>& Name() const noexcept {
				return m_name;
			}

			/**
			 * @brief Sets the item name.
			 * @param name New name.
			 */
			constexpr void Name(const std::string& name) noexcept {
				m_name = name;
			}

			/**
			 * @brief Checks if the current name is valid.
			 * @return true if the name is valid.
			 */
			inline bool IsNameValid() const noexcept {
				return m_name.has_value() && Item::IsNameValid(m_name.value());
			}

			/**
			 * @brief Gets the item type.
			 * @return Item type.
			 */
			constexpr virtual Type Type() const noexcept = 0;

			/**
			 * @brief Returns the comment type if this item is a Comment, otherwise std::nullopt.
			 *
			 * Used by the serialization layer to determine the concrete Comment specialization
			 * without relying on RTTI / dynamic_cast.
			 *
			 * @return The CommentType if this is a comment, std::nullopt otherwise.
			 */
			virtual std::optional<CommentType> GetCommentType() const noexcept {
				return std::nullopt;
			}

			/**
			 * @brief Gets the item type as a string.
			 * @return Item type as string.
			 */
			constexpr std::string TypeToString() const noexcept {
				return Item::TypeToString(this->Type());
			}

			/**
			 * @brief Serializes the item.
			 * @param indent_level Indentation level.
			 * @return Serialized string.
			 */
			virtual std::string Serialize(const int& indent_level) const noexcept;

			/**
			 * @brief Converts the item to a string.
			 * @return Serialized representation.
			 */
			operator std::string() const {
				return this->Serialize(0);
			}

			/**
			 * @brief Gets the item value (const).
			 * @tparam T Target value type.
			 * @return Const reference to the value.
			 * @throw WrongValueTypeConversion on type mismatch.
			 */
			template<typename T>
			const T& Value() const {
				if constexpr (std::is_base_of_v<std::remove_reference_t<decltype(*this)>, T>) {
					return static_cast<const T&>(*this);
				} else if constexpr (std::is_base_of_v<std::remove_reference_t<decltype(*this)>, Item::Value<T>>) {
					return *static_cast<const Item::Value<T>&>(*this);
				} else {
					throw WrongValueTypeConversion("Wrong value type {} while expecting {}", this->TypeToString(), typeid(T).name());
				}
			}

			/**
			 * @brief Gets the item value (mutable).
			 * @tparam T Target value type.
			 * @return Reference to the value.
			 * @throw WrongValueTypeConversion on type mismatch.
			 */
			template<typename T>
			T& Value() {
				return const_cast<T&>(static_cast<const Base&>(*this).Value<T>());
			}

		protected:
			std::optional<std::string> m_name; ///< Item name
	};
}