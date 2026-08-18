#pragma once

#include <StormByte/config/item/base.hxx>

#include <vector>
#include <cstddef>

/**
 * @namespace Item
 * @brief All the configuration item classes namespace
 */
namespace StormByte::Config::Item {
	/**
	 * @class Value
	 * @brief Represents a configuration item with a value.
	 * @tparam T The type of the value (only std::string, int, double, bool or std::vector<std::byte> allowed).
	 */
	template<AllowedValueType T>
	class STORMBYTE_CONFIG_PUBLIC Value: public Base {
		public:
			/**
			 * @brief Constructs a Value with the given value.
			 * @param value The value of the item.
			 */
			Value(const T& value):Base(), m_value(value) {}

			template <typename U = T>
			Value(const char* value) requires std::is_same_v<U, std::string>
				: Base(), m_value(std::string(value)) {}

			template <typename U = T>
			Value(const char* name, const char* value) requires std::is_same_v<U, std::string>
				: Base(std::string(name)), m_value(std::string(value)) {}

			/**
			 * Move Constructor
			 * @param value item value
			 */
			Value(T&& value):Base(), m_value(std::move(value)) {}

			/**
			 * Constructor
			 * @param name item name
			 * @param value item value
			 */
			Value(const std::string& name, const T& value):Base(name), m_value(value) {}

			/**
			 * Move Constructor
			 * @param name item name
			 * @param value item value
			 */
			Value(std::string&& name, T&& value):Base(std::move(name)), m_value(std::move(value)) {}

			/**
			 * Constructor overload (for std::string only)
			 * @param name item name
			 * @param value item value
			 */
			Value(const std::string& name, const char* value) requires std::is_same_v<T, std::string>
				: Base(name), m_value(std::string(value)) {}

			/**
			 * Move Constructor overload (for std::string only)
			 * @param name item name
			 * @param value item value
			 */
			Value(std::string&& name, const char* value) requires std::is_same_v<T, std::string>
				: Base(std::move(name)), m_value(std::string(value)) {}

			/**
			 * Copy constructor
			 * @param single item to copy
			 */
			Value(const Value& single)						= default;

			/**
			 * Move constructor
			 * @param single item to move
			 */
			Value(Value&& single) noexcept					= default;

			/**
			 * Assignment operator
			 * @param single item to copy
			 */
			Value& operator=(const Value& single)			= default;

			/**
			 * Move assignment operator
			 * @param single item to move
			 */
			Value& operator=(Value&& single) noexcept		= default;

			/**
			 * Destructor
			 */
			virtual ~Value() noexcept override				= default;

			/**
			 * @brief Polymorphic equality comparison.
			 * @param other The other item to compare against.
			 * @return true if both items are of the same type and hold the same value.
			 */
			bool Equals(const Base& other) const noexcept override {
				if (this->Type() != other.Type())
					return false;
				if (this->Name() != other.Name())
					return false;
				const Value<T>& other_value = static_cast<const Value<T>&>(other);
				return m_value == other_value.m_value;
			}

			/**
			 * @brief Gets the type of the item.
			 * @return Item::Type The type of the item.
			 */
			constexpr virtual Item::Type Type() const noexcept override {
				if constexpr (std::is_same_v<T, std::string>) {
					return Item::Type::String;
				} else if constexpr (std::is_same_v<T, int>) {
					return Item::Type::Integer;
				} else if constexpr (std::is_same_v<T, double>) {
					return Item::Type::Double;
				} else if constexpr (std::is_same_v<T, bool>) {
					return Item::Type::Bool;
				} else if constexpr (std::is_same_v<T, std::vector<std::byte>>) {
					return Item::Type::Binary;
				}
			}

			/**
			 * @brief Checks if two Value objects are equal.
			 * @param single The Value object to compare.
			 * @return True if equal, false otherwise.
			 */
			bool operator==(const Value<T>& single) const noexcept {
				return Equals(single);
			}

			/**
			 * Inequality operator
			 * @param single item to compare
			 * @return is not equal?
			 */
			bool operator!=(const Value<T>& single) const noexcept {
				return !operator==(single);
			}

			/**
			 * Gets the item value
			 * @return item value
			 */
			T& operator*() noexcept {
				return m_value;
			}

			/**
			 * Gets the item value
			 * @return item value
			 */
			const T& operator*() const noexcept {
				return m_value;
			}

			/**
			 * @brief Serializes the item to a string.
			 * @param indent_level The indentation level for serialization.
			 * @return The serialized string.
			 */
			std::string Serialize(const int& indent_level) const noexcept override;

			/**
			 * Clones the item
			 * @return cloned item
			 */
			virtual PointerType Clone() const override {
				return MakePointer<Value<T>>(*this);
			}

			/**
			 * Moves the item
			 * @return moved item
			 */
			virtual PointerType Move() override {
				return MakePointer<Value<T>>(std::move(*this));
			}

		protected:
			T m_value;										///< The value of the item.
	};

	// Deduction guides
	Value(const char*) 				-> Value<std::string>;
	Value(const char*, const char*) -> Value<std::string>;
}