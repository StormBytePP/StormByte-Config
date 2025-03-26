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
			List()												= default;

			/**
			 * @brief Constructs a List with the given name.
			 * @param name The name of the list.
			 */
			List(const std::string& name);

			/**
			 * @brief Constructs a List with the given name.
			 * @param name The name of the list.
			 */
			List(std::string&& name);

			/**
			 * Copy constructor
			 * @param list List to copy
			 */
			List(const List& list)								= default;

			/**
			 * Move constructor
			 * @param list List to move
			 */
			List(List&& list) noexcept							= default;

			/**
			 * Assignment operator
			 * @param list List to assign
			 */
			List& operator=(const List& list)					= default;

			/**
			 * Move assignment operator
			 * @param list List to move
			 */
			List& operator=(List&& list) noexcept				= default;

			/**
			 * Destructor
			 */
			~List() noexcept override							= default;

			/**
			 * Clones the list
			 * @return cloned list
			 */
			inline PointerType									Clone() const override {
				return MakePointer<List>(*this);
			}

			/**
			 * Moves the list
			 * @return moved list
			 */
			inline PointerType									Move() override {
				return MakePointer<List>(std::move(*this));
			}

			/**
			 * Gets the container type
			 * @return Container type
			 */
			constexpr Item::ContainerType						ContainerType() const noexcept override {
				return ContainerType::List;
			}

		private:
			/**
			 * Actions to be done before adding an item
			 * @param item item to check
			 * @param onexisting action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 */
			Base::PointerType									BeforeAdditionActions(Base::PointerType item, const OnExistingAction onexisting) override;
	};
}