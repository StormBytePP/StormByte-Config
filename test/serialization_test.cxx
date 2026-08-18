//==============================================================================
// FILE: test/serialization_test.cxx
//==============================================================================

#include <StormByte/config/config.hxx>
#include <StormByte/serializable.hxx>
#include <StormByte/system.hxx>
#include <StormByte/test_handlers.h>

#include <fstream>

using namespace StormByte::Config;

int test_serialize_value_string() {
	Item::Value<std::string> value("test", "Hello, World!");
	StormByte::Serializable<Item::Value<std::string>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();
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
	std::vector<std::byte> buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Value<int>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_int", 1);
	}
	ASSERT_TRUE("test_serialize_value_int", value == expected_value.value());
	RETURN_TEST("test_serialize_value_int", 0);
}

int test_serialize_value_double() {
	Item::Value<double> value("test", 62.78);
	StormByte::Serializable<Item::Value<double>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Value<double>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_double", 1);
	}
	ASSERT_TRUE("test_serialize_value_double", value == expected_value.value());
	RETURN_TEST("test_serialize_value_double", 0);
}

int test_serialize_value_bool() {
	Item::Value<bool> value("test", true);
	StormByte::Serializable<Item::Value<bool>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Value<bool>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_bool", 1);
	}
	ASSERT_TRUE("test_serialize_value_bool", value == expected_value.value());
	RETURN_TEST("test_serialize_value_bool", 0);
}

int test_serialize_comment_single_bash() {
	Item::Comment<Item::CommentType::SingleLineBash> value("Single line comment in bash");
	StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineBash>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();
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
	std::vector<std::byte> buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineC>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_comment_single_C", 1);
	}
	ASSERT_TRUE("test_serialize_comment_single_C", value == expected_value.value());
	RETURN_TEST("test_serialize_comment_single_C", 0);
}

int test_serialize_comment_multi_C() {
	Item::Comment<Item::CommentType::MultiLineC> value("Multi line comment in C\n"
		"Another line\n"
		"And another one"
	);
	StormByte::Serializable<Item::Comment<Item::CommentType::MultiLineC>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();
	auto expected_value = StormByte::Serializable<Item::Comment<Item::CommentType::MultiLineC>>::Deserialize(buffer);
	if (!expected_value) {
		std::cerr << "Expecting value! " << expected_value.error()->what() << std::endl;
		RETURN_TEST("test_serialize_comment_multi_C", 1);
	}
	ASSERT_TRUE("test_serialize_comment_multi_C", value == expected_value.value());
	RETURN_TEST("test_serialize_comment_multi_C", 0);
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
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected_group = StormByte::Serializable<Item::Group>::Deserialize(buffer);
	if (!expected_group) {
		std::cerr << "Expecting value! " << expected_group.error()->what() << std::endl;
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
	std::vector<std::byte> buffer = serializable.Serialize();

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
	std::vector<std::byte> buffer = serializable.Serialize();

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
	std::vector<std::byte> buffer = serializable.Serialize();
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

		auto expected_cfg = StormByte::Serializable<Config>::Deserialize(buff);
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
		Config cfg_human_readable2;
		file >> cfg_human_readable2;
		file.close();

		ASSERT_EQUAL("test_config_binary_deserialize", cfg_human_readable, cfg_human_readable2);
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		RETURN_TEST("test_config_binary_deserialize", 1);
	}
	
	RETURN_TEST("test_config_binary_deserialize", 0);
}

// ===================== Binary serialization tests =====================

int test_serialize_value_binary() {
	std::vector<std::byte> data = {
		std::byte{0x48}, std::byte{0x65}, std::byte{0x6C},
		std::byte{0x6C}, std::byte{0x6F} // "Hello"
	};
	Item::Value<std::vector<std::byte>> value("test", data);

	StormByte::Serializable<Item::Value<std::vector<std::byte>>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected = StormByte::Serializable<Item::Value<std::vector<std::byte>>>::Deserialize(buffer);
	if (!expected) {
		std::cerr << "Expecting value! " << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_binary", 1);
	}
	ASSERT_TRUE("test_serialize_value_binary", value == expected.value());
	RETURN_TEST("test_serialize_value_binary", 0);
}

int test_serialize_value_binary_empty() {
	Item::Value<std::vector<std::byte>> value("empty", std::vector<std::byte>{});

	StormByte::Serializable<Item::Value<std::vector<std::byte>>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected = StormByte::Serializable<Item::Value<std::vector<std::byte>>>::Deserialize(buffer);
	if (!expected) {
		std::cerr << "Expecting value! " << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_binary_empty", 1);
	}
	ASSERT_TRUE("test_serialize_value_binary_empty", value == expected.value());
	ASSERT_EQUAL("test_serialize_value_binary_empty", 0u, (*expected.value()).size());
	RETURN_TEST("test_serialize_value_binary_empty", 0);
}

int test_serialize_value_binary_all_bytes() {
	// All possible byte values 0x00 - 0xFF
	std::vector<std::byte> data(256);
	for (std::size_t i = 0; i < 256; ++i)
		data[i] = static_cast<std::byte>(i);

	Item::Value<std::vector<std::byte>> value("all_bytes", data);

	StormByte::Serializable<Item::Value<std::vector<std::byte>>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected = StormByte::Serializable<Item::Value<std::vector<std::byte>>>::Deserialize(buffer);
	if (!expected) {
		std::cerr << "Expecting value! " << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_binary_all_bytes", 1);
	}
	ASSERT_TRUE("test_serialize_value_binary_all_bytes", value == expected.value());
	RETURN_TEST("test_serialize_value_binary_all_bytes", 0);
}

int test_serialize_value_binary_large() {
	// 10 KB of data
	std::vector<std::byte> data(10 * 1024);
	for (std::size_t i = 0; i < data.size(); ++i)
		data[i] = static_cast<std::byte>(i % 256);

	Item::Value<std::vector<std::byte>> value("large", std::move(data));

	StormByte::Serializable<Item::Value<std::vector<std::byte>>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected = StormByte::Serializable<Item::Value<std::vector<std::byte>>>::Deserialize(buffer);
	if (!expected) {
		std::cerr << "Expecting value! " << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_binary_large", 1);
	}
	ASSERT_TRUE("test_serialize_value_binary_large", value == expected.value());
	RETURN_TEST("test_serialize_value_binary_large", 0);
}

int test_serialize_group_with_binary() {
	Item::Group group("test");
	group.Add(Item::Value<std::string>("string", "Hello"));
	group.Add(Item::Value<int>("int", 42));

	std::vector<std::byte> bin = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};
	group.Add(Item::Value<std::vector<std::byte>>("binary", bin));

	group.Add(Item::Value<bool>("flag", true));

	StormByte::Serializable<Item::Group> serializable(group);
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected = StormByte::Serializable<Item::Group>::Deserialize(buffer);
	if (!expected) {
		std::cerr << "Expecting value! " << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_group_with_binary", 1);
	}
	ASSERT_TRUE("test_serialize_group_with_binary", group == expected.value());
	RETURN_TEST("test_serialize_group_with_binary", 0);
}

int test_serialize_list_with_binary() {
	Item::List list("test");
	list.Add(Item::Value<std::string>("text"));
	list.Add(Item::Value<int>(99));

	std::vector<std::byte> bin = {std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF}};
	list.Add(Item::Value<std::vector<std::byte>>(bin));

	list.Add(Item::Value<bool>(false));

	StormByte::Serializable<Item::List> serializable(list);
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected = StormByte::Serializable<Item::List>::Deserialize(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_list_with_binary", 1);
	}
	ASSERT_TRUE("test_serialize_list_with_binary", list == expected.value());
	RETURN_TEST("test_serialize_list_with_binary", 0);
}

int test_serialize_nested_with_binary() {
	Item::Group root("root");
	Item::Group nested("nested");

	std::vector<std::byte> secret = {
		std::byte{0x53}, std::byte{0x65}, std::byte{0x63},
		std::byte{0x72}, std::byte{0x65}, std::byte{0x74}
	};
	nested.Add(Item::Value<std::vector<std::byte>>("secret", secret));
	nested.Add(Item::Value<std::string>("name", "hidden"));

	root.Add(std::move(nested));
	root.Add(Item::Value<int>("version", 1));

	StormByte::Serializable<Item::Group> serializable(root);
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected = StormByte::Serializable<Item::Group>::Deserialize(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_nested_with_binary", 1);
	}
	ASSERT_TRUE("test_serialize_nested_with_binary", root == expected.value());
	RETURN_TEST("test_serialize_nested_with_binary", 0);
}

int test_shared_ptr_binary_serialize() {
	std::vector<std::byte> data = {std::byte{0xCA}, std::byte{0xFE}};
	std::shared_ptr<Item::Base> value = std::make_shared<Item::Value<std::vector<std::byte>>>("bin", data);

	StormByte::Serializable<std::shared_ptr<Item::Base>> serializable(value);
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected = StormByte::Serializable<std::shared_ptr<Item::Base>>::Deserialize(buffer);
	if (!expected) {
		std::cerr << "Expecting value! " << expected.error()->what() << std::endl;
		RETURN_TEST("test_shared_ptr_binary_serialize", 1);
	}
	ASSERT_TRUE("test_shared_ptr_binary_serialize", *value == *expected.value());
	RETURN_TEST("test_shared_ptr_binary_serialize", 0);
}

int test_serialize_binary_roundtrip_text_and_binary() {
	// Create via text form, then serialize to binary and back
	Config cfg;
	cfg << R"(
		data = b"SGVsbG8gV29ybGQ="
		empty = b""
		mixed = {
			bin = b"AQIDBA=="
			num = 42
		}
	)";

	StormByte::Serializable<Config> serializable(cfg);
	std::vector<std::byte> buffer = serializable.Serialize();

	auto expected = StormByte::Serializable<Config>::Deserialize(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_binary_roundtrip_text_and_binary", 1);
	}

	ASSERT_TRUE("test_serialize_binary_roundtrip_text_and_binary", cfg == expected.value());
	RETURN_TEST("test_serialize_binary_roundtrip_text_and_binary", 0);
}

int test_text_to_binary_to_text_roundtrip() {
	int result = 0;

	// 1. Original configuration in text form (with Base64 binary data)
	// Note: we use a double that serializes cleanly
	const std::string original_text =
		"name = \"StormByte\"\n"
		"version = 2\n"
		"enabled = true\n"
		"payload = b\"SGVsbG8gV29ybGQ=\"\n"
		"settings = {\n"
		"\ttimeout = 30\n"
		"\tsecret = b\"U2VjcmV0RGF0YQ==\"\n"
		"}\n"
		"items = [\n"
		"\t\"first\"\n"
		"\tb\"c2Vjb25k\"\n"
		"\t42\n"
		"]\n";

	try {
		// 2. Load from text
		Config cfg1;
		cfg1 << original_text;

		// 3. Serialize to binary
		StormByte::Serializable<Config> serializable(cfg1);
		std::vector<std::byte> binary_buffer = serializable.Serialize();

		// 4. Deserialize from binary
		auto expected = StormByte::Serializable<Config>::Deserialize(binary_buffer);
		if (!expected) {
			std::cerr << "Failed to deserialize binary: " << expected.error()->what() << std::endl;
			RETURN_TEST("test_text_to_binary_to_text_roundtrip", 1);
		}
		Config cfg2 = std::move(expected.value());

		// 5. Convert back to text
		std::string regenerated_text = static_cast<std::string>(cfg2);

		// 6. Compare both text representations
		ASSERT_EQUAL("test_text_to_binary_to_text_roundtrip", original_text, regenerated_text);

		// Extra: also verify that binary data was correctly recovered
		const auto& payload = cfg2["payload"].Value<std::vector<std::byte>>();
		std::string recovered(reinterpret_cast<const char*>(payload.data()), payload.size());
		ASSERT_EQUAL("test_text_to_binary_to_text_roundtrip", "Hello World", recovered);

		const auto& secret = cfg2["settings/secret"].Value<std::vector<std::byte>>();
		std::string recovered_secret(reinterpret_cast<const char*>(secret.data()), secret.size());
		ASSERT_EQUAL("test_text_to_binary_to_text_roundtrip", "SecretData", recovered_secret);

	} catch (const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}

	RETURN_TEST("test_text_to_binary_to_text_roundtrip", result);
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

	result += test_serialize_value_binary();
	result += test_serialize_value_binary_empty();
	result += test_serialize_value_binary_all_bytes();
	result += test_serialize_value_binary_large();
	result += test_serialize_group_with_binary();
	result += test_serialize_list_with_binary();
	result += test_serialize_nested_with_binary();
	result += test_shared_ptr_binary_serialize();
	result += test_serialize_binary_roundtrip_text_and_binary();
	result += test_text_to_binary_to_text_roundtrip();

	if (result == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << result << " tests failed." << std::endl;
	}
	return result;
}
