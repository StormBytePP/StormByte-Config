#include <StormByte/config/item/list.hxx>

using namespace StormByte::Config::Item;

List::List(const std::string& name) : Container(name) {}

List::List(std::string&& name) : Container(std::move(name)) {}

Base::PointerType List::BeforeAdditionActions(Base::PointerType item, const StormByte::Config::OnExistingAction on_existing) {
	if (!item) {
		throw Exception("Tried to add a null pointer to List!");
	}
	
	if (item->Type() != Type::Comment) {
		if (item->Name().has_value()) {
			throw InvalidName("Lists do not support named items (received name '{}')", item->Name().value());
		}	

		auto item_iterator = std::find_if(
			m_items.begin(),
			m_items.end(),
			[&item](std::shared_ptr<Base>& i) {
				return *i == *item;
			}
		);
		if (item_iterator != m_items.end()) {
			switch (on_existing) {
				case OnExistingAction::Keep:
					return *item_iterator;
				case OnExistingAction::Overwrite: {
					m_items.erase(item_iterator);
					break;
				}
				case OnExistingAction::ThrowException:
					throw ItemAlreadyExists("Config item already exists in List");
			}
		}
	}

	m_items.push_back(item);
	return m_items.back();
}