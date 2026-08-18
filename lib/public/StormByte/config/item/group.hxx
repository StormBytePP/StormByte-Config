#pragma once

#include <StormByte/config/item/container.hxx>

/**
 * @namespace Item
 * @brief All the configuration item classes namespace
 */
namespace StormByte::Config::Item {
	/**
	 * @class Group
	 * @brief Represents a group in a configuration file that can hold other items, subgroups, and sublists recursively.
	 * @code
	 * settings = {
	 *     username = "StormByte"
	 *     credit = 66.5
	 * }
	 * @endcode
	 */
	class STORMBYTE_CONFIG_PUBLIC Group: public Container {
		public:
			/**
			 * @brief Constructs an empty Group.
			 */
			Group() = default;

			/**
			 * @brief Constructs a Group with the given name.
			 * @param name The name of the group.
			 */
			Group(const std::string& name);

			/**
			 * @brief Constructs a Group with the given name.
			 * @param name The name of the group.
			 */
			Group(std::string&& name);

			/**
			 * Copy constructor
			 * @param group group to copy
			 */
			Group(const Group& group)							= default;

			/**
			 * Move constructor
			 * @param group group to move
			 */
			Group(Group&& group) noexcept						= default;

			/**
			 * Assignment operator
			 * @param group group to copy
			 */
			Group& operator=(const Group& group)				= default;

			/**
			 * Move assignment operator
			 * @param group group to move
			 */
			Group& operator=(Group&& group) noexcept			= default;

			/**
			 * Destructor
			 */
			~Group() noexcept override							= default;

			/**
			 * Clones the container
			 * @return cloned container
			 */
			inline PointerType									Clone() const override {
				return MakePointer<Group>(*this);
			}

			/**
			 * Moves the container
			 * @return moved container
			 */
			inline PointerType									Move() override {
				return MakePointer<Group>(std::move(*this));
			}

			/**
			 * Gets the container type
			 * @return Container type
			 */
			constexpr Item::ContainerType						ContainerType() const noexcept override {
				return ContainerType::Group;
			}

		private:
			/**
			 * Actions to be done before adding an item
			 * @param item item to check
			 * @param onexisting action to take if item name already exists
			 * @throw ItemNameAlreadyExists if item name already exists
			 */
			Base::PointerType 									BeforeAdditionActions(Base::PointerType item, const StormByte::Config::OnExistingAction onexisting) override;
	};
}