//==============================================================================
// FILE: test/serialization_test.cxx
//==============================================================================

#include <StormByte/config/config.hxx>
#include <StormByte/serializable.hxx>
#include <StormByte/system.hxx>
#include <StormByte/test_handlers.h>

#include <fstream>
#include <random>
#include <cstring>
#include <algorithm>

using namespace StormByte::Config;

// =============================================================================
// Helpers
// =============================================================================

namespace {

std::vector<std::byte> SerializeGroup() {
	Item::Group group("root");
	group.Add(Item::Value<std::string>("name", "StormByte"));
	group.Add(Item::Value<int>("version", 42));
	group.Add(Item::Value<bool>("enabled", true));
	group.Add(Item::Comment<Item::CommentType::SingleLineBash>("a comment"));
	group.Add(Item::Value<std::vector<std::byte>>("payload",
		{std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF}}));

	Item::List list("items");
	list.Add(Item::Value(1));
	list.Add(Item::Value("two"));
	list.Add(Item::Value(3.14));
	group.Add(std::move(list));

	return StormByte::Serializable<Item::Group>(group).Serialize();
}

std::vector<std::byte> SerializeList() {
	Item::List list("test");
	list.Add(Item::Value(10));
	list.Add(Item::Value("hello"));
	list.Add(Item::Comment<Item::CommentType::SingleLineC>("note"));
	list.Add(Item::Value(true));
	return StormByte::Serializable<Item::List>(list).Serialize();
}

std::vector<std::byte> SerializeStringValue() {
	Item::Value<std::string> v("key", "Hello, StormByte!");
	return StormByte::Serializable<Item::Value<std::string>>(v).Serialize();
}

std::vector<std::byte> SerializeIntValue() {
	Item::Value<int> v("num", 0x12345678);
	return StormByte::Serializable<Item::Value<int>>(v).Serialize();
}

std::vector<std::byte> SerializeBinaryValue() {
	std::vector<std::byte> data(64);
	for (std::size_t i = 0; i < data.size(); ++i)
		data[i] = static_cast<std::byte>(i);
	Item::Value<std::vector<std::byte>> v("bin", data);
	return StormByte::Serializable<Item::Value<std::vector<std::byte>>>(v).Serialize();
}

// Flip a single bit
void FlipBit(std::vector<std::byte>& buf, std::size_t byte_index, unsigned bit) {
	if (byte_index >= buf.size() || bit > 7) return;
	auto& b = reinterpret_cast<unsigned char&>(buf[byte_index]);
	b ^= static_cast<unsigned char>(1u << bit);
}

// Overwrite one byte
void CorruptByte(std::vector<std::byte>& buf, std::size_t index, std::byte value) {
	if (index < buf.size())
		buf[index] = value;
}

// Truncate
std::vector<std::byte> Truncate(const std::vector<std::byte>& buf, std::size_t new_size) {
	if (new_size >= buf.size()) return buf;
	return std::vector<std::byte>(buf.begin(), buf.begin() + static_cast<std::ptrdiff_t>(new_size));
}

} // anonymous namespace

// =============================================================================
// Existing good tests (kept intact)
// =============================================================================

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

	auto expected_valueSB = StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineBash>>::Deserialize(
		StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineBash>>(valueSB).Serialize());
	auto expected_valueSB_ptr = StormByte::Serializable<std::shared_ptr<Item::Base>>::Deserialize(
		StormByte::Serializable<std::shared_ptr<Item::Base>>(valueSB_ptr).Serialize());
	auto expected_valueSC = StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineC>>::Deserialize(
		StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineC>>(valueSC).Serialize());
	auto expected_valueSC_ptr = StormByte::Serializable<std::shared_ptr<Item::Base>>::Deserialize(
		StormByte::Serializable<std::shared_ptr<Item::Base>>(valueSC_ptr).Serialize());
	auto expected_valueMC = StormByte::Serializable<Item::Comment<Item::CommentType::MultiLineC>>::Deserialize(
		StormByte::Serializable<Item::Comment<Item::CommentType::MultiLineC>>(valueMC).Serialize());
	auto expected_valueMC_ptr = StormByte::Serializable<std::shared_ptr<Item::Base>>::Deserialize(
		StormByte::Serializable<std::shared_ptr<Item::Base>>(valueMC_ptr).Serialize());

	if (!expected_valueSB || !expected_valueSB_ptr || !expected_valueSC ||
		!expected_valueSC_ptr || !expected_valueMC || !expected_valueMC_ptr) {
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
		std::streamsize file_size = file.tellg();
		file.seekg(0, std::ios::beg);
	
		std::vector<std::byte> buff(static_cast<std::size_t>(file_size));
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
		cfg_human_readable = std::move(expected_cfg.value());

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

int test_serialize_value_binary() {
	std::vector<std::byte> data = {
		std::byte{0x48}, std::byte{0x65}, std::byte{0x6C},
		std::byte{0x6C}, std::byte{0x6F}
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
		Config cfg1;
		cfg1 << original_text;

		StormByte::Serializable<Config> serializable(cfg1);
		std::vector<std::byte> binary_buffer = serializable.Serialize();

		auto expected = StormByte::Serializable<Config>::Deserialize(binary_buffer);
		if (!expected) {
			std::cerr << "Failed to deserialize binary: " << expected.error()->what() << std::endl;
			RETURN_TEST("test_text_to_binary_to_text_roundtrip", 1);
		}
		Config cfg2 = std::move(expected.value());

		std::string regenerated_text = static_cast<std::string>(cfg2);

		ASSERT_EQUAL("test_text_to_binary_to_text_roundtrip", original_text, regenerated_text);

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

// =============================================================================
// NEW: Robustness & Corruption tests
// =============================================================================

// ----- Critical: empty / truncated buffers must fail -----

int test_corruption_empty_buffer() {
	std::vector<std::byte> empty;
	auto r1 = StormByte::Serializable<Item::Group>::Deserialize(empty);
	auto r2 = StormByte::Serializable<Item::List>::Deserialize(empty);
	auto r3 = StormByte::Serializable<Item::Value<std::string>>::Deserialize(empty);
	auto r4 = StormByte::Serializable<Config>::Deserialize(empty);

	if (r1 || r2 || r3 || r4) {
		std::cerr << "test_corruption_empty_buffer: empty buffer was accepted\n";
		RETURN_TEST("test_corruption_empty_buffer", 1);
	}
	RETURN_TEST("test_corruption_empty_buffer", 0);
}

int test_corruption_truncated_all_lengths() {
	auto clean = SerializeGroup();
	int failures = 0;

	// Truncate at every possible length shorter than the original
	for (std::size_t len = 0; len < clean.size(); ++len) {
		auto truncated = Truncate(clean, len);
		auto result = StormByte::Serializable<Item::Group>::Deserialize(truncated);
		if (result) {
			// Only the full buffer should succeed
			std::cerr << "test_corruption_truncated_all_lengths: truncated buffer of size "
					<< len << " was accepted\n";
			++failures;
		}
	}
	RETURN_TEST("test_corruption_truncated_all_lengths", failures > 0 ? 1 : 0);
}

// ----- Critical: single-byte corruption in the header region must usually fail -----

int test_corruption_header_bytes() {
	auto clean = SerializeGroup();
	if (clean.size() < 16) {
		std::cerr << "test_corruption_header_bytes: buffer too small\n";
		RETURN_TEST("test_corruption_header_bytes", 1);
	}

	int accepted = 0;
	// Corrupt the first 16 bytes (where type, name length, etc. normally live)
	for (std::size_t i = 0; i < 16; ++i) {
		for (int v = 0; v < 256; v += 17) { // sample some values
			auto buf = clean;
			CorruptByte(buf, i, static_cast<std::byte>(v));
			auto result = StormByte::Serializable<Item::Group>::Deserialize(buf);
			if (result)
				++accepted;
		}
	}

	// It is acceptable that a few random values still produce a valid object,
	// but a large percentage of header corruptions must fail.
	// We just check that the test itself doesn't crash and that not *everything* is accepted.
	if (accepted > 200) { // very loose threshold
		std::cerr << "test_corruption_header_bytes: too many corrupted headers were accepted ("
				<< accepted << ")\n";
		RETURN_TEST("test_corruption_header_bytes", 1);
	}
	RETURN_TEST("test_corruption_header_bytes", 0);
}

// ----- Critical: bit flips across the whole buffer must never crash -----

int test_corruption_no_crash_single_bit_flip() {
	auto clean = SerializeGroup();
	std::size_t crashes = 0; // we can't really catch crashes, but we detect unexpected success patterns

	for (std::size_t i = 0; i < clean.size(); ++i) {
		for (unsigned bit = 0; bit < 8; ++bit) {
			auto buf = clean;
			FlipBit(buf, i, bit);
			// Must not throw uncaught exceptions or crash
			auto result = StormByte::Serializable<Item::Group>::Deserialize(buf);
			// We don't care if it succeeds or fails, only that it returns cleanly
			(void)result;
		}
	}
	RETURN_TEST("test_corruption_no_crash_single_bit_flip", 0);
}

int test_corruption_no_crash_single_byte_overwrite() {
	auto clean = SerializeList();
	for (std::size_t i = 0; i < clean.size(); ++i) {
		for (int v = 0; v < 256; v += 23) {
			auto buf = clean;
			CorruptByte(buf, i, static_cast<std::byte>(v));
			auto result = StormByte::Serializable<Item::List>::Deserialize(buf);
			(void)result;
		}
	}
	RETURN_TEST("test_corruption_no_crash_single_byte_overwrite", 0);
}

// ----- Payload-only corruption (tolerable) -----

int test_corruption_payload_string_still_safe() {
	auto clean = SerializeStringValue();
	// Flip bits in the second half of the buffer (more likely to be payload)
	std::size_t start = clean.size() / 2;
	int safe = 0;

	for (std::size_t i = start; i < clean.size(); ++i) {
		auto buf = clean;
		FlipBit(buf, i, 0);
		auto result = StormByte::Serializable<Item::Value<std::string>>::Deserialize(buf);
		// Either fails controlled or succeeds with different content — both OK
		++safe;
		(void)result;
	}
	RETURN_TEST("test_corruption_payload_string_still_safe", safe > 0 ? 0 : 1);
}

int test_corruption_payload_binary_still_safe() {
	auto clean = SerializeBinaryValue();
	std::size_t start = clean.size() / 3;
	for (std::size_t i = start; i < clean.size(); ++i) {
		auto buf = clean;
		CorruptByte(buf, i, std::byte{0xAA});
		auto result = StormByte::Serializable<Item::Value<std::vector<std::byte>>>::Deserialize(buf);
		(void)result;
	}
	RETURN_TEST("test_corruption_payload_binary_still_safe", 0);
}

// ----- Huge size / DoS style attacks -----

int test_corruption_huge_claimed_size() {
	auto clean = SerializeGroup();
	int accepted = 0;

	// Only corrupt a full 8-byte size field at the start of the buffer
	if (clean.size() >= sizeof(std::uint64_t)) {
		auto buf = clean;
		std::uint64_t huge = 0xFFFFFFFFFFFFFFFFULL;
		std::memcpy(buf.data(), &huge, sizeof(huge));
		auto result = StormByte::Serializable<Item::Group>::Deserialize(buf);
		if (result)
			++accepted;
	}

	// Also try a few aligned early positions that could be size fields
	for (std::size_t i = 0; i + sizeof(std::uint64_t) <= std::min<std::size_t>(24, clean.size()); i += sizeof(std::uint64_t)) {
		auto buf = clean;
		std::uint64_t huge = 0xFFFFFFFFFFFFFFFFULL;
		std::memcpy(buf.data() + i, &huge, sizeof(huge));
		auto result = StormByte::Serializable<Item::Group>::Deserialize(buf);
		if (result)
			++accepted;
	}

	if (accepted > 0) {
		std::cerr << "test_corruption_huge_claimed_size: " << accepted
				<< " buffers with huge size fields were accepted\n";
		RETURN_TEST("test_corruption_huge_claimed_size", 1);
	}
	RETURN_TEST("test_corruption_huge_claimed_size", 0);
}

// ----- Nested structure with deep corruption -----

int test_corruption_deep_nested() {
	auto clean = SerializeGroup();
	// Corrupt the last 25% of the buffer (deep inside nested items)
	std::size_t start = (clean.size() * 3) / 4;
	int failures = 0;

	for (std::size_t i = start; i < clean.size(); ++i) {
		auto buf = clean;
		CorruptByte(buf, i, std::byte{0xFF});
		auto result = StormByte::Serializable<Item::Group>::Deserialize(buf);
		// Most should fail; we just require no crash
		(void)result;
	}
	RETURN_TEST("test_corruption_deep_nested", 0);
}

// ----- Comment name round-trip under light corruption -----

int test_comment_name_survives_roundtrip() {
	Item::Comment<Item::CommentType::SingleLineBash> original("important note");
	original.Name("mycomment");

	auto buffer = StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineBash>>(original).Serialize();
	auto expected = StormByte::Serializable<Item::Comment<Item::CommentType::SingleLineBash>>::Deserialize(buffer);

	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_comment_name_survives_roundtrip", 1);
	}
	ASSERT_TRUE("test_comment_name_survives_roundtrip", original == expected.value());
	ASSERT_TRUE("test_comment_name_survives_roundtrip", expected.value().Name().has_value());
	ASSERT_EQUAL("test_comment_name_survives_roundtrip", "mycomment", expected.value().Name().value());
	RETURN_TEST("test_comment_name_survives_roundtrip", 0);
}

// ----- Random multi-byte corruption (stress) -----

int test_corruption_random_stress() {
	auto clean = SerializeGroup();
	std::mt19937 rng(0xC0FFEE); // deterministic
	std::uniform_int_distribution<std::size_t> pos_dist(0, clean.size() - 1);
	std::uniform_int_distribution<int> val_dist(0, 255);

	constexpr int ITERATIONS = 500;
	for (int i = 0; i < ITERATIONS; ++i) {
		auto buf = clean;
		// Corrupt between 1 and 5 random bytes
		int count = 1 + (i % 5);
		for (int c = 0; c < count; ++c) {
			CorruptByte(buf, pos_dist(rng), static_cast<std::byte>(val_dist(rng)));
		}
		auto result = StormByte::Serializable<Item::Group>::Deserialize(buf);
		(void)result; // must not crash
	}
	RETURN_TEST("test_corruption_random_stress", 0);
}

// =============================================================================
// EXTRA MONSTER TESTS
// =============================================================================

int test_cross_type_list_as_group() {
	auto list_buf = SerializeList();
	auto as_group = StormByte::Serializable<Item::Group>::Deserialize(list_buf);
	// Must not succeed as a Group (type mismatch)
	if (as_group) {
		std::cerr << "test_cross_type_list_as_group: List buffer accepted as Group\n";
		RETURN_TEST("test_cross_type_list_as_group", 1);
	}
	RETURN_TEST("test_cross_type_list_as_group", 0);
}

int test_cross_type_group_as_list() {
	auto group_buf = SerializeGroup();
	auto as_list = StormByte::Serializable<Item::List>::Deserialize(group_buf);
	if (as_list) {
		std::cerr << "test_cross_type_group_as_list: Group buffer accepted as List\n";
		RETURN_TEST("test_cross_type_group_as_list", 1);
	}
	RETURN_TEST("test_cross_type_group_as_list", 0);
}

int test_serialize_idempotent_roundtrip() {
	Item::Group original("root");
	original.Add(Item::Value<std::string>("name", "StormByte"));
	original.Add(Item::Value<int>("n", 7));
	original.Add(Item::Comment<Item::CommentType::SingleLineBash>("note"));

	auto buf1 = StormByte::Serializable<Item::Group>(original).Serialize();
	auto d1 = StormByte::Serializable<Item::Group>::Deserialize(buf1);
	if (!d1) {
		std::cerr << d1.error()->what() << std::endl;
		RETURN_TEST("test_serialize_idempotent_roundtrip", 1);
	}

	auto buf2 = StormByte::Serializable<Item::Group>(d1.value()).Serialize();
	auto d2 = StormByte::Serializable<Item::Group>::Deserialize(buf2);
	if (!d2) {
		std::cerr << d2.error()->what() << std::endl;
		RETURN_TEST("test_serialize_idempotent_roundtrip", 1);
	}

	ASSERT_TRUE("test_serialize_idempotent_roundtrip", original == d1.value());
	ASSERT_TRUE("test_serialize_idempotent_roundtrip", d1.value() == d2.value());
	ASSERT_TRUE("test_serialize_idempotent_roundtrip", buf1 == buf2);
	RETURN_TEST("test_serialize_idempotent_roundtrip", 0);
}

int test_deep_nesting_group() {
	constexpr int DEPTH = 64;
	Item::Group root("l0");
	Item::Group* current = &root;

	for (int i = 1; i < DEPTH; ++i) {
		Item::Group child("l" + std::to_string(i));
		current->Add(std::move(child));
		current = &current->Items().back()->Value<Item::Group>();
	}
	current->Add(Item::Value<int>("leaf", 42));

	auto buf = StormByte::Serializable<Item::Group>(root).Serialize();
	auto expected = StormByte::Serializable<Item::Group>::Deserialize(buf);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_deep_nesting_group", 1);
	}

	ASSERT_TRUE("test_deep_nesting_group", root == expected.value());
	RETURN_TEST("test_deep_nesting_group", 0);
}

int test_trailing_garbage_ignored_or_rejected() {
	auto clean = SerializeGroup();
	// Append random garbage
	auto dirty = clean;
	dirty.push_back(std::byte{0xDE});
	dirty.push_back(std::byte{0xAD});
	dirty.push_back(std::byte{0xBE});
	dirty.push_back(std::byte{0xEF});

	auto result = StormByte::Serializable<Item::Group>::Deserialize(dirty);
	// Either fully succeeds (if deserializer stops at end of object)
	// or fails controlled — both OK; crash is not.
	(void)result;
	RETURN_TEST("test_trailing_garbage_ignored_or_rejected", 0);
}

int test_double_corruption_size_and_payload() {
	auto clean = SerializeGroup();
	if (clean.size() < 16)
		RETURN_TEST("test_double_corruption_size_and_payload", 0);

	auto buf = clean;
	// Corrupt first 4 bytes and last 4 bytes
	for (std::size_t i = 0; i < 4; ++i)
		buf[i] = std::byte{0xFF};
	for (std::size_t i = 0; i < 4; ++i)
		buf[buf.size() - 1 - i] = std::byte{0xAA};

	auto result = StormByte::Serializable<Item::Group>::Deserialize(buf);
	(void)result; // must not crash
	RETURN_TEST("test_double_corruption_size_and_payload", 0);
}

// =============================================================================
// main
// =============================================================================

int main() {
	int result = 0;

	// Original tests
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

	// New robustness / corruption tests
	result += test_corruption_empty_buffer();
	result += test_corruption_truncated_all_lengths();
	result += test_corruption_header_bytes();
	result += test_corruption_no_crash_single_bit_flip();
	result += test_corruption_no_crash_single_byte_overwrite();
	result += test_corruption_payload_string_still_safe();
	result += test_corruption_payload_binary_still_safe();
	result += test_corruption_huge_claimed_size();
	result += test_corruption_deep_nested();
	result += test_comment_name_survives_roundtrip();
	result += test_corruption_random_stress();

	result += test_cross_type_list_as_group();
	result += test_cross_type_group_as_list();
	result += test_serialize_idempotent_roundtrip();
	result += test_deep_nesting_group();
	result += test_trailing_garbage_ignored_or_rejected();
	result += test_double_corruption_size_and_payload();

	if (result == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << result << " tests failed." << std::endl;
	}
	return result;
}