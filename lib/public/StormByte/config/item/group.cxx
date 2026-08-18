#include <StormByte/config/item/group.hxx>

using namespace StormByte::Config::Item;

Group::Group(const std::string& name) : Container(name) {}

Group::Group(std::string&& name) : Container(std::move(name)) {}

Base::PointerType Group::BeforeAdditionActions(Base::PointerType item, const StormByte::Config::OnExistingAction onexisting) {
	if (!item) {
		throw Exception("Tried to add a null pointer to Group!");
	}

	if (item->Type() != Type::Comment) {
		if (!item->Name().has_value())
			throw InvalidName("Groups require every item to have a name");
		else if (!item->IsNameValid()) {
			throw InvalidName("Invalid item name '{}' for Group (must start with a letter and contain only letters, digits or underscores)", item->Name().value());
		}

		const auto it = std::find_if(m_items.begin(), m_items.end(), [&item](const Base::PointerType& i) {
			return i->Type() != Type::Comment && i->Name().value() == item->Name().value();
		});
		if (it != m_items.end()) {
			switch (onexisting) {
				case OnExistingAction::Keep:
					return *it;
				case OnExistingAction::Overwrite:
					m_items.erase(it);
					break;
				case OnExistingAction::ThrowException:
					throw ItemNameAlreadyExists(*item->Name());
			}
		}
	}

	m_items.push_back(item);
	return m_items.back();
}