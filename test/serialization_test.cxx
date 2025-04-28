#include <StormByte/config/config.hxx>
#include <StormByte/serializable.hxx>
#include <StormByte/system.hxx>
#include <StormByte/test_handlers.h>

#include <fstream>

using namespace StormByte::Config;

int test_serialize_value_string() {
	Item::Value<std::string> value("test", "Hello, World!");
	StormByte::Serializable<Item::Value<std::string>> serializable(value);
	StormByte::Buffer::Simple buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Value<std::string>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_string", 1);
	}
	ASSERT_TRUE("test_serialize_value_string", value == expected_value.value());
	RETURN_TEST("test_serialize_value_string", 0);
}

int test_serialize_value_int() {
	Item::Value<int> value("test", 62);
	StormByte::Serializable<Item::Value<int>> serializable(value);
	StormByte::Buffer::Simple buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Value<int>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_string", 1);
	}
	ASSERT_TRUE("test_serialize_value_string", value == expected_value.value());
	RETURN_TEST("test_serialize_value_string", 0);
}

int test_serialize_value_double() {
	Item::Value<double> value("test", 62.78);
	StormByte::Serializable<Item::Value<double>> serializable(value);
	StormByte::Buffer::Simple buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Value<double>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_string", 1);
	}
	ASSERT_TRUE("test_serialize_value_string", value == expected_value.value());
	RETURN_TEST("test_serialize_value_string", 0);
}

int test_serialize_value_bool() {
	Item::Value<bool> value("test", true);
	StormByte::Serializable<Item::Value<bool>> serializable(value);
	StormByte::Buffer::Simple buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Value<bool>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_string", 1);
	}
	ASSERT_TRUE("test_serialize_value_string", value == expected_value.value());
	RETURN_TEST("test_serialize_value_string", 0);
}

int test_serialize_comment_single_bash() {
	Item::Comment<Item::CommentType::SingleLineBash> value("Single line comment in bash");
	StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineBash>> serializable(value);
	StormByte::Buffer::Simple buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineBash>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_comment_single_bash", 1);
	}
	ASSERT_TRUE("test_serialize_comment_single_bash", value == expected_value.value());
	RETURN_TEST("test_serialize_comment_single_bash", 0);
}

int test_serialize_comment_single_C() {
	Item::Comment<Item::CommentType::SingleLineC> value("Single line comment in C");
	StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineC>> serializable(value);
	StormByte::Buffer::Simple buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineC>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_comment_single_bash", 1);
	}
	ASSERT_TRUE("test_serialize_comment_single_bash", value == expected_value.value());
	RETURN_TEST("test_serialize_comment_single_bash", 0);
}

int test_serialize_comment_multi_C() {
	Item::Comment<Item::CommentType::MultiLineC> value("Multi line comment in C\n"
		"Another line\n"
		"And another one"
	);
	StormByte::Serializable<Item::Comment<Item::CommentType::MultiLineC>> serializable(value);
	StormByte::Buffer::Simple buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Comment<Item::CommentType::MultiLineC>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_comment_single_bash", 1);
	}
	ASSERT_TRUE("test_serialize_comment_single_bash", value == expected_value.value());
	RETURN_TEST("test_serialize_comment_single_bash", 0);
}

int test_serialize_group() {
	Item::Group group("test");
	group.Add(Item::Value<std::string>("string", "Hello, World!"));
	group.Add(Item::Value<int>("int", 62));
	group.Add(Item::Value<double>("double", 62.78));
	group.Add(Item::Value<bool>("bool", true));
	group.Add(Item::Comment<Item::CommentType::SingleLineBash>("Single line comment in bash"));
	group.Add(Item::Comment<Item::CommentType::SingleLineC>("Single line comment in C"));
	group.Add(Item::Comment<Item::CommentType::MultiLineC>("Multi line comment in C\n"
		"Another line\n"
		"And another one"
	));
	StormByte::Serializable<Item::Group> serializable(group);
	StormByte::Buffer::Simple buffer = serializable.Serialize();

	auto expected_group = StormByte::Serializable<Item::Group>::Deserialize(buffer);
	if (!expected_group) {
		std::cerr << expected_group.error()->what() << std::endl;
		RETURN_TEST("test_serialize_group", 1);
	}
	ASSERT_TRUE("test_serialize_group", group == expected_group.value());
	RETURN_TEST("test_serialize_group", 0);
}

int test_serialize_list() {
	Item::List list("test");
	list.Add(Item::Value<std::string>("Hello, World!"));
	list.Add(Item::Value<int>(62));
	list.Add(Item::Value<double>(62.78));
	list.Add(Item::Value<bool>(true));
	list.Add(Item::Comment<Item::CommentType::SingleLineBash>("Single line comment in bash"));
	list.Add(Item::Comment<Item::CommentType::SingleLineC>("Single line comment in C"));
	list.Add(Item::Comment<Item::CommentType::MultiLineC>("Multi line comment in C\n"
		"Another line\n"
		"And another one"
	));
	StormByte::Serializable<Item::List> serializable(list);
	StormByte::Buffer::Simple buffer = serializable.Serialize();

	auto expected_list = StormByte::Serializable<Item::List>::Deserialize(buffer);
	if (!expected_list) {
		std::cerr << expected_list.error()->what() << std::endl;
		RETURN_TEST("test_serialize_list", 1);
	}
	ASSERT_TRUE("test_serialize_list", list == expected_list.value());
	RETURN_TEST("test_serialize_list", 0);
}

int test_serialize_nested_groups() {
	Item::Group group("test");
	group.Add(Item::Value<std::string>("string", "Hello, World!"));
	group.Add(Item::Value<int>("int", 62));
	group.Add(Item::Value<double>("double", 62.78));
	group.Add(Item::Value<bool>("bool", true));
	group.Add(Item::Comment<Item::CommentType::SingleLineBash>("Single line comment in bash"));
	group.Add(Item::Comment<Item::CommentType::SingleLineC>("Single line comment in C"));
	group.Add(Item::Comment<Item::CommentType::MultiLineC>("Multi line comment in C\n"
		"Another line\n"
		"And another one"
	));
	Item::Group nested_group("nested");
	nested_group.Add(Item::Value<std::string>("string", "Hello, World!"));
	nested_group.Add(Item::Value<int>("int", 62));
	nested_group.Add(Item::Value<double>("double", 62.78));
	nested_group.Add(Item::Value<bool>("bool", true));
	nested_group.Add(Item::Comment<Item::CommentType::SingleLineBash>("Single line comment in bash"));
	nested_group.Add(Item::Comment<Item::CommentType::SingleLineC>("Single line comment in C"));
	nested_group.Add(Item::Comment<Item::CommentType::MultiLineC>("Multi line comment in C\n"
		"Another line\n"
		"And another one"
	));
	group.Add(nested_group);
	StormByte::Serializable<Item::Group> serializable(group);
	StormByte::Buffer::Simple buffer = serializable.Serialize();

	auto expected_group = StormByte::Serializable<Item::Group>::Deserialize(buffer);
	if (!expected_group) {
		std::cerr << expected_group.error()->what() << std::endl;
		RETURN_TEST("test_serialize_nested_groups", 1);
	}
	ASSERT_TRUE("test_serialize_nested_groups", group == expected_group.value());
	RETURN_TEST("test_serialize_nested_groups", 0);
}

int test_shared_ptr_string_serialize() {
	std::shared_ptr<Item::Base> value = std::make_shared<Item::Value<std::string>>("test", "Hello, World!");
	StormByte::Serializable<std::shared_ptr<Item::Base>> serializable(value);
	StormByte::Buffer::Simple buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<std::shared_ptr<Item::Base>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_shared_ptr_string_serialize", 1);
	}
	ASSERT_TRUE("test_shared_ptr_string_serialize", *value == *expected_value.value());
	RETURN_TEST("test_shared_ptr_string_serialize", 0);
}

int test_comment_serialize() {
	Item::Comment<Item::CommentType::SingleLineBash> valueSB("SB");
	Item::Comment<Item::CommentType::SingleLineC> valueSC("SC");
	Item::Comment<Item::CommentType::MultiLineC> valueMC("MC");
	std::shared_ptr<Item::Base> valueSB_ptr = std::make_shared<Item::Comment<Item::CommentType::SingleLineBash>>(valueSB);
	std::shared_ptr<Item::Base> valueSC_ptr = std::make_shared<Item::Comment<Item::CommentType::SingleLineC>>(valueSC);
	std::shared_ptr<Item::Base> valueMC_ptr = std::make_shared<Item::Comment<Item::CommentType::MultiLineC>>(valueMC);
	StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineBash>> serializableSB(valueSB);
	StormByte::Serializable<std::shared_ptr<Item::Base>> serializableSB_ptr(valueSB_ptr);
	StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineC>> serializableSC(valueSC);
	StormByte::Serializable<std::shared_ptr<Item::Base>> serializableSC_ptr(valueSC_ptr);
	StormByte::Serializable<Item::Comment<Item::CommentType::MultiLineC>> serializableMC(valueMC);
	StormByte::Serializable<std::shared_ptr<Item::Base>> serializableMC_ptr(valueMC_ptr);

	// std::cout << "Single Bash: " << std::endl << serializableSB.Serialize().HexData() << std::endl;
	// std::cout << "Single Bash PTR: " << std::endl << serializableSB_ptr.Serialize().HexData() << std::endl;
	// std::cout << "Single C: " << std::endl << serializableSC.Serialize().HexData() << std::endl;
	// std::cout << "Single C PTR: " << std::endl << serializableSC_ptr.Serialize().HexData() << std::endl;
	// std::cout << "Multi C: " << std::endl << serializableMC.Serialize().HexData() << std::endl;
	// std::cout << "Multi C PTR: " << std::endl << serializableMC_ptr.Serialize().HexData() << std::endl;

	auto expected_valueSB = StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineBash>>::Deserialize(serializableSB.Serialize());
	auto expected_valueSB_ptr = StormByte::Serializable<std::shared_ptr<Item::Base>>::Deserialize(serializableSB_ptr.Serialize());
	auto expected_valueSC = StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineC>>::Deserialize(serializableSC.Serialize());
	auto expected_valueSC_ptr = StormByte::Serializable<std::shared_ptr<Item::Base>>::Deserialize(serializableSC_ptr.Serialize());
	auto expected_valueMC = StormByte::Serializable<Item::Comment<Item::CommentType::MultiLineC>>::Deserialize(serializableMC.Serialize());
	auto expected_valueMC_ptr = StormByte::Serializable<std::shared_ptr<Item::Base>>::Deserialize(serializableMC_ptr.Serialize());

	if (!expected_valueSB) {
		std::cerr << "Expecting value! " << expected_valueSB.error()->what() << std::endl;
		RETURN_TEST("test_comment_serialize", 1);
	}
	else if (!expected_valueSB_ptr) {
		std::cerr << "Expecting value! " << expected_valueSB_ptr.error()->what() << std::endl;
		RETURN_TEST("test_comment_serialize", 1);
	}
	else if (!expected_valueSC) {
		std::cerr << "Expecting value! " << expected_valueSC.error()->what() << std::endl;
		RETURN_TEST("test_comment_serialize", 1);
	}
	else if (!expected_valueSC_ptr) {
		std::cerr << "Expecting value! " << expected_valueSC_ptr.error()->what() << std::endl;
		RETURN_TEST("test_comment_serialize", 1);
	}
	else if (!expected_valueMC) {
		std::cerr << "Expecting value! " << expected_valueMC.error()->what() << std::endl;
		RETURN_TEST("test_comment_serialize", 1);
	}
	else if (!expected_valueMC_ptr) {
		std::cerr << "Expecting value! " << expected_valueMC_ptr.error()->what() << std::endl;
		RETURN_TEST("test_comment_serialize", 1);
	}

	RETURN_TEST("test_comment_serialize", 0);
}

int test_config_binary_deserialize() {
	Config cfg_human_readable, cfg_binary;
	try {
		std::fstream file;
		const std::filesystem::path binary_file = CurrentFileDirectory / "files" / "config_example.bin";
		const std::filesystem::path human_readable_file = CurrentFileDirectory / "files" / "complex_conf1.conf";
		file.open(binary_file, std::ios::binary | std::ios::ate | std::ios::in);

		if (!file.is_open()) {
			std::cerr << "Can't open file " << binary_file.string().c_str() << std::endl;
			RETURN_TEST("test_config_binary_deserialize", 1);
		}
		// Determine the file size
		std::streamsize file_size = file.tellg();
		file.seekg(0, std::ios::beg);
	
		// Create a vector of the appropriate size
		std::vector<std::byte> buff(static_cast<std::size_t>(file_size));

		// Read the file content into the vector
		if (!file.read(reinterpret_cast<char*>(buff.data()), file_size)) {
			std::cerr << "Failed to read file" << std::endl;
			RETURN_TEST("test_config_binary_deserialize", 1);
		}
		file.close();

		StormByte::Buffer::Simple buffer(buff);
		auto expected_cfg = StormByte::Serializable<Config>::Deserialize(buffer);
		if (!expected_cfg) {
			std::cerr << expected_cfg.error()->what() << std::endl;
			RETURN_TEST("test_config_binary_deserialize", 1);
		}
		else {
			cfg_human_readable = std::move(expected_cfg.value());
		}

		// Now read the file in human readable form to compare
		file.open(human_readable_file, std::ios::in);
		if (!file.is_open()) {
			std::cerr << "Can't open file " << human_readable_file.string().c_str() << std::endl;
			RETURN_TEST("test_config_binary_deserialize", 1);
		}
		Config cfg_human_readable;
		file >> cfg_human_readable;
		file.close();

		ASSERT_EQUAL("test_config_binary_deserialize", cfg_human_readable, cfg_human_readable);
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		RETURN_TEST("test_config_binary_deserialize", 1);
	}
	
	RETURN_TEST("test_config_serialize", 0);
}

int main() {
    int result = 0;
	result += test_serialize_value_string();
	result += test_serialize_value_int();
	result += test_serialize_value_double();
	result += test_serialize_value_bool();
	result += test_serialize_comment_single_bash();
	result += test_serialize_comment_single_C();
	result += test_serialize_comment_multi_C();
	result += test_serialize_group();
	result += test_serialize_list();
	result += test_serialize_nested_groups();
	result += test_shared_ptr_string_serialize();
	result += test_comment_serialize();
	result += test_config_binary_deserialize();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}