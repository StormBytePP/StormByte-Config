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
	 * Checks if a name is valid
	 * @param name name to check
	 * @return is name valid?
	 */
	bool STORMBYTE_CONFIG_PUBLIC IsNameValid(const std::string&) noexcept;
	
	/**
	 * @class Base
	 * @brief The base class for all configuration items.
	 */
	class STORMBYTE_CONFIG_PUBLIC Base: public Clonable<Base, std::shared_ptr<Base>> {
		public:
			/**
			 * @brief Constructs a Base item with an optional name.
			 * @param name The name of the item.
			 */
			Base() 											= default;

			/**
			 * @brief Constructs a Base item with an optional name.
			 * @param name The name of the item.
			 */
			Base(const std::string& name);

			/**
			 * Copy constructor
			 * @param base item to copy
			 */
			Base(const Base& base) 							= default;
			
			/**
			 * Move constructor
			 * @param base item to move
			 */
			Base(Base&& base) noexcept						= default;

			/**
			 * Assignment operator
			 * @param base item to copy
			 */
			Base& operator=(const Base& base) 				= default;

			/**
			 * Move assignment operator
			 * @param base item to move
			 */
			Base& operator=(Base&& base) noexcept 			= default;

			/**
			 * Destructor
			 */
			virtual ~Base() noexcept 						= default;

			/**
			 * Equality operator
			 * @param other item to compare
			 * @return true if both items are equal
			 */
			bool operator==(const Base& other) const noexcept {
				return this->Equals(other);
			}

			/**
			 * Inequality operator
			 * @param other item to compare
			 * @return true if items are not equal
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
			constexpr const std::optional<std::string>& 	Name() const noexcept {
				return m_name;
			}

			/**
			 * Sets the item name
			 * @return item name
			 */
			constexpr void 									Name(const std::string& name) noexcept {
				m_name = name;
			}

			/**
			 * Checks if current name is valid
			 * @return is name valid?
			 */
			inline bool 									IsNameValid() const noexcept {
				return m_name.has_value() && Item::IsNameValid(m_name.value());
			}

			/**
			 * Gets the item name
			 * @return item name
			 */
			constexpr virtual Type 							Type() const noexcept = 0;

			/**
			 * @brief Returns the comment type if this item is a Comment, otherwise std::nullopt.
			 *
			 * This method is used by the serialization layer to determine the concrete
			 * Comment specialization (SingleLineBash, SingleLineC or MultiLineC)
			 * without relying on RTTI / dynamic_cast, which can fail under certain
			 * compilers and standard libraries (especially Clang + libc++).
			 *
			 * @return The CommentType if this is a comment, std::nullopt otherwise.
			 */
			virtual std::optional<CommentType> GetCommentType() const noexcept {
				return std::nullopt;
			}

			/**
			 * Gets the item type as string
			 * @return item type as string
			 */
			constexpr std::string							TypeToString() const noexcept {
				return Item::TypeToString(this->Type());
			}

			/**
			 * Gets the item name
			 * @return item name
			 */
			virtual std::string								Serialize(const int& indent_level) const noexcept;

			/**
			 * Converts current configuration to string
			 * @return configuration as string
			 */
			operator 										std::string() const {
				return this->Serialize(0);
			}

			/**
			 * Gets the item value
			 * @tparam T item value type
			 * @return item value
			 */
			template<typename T>
			const T& Value() const {
				if constexpr (std::is_base_of_v<std::remove_reference_t<decltype(*this)>, T>) {
					// Direct match: T (e.g., Container, Group, etc.) matches this object type
					return static_cast<const T&>(*this);
				} else if constexpr (std::is_base_of_v<std::remove_reference_t<decltype(*this)>, Item::Value<T>>) {
					// Indirect match: T is wrapped inside Item::Value<T>
					return *static_cast<const Item::Value<T>&>(*this);
				} else {
					// No valid match: Throw an exception for incorrect conversion
					throw WrongValueTypeConversion("Wrong value type {} while expecting {}", this->TypeToString(), typeid(T).name());
				}
			}
			
			/**
			 * Gets the item value
			 * @tparam T item value type
			 * @return item value
			 */
			template<typename T>
			T& Value() {
				return const_cast<T&>(static_cast<const Base&>(*this).Value<T>());
			}
				

		protected:
			std::optional<std::string> m_name;				///< Item name
	};
}