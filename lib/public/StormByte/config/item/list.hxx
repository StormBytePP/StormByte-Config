#pragma once

#include <StormByte/config/item/container.hxx>

/**
 * @namespace Item
 * @brief All the configuration item classes namespace
 */
namespace StormByte::Config::Item {
	/**
	 * @class List
	 * @brief Represents a list in a configuration file that can hold other items, subgroups, and sublists recursively.
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
