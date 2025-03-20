#include <StormByte/config/exception.hxx>
#include <StormByte/config/item/type.hxx>

#include <format>

using namespace StormByte::Config;

Exception::Exception(const std::string& msg):StormByte::Exception(msg) {}

WrongValueTypeConversion::WrongValueTypeConversion(const std::string& srctype, const std::string& dsttype):
Exception(std::format("Conversion failed from {} to {}", srctype, dsttype)) {}

ValueFailure::ValueFailure(const std::string& srctype, const std::string& dsttype):
Exception(std::format("Try to add/set {} value which is of type {}", dsttype, srctype)) {}

InvalidName::InvalidName():
Exception("Empty names are not allowed in Group") {}

InvalidName::InvalidName(const std::string& name, const std::string& containertype):
Exception(containertype == "Group" ?
	std::format("Invalid name {} given as identifier, only alfanumeric is allowed", name) :
	std::format("Lists do not accept named items (given {})", name)) {}

InvalidPath::InvalidPath(const std::string& path):
Exception(std::format("Invalid path {} given", path)) {}

ParseError::ParseError(const unsigned int& line, const std::string& reason):
Exception(std::format("Parse error on line {}: {}", std::to_string(line), reason)) {}

ItemNotFound::ItemNotFound(const std::string& name):
Exception(std::format("Item {} do not exist", name)) {}

ItemAlreadyExists::ItemAlreadyExists():
Exception("Another item with the same contents already exists") {}

ItemNameAlreadyExists::ItemNameAlreadyExists(const std::string& name):
Exception(std::format("Another item with name {} already exists", name)) {}

OutOfBounds::OutOfBounds(const size_t& index, const size_t& size):
Exception(std::format("Index {} is out of bounds when size is {}", std::to_string(index), std::to_string(size))) {}