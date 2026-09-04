/*
 * Copyright (C) 2024-2026 David C. Manuelda (StormBytePP)
 *
 * This file is part of StormByte-Config.
 *
 * StormByte-Config is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * or later, as published by the Free Software Foundation.
 *
 * StormByte-Config is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with StormByte-Config. If not, see
 * <https://www.gnu.org/licenses/lgpl-3.0.html>.
 */

#include <StormByte/config/config.hxx>
#include <StormByte/system.hxx>
#include <StormByte/test_handlers.h>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>
#include <sstream>
using namespace StormByte::Config;
namespace {
Config MakeSampleConfig() {
	Config cfg;
	cfg.Add(Item::Value<std::string>("name", "StormByte"));
	cfg.Add(Item::Value<int>("version", 42));
	cfg.Add(Item::Value<bool>("enabled", true));
	cfg.Add(Item::Comment<Item::CommentType::SingleLineBash>("a comment"));
	cfg.Add(Item::Value<std::vector<std::byte>>("payload",
		{std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF}}));
	Item::List list("items");
	list.Add(Item::Value(1));
	list.Add(Item::Value("two"));
	list.Add(Item::Value(3.14));
	cfg.Add(std::move(list));
	return cfg;
}
Config MakeListConfig() {
	Config cfg;
	Item::List list("test");
	list.Add(Item::Value(10));
	list.Add(Item::Value("hello"));
	list.Add(Item::Comment<Item::CommentType::SingleLineC>("note"));
	list.Add(Item::Value(true));
	cfg.Add(std::move(list));
	return cfg;
}
std::vector<std::byte> SerializeConfig(const Config& cfg) {
	std::ostringstream oss(std::ios::binary);
	cfg.Save(oss, Mode::Binary);
	const std::string& s = oss.str();
	return std::vector<std::byte>(
		reinterpret_cast<const std::byte*>(s.data()),
		reinterpret_cast<const std::byte*>(s.data()) + s.size());
}
ExpectedConfig DeserializeConfig(const std::vector<std::byte>& buffer) {
	std::string raw(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	std::istringstream iss(std::move(raw), std::ios::binary);
	return Config::Load(iss, Mode::Binary);
}
void FlipBit(std::vector<std::byte>& buf, std::size_t byte_index, unsigned bit) {
	if (byte_index >= buf.size() || bit > 7) return;
	auto& b = reinterpret_cast<unsigned char&>(buf[byte_index]);
	b ^= static_cast<unsigned char>(1u << bit);
}
void CorruptByte(std::vector<std::byte>& buf, std::size_t index, std::byte value) {
	if (index < buf.size())
		buf[index] = value;
}
std::vector<std::byte> Truncate(const std::vector<std::byte>& buf, std::size_t new_size) {
	if (new_size >= buf.size()) return buf;
	return std::vector<std::byte>(buf.begin(), buf.begin() + static_cast<std::ptrdiff_t>(new_size));
}
} // namespace
// =============================================================================
// Round-trip via Config::Save / Config::Load (Mode::Binary)
// =============================================================================
int test_serialize_value_string() {
	Config cfg;
	cfg.Add(Item::Value<std::string>("test", "Hello, World!"));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_string", 1);
	}
	ASSERT_TRUE("test_serialize_value_string", cfg == expected.value());
	RETURN_TEST("test_serialize_value_string", 0);
}
int test_serialize_value_int() {
	Config cfg;
	cfg.Add(Item::Value<int>("test", 62));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_int", 1);
	}
	ASSERT_TRUE("test_serialize_value_int", cfg == expected.value());
	RETURN_TEST("test_serialize_value_int", 0);
}
int test_serialize_value_double() {
	Config cfg;
	cfg.Add(Item::Value<double>("test", 62.78));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_double", 1);
	}
	ASSERT_TRUE("test_serialize_value_double", cfg == expected.value());
	RETURN_TEST("test_serialize_value_double", 0);
}
int test_serialize_value_bool() {
	Config cfg;
	cfg.Add(Item::Value<bool>("test", true));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_bool", 1);
	}
	ASSERT_TRUE("test_serialize_value_bool", cfg == expected.value());
	RETURN_TEST("test_serialize_value_bool", 0);
}
int test_serialize_comment_single_bash() {
	Config cfg;
	cfg.Add(Item::Comment<Item::CommentType::SingleLineBash>("Single line comment in bash"));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_comment_single_bash", 1);
	}
	ASSERT_TRUE("test_serialize_comment_single_bash", cfg == expected.value());
	RETURN_TEST("test_serialize_comment_single_bash", 0);
}
int test_serialize_comment_single_C() {
	Config cfg;
	cfg.Add(Item::Comment<Item::CommentType::SingleLineC>("Single line comment in C"));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_comment_single_C", 1);
	}
	ASSERT_TRUE("test_serialize_comment_single_C", cfg == expected.value());
	RETURN_TEST("test_serialize_comment_single_C", 0);
}
int test_serialize_comment_multi_C() {
	Config cfg;
	cfg.Add(Item::Comment<Item::CommentType::MultiLineC>(
		"Multi line comment in C\nAnother line\nAnd another one"));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_comment_multi_C", 1);
	}
	ASSERT_TRUE("test_serialize_comment_multi_C", cfg == expected.value());
	RETURN_TEST("test_serialize_comment_multi_C", 0);
}
int test_serialize_group() {
	Config cfg;
	cfg.Add(Item::Value<std::string>("string", "Hello, World!"));
	cfg.Add(Item::Value<int>("int", 62));
	cfg.Add(Item::Value<double>("double", 62.78));
	cfg.Add(Item::Value<bool>("bool", true));
	cfg.Add(Item::Comment<Item::CommentType::SingleLineBash>("Single line comment in bash"));
	cfg.Add(Item::Comment<Item::CommentType::SingleLineC>("Single line comment in C"));
	cfg.Add(Item::Comment<Item::CommentType::MultiLineC>(
		"Multi line comment in C\nAnother line\nAnd another one"));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_group", 1);
	}
	ASSERT_TRUE("test_serialize_group", cfg == expected.value());
	RETURN_TEST("test_serialize_group", 0);
}
int test_serialize_list() {
	Config cfg;
	Item::List list("test");
	list.Add(Item::Value<std::string>("Hello, World!"));
	list.Add(Item::Value<int>(62));
	list.Add(Item::Value<double>(62.78));
	list.Add(Item::Value<bool>(true));
	list.Add(Item::Comment<Item::CommentType::SingleLineBash>("Single line comment in bash"));
	list.Add(Item::Comment<Item::CommentType::SingleLineC>("Single line comment in C"));
	list.Add(Item::Comment<Item::CommentType::MultiLineC>(
		"Multi line comment in C\nAnother line\nAnd another one"));
	cfg.Add(std::move(list));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_list", 1);
	}
	ASSERT_TRUE("test_serialize_list", cfg == expected.value());
	RETURN_TEST("test_serialize_list", 0);
}
int test_serialize_nested_groups() {
	Config cfg;
	cfg.Add(Item::Value<std::string>("string", "Hello, World!"));
	cfg.Add(Item::Value<int>("int", 62));
	cfg.Add(Item::Value<double>("double", 62.78));
	cfg.Add(Item::Value<bool>("bool", true));
	Item::Group nested("nested");
	nested.Add(Item::Value<std::string>("string", "Hello, World!"));
	nested.Add(Item::Value<int>("int", 62));
	nested.Add(Item::Value<double>("double", 62.78));
	nested.Add(Item::Value<bool>("bool", true));
	cfg.Add(std::move(nested));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_nested_groups", 1);
	}
	ASSERT_TRUE("test_serialize_nested_groups", cfg == expected.value());
	RETURN_TEST("test_serialize_nested_groups", 0);
}
int test_config_binary_deserialize() {
	try {
		std::fstream file;
		const std::filesystem::path binary_file = CurrentFileDirectory / "files" / "config_example.bin";
		const std::filesystem::path human_readable_file = CurrentFileDirectory / "files" / "complex_conf1.conf";
		file.open(binary_file, std::ios::binary | std::ios::in);
		if (!file.is_open()) {
			std::cerr << "Can't open file " << binary_file.string() << std::endl;
			RETURN_TEST("test_config_binary_deserialize", 1);
		}
		auto expected_cfg = Config::Load(file, Mode::Binary);
		file.close();
		if (!expected_cfg) {
			std::cerr << expected_cfg.error()->what() << std::endl;
			RETURN_TEST("test_config_binary_deserialize", 1);
		}
		Config cfg_from_bin = std::move(expected_cfg.value());
		file.open(human_readable_file, std::ios::in);
		if (!file.is_open()) {
			std::cerr << "Can't open file " << human_readable_file.string() << std::endl;
			RETURN_TEST("test_config_binary_deserialize", 1);
		}
		Config cfg_from_text;
		file >> cfg_from_text;
		file.close();
		ASSERT_EQUAL("test_config_binary_deserialize", cfg_from_bin, cfg_from_text);
	}
	catch (const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		RETURN_TEST("test_config_binary_deserialize", 1);
	}
	RETURN_TEST("test_config_binary_deserialize", 0);
}
int test_serialize_value_binary() {
	Config cfg;
	std::vector<std::byte> data = {
		std::byte{0x48}, std::byte{0x65}, std::byte{0x6C},
		std::byte{0x6C}, std::byte{0x6F}
	};
	cfg.Add(Item::Value<std::vector<std::byte>>("test", data));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_binary", 1);
	}
	ASSERT_TRUE("test_serialize_value_binary", cfg == expected.value());
	RETURN_TEST("test_serialize_value_binary", 0);
}
int test_serialize_value_binary_empty() {
	Config cfg;
	cfg.Add(Item::Value<std::vector<std::byte>>("empty", std::vector<std::byte>{}));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_binary_empty", 1);
	}
	ASSERT_TRUE("test_serialize_value_binary_empty", cfg == expected.value());
	ASSERT_EQUAL("test_serialize_value_binary_empty", 0u,
		expected.value()["empty"].Value<std::vector<std::byte>>().size());
	RETURN_TEST("test_serialize_value_binary_empty", 0);
}
int test_serialize_value_binary_all_bytes() {
	std::vector<std::byte> data(256);
	for (std::size_t i = 0; i < 256; ++i)
		data[i] = static_cast<std::byte>(i);
	Config cfg;
	cfg.Add(Item::Value<std::vector<std::byte>>("all_bytes", data));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_binary_all_bytes", 1);
	}
	ASSERT_TRUE("test_serialize_value_binary_all_bytes", cfg == expected.value());
	RETURN_TEST("test_serialize_value_binary_all_bytes", 0);
}
int test_serialize_value_binary_large() {
	std::vector<std::byte> data(10 * 1024);
	for (std::size_t i = 0; i < data.size(); ++i)
		data[i] = static_cast<std::byte>(i % 256);
	Config cfg;
	cfg.Add(Item::Value<std::vector<std::byte>>("large", std::move(data)));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_value_binary_large", 1);
	}
	ASSERT_TRUE("test_serialize_value_binary_large", cfg == expected.value());
	RETURN_TEST("test_serialize_value_binary_large", 0);
}
int test_serialize_group_with_binary() {
	Config cfg;
	cfg.Add(Item::Value<std::string>("string", "Hello"));
	cfg.Add(Item::Value<int>("int", 42));
	cfg.Add(Item::Value<std::vector<std::byte>>("binary",
		{std::byte{0x01}, std::byte{0x02}, std::byte{0x03}}));
	cfg.Add(Item::Value<bool>("flag", true));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_group_with_binary", 1);
	}
	ASSERT_TRUE("test_serialize_group_with_binary", cfg == expected.value());
	RETURN_TEST("test_serialize_group_with_binary", 0);
}
int test_serialize_list_with_binary() {
	Config cfg;
	Item::List list("test");
	list.Add(Item::Value<std::string>("text"));
	list.Add(Item::Value<int>(99));
	list.Add(Item::Value<std::vector<std::byte>>(
		{std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF}}));
	list.Add(Item::Value<bool>(false));
	cfg.Add(std::move(list));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_list_with_binary", 1);
	}
	ASSERT_TRUE("test_serialize_list_with_binary", cfg == expected.value());
	RETURN_TEST("test_serialize_list_with_binary", 0);
}
int test_serialize_nested_with_binary() {
	Config cfg;
	Item::Group nested("nested");
	nested.Add(Item::Value<std::vector<std::byte>>("secret",
		{std::byte{0x53}, std::byte{0x65}, std::byte{0x63},
		std::byte{0x72}, std::byte{0x65}, std::byte{0x74}}));
	nested.Add(Item::Value<std::string>("name", "hidden"));
	cfg.Add(std::move(nested));
	cfg.Add(Item::Value<int>("version", 1));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_nested_with_binary", 1);
	}
	ASSERT_TRUE("test_serialize_nested_with_binary", cfg == expected.value());
	RETURN_TEST("test_serialize_nested_with_binary", 0);
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
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_binary_roundtrip_text_and_binary", 1);
	}
	ASSERT_TRUE("test_serialize_binary_roundtrip_text_and_binary", cfg == expected.value());
	RETURN_TEST("test_serialize_binary_roundtrip_text_and_binary", 0);
}
int test_text_to_binary_to_text_roundtrip() {
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
		auto binary_buffer = SerializeConfig(cfg1);
		auto expected = DeserializeConfig(binary_buffer);
		if (!expected) {
			std::cerr << expected.error()->what() << std::endl;
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
	}
	catch (const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		RETURN_TEST("test_text_to_binary_to_text_roundtrip", 1);
	}
	RETURN_TEST("test_text_to_binary_to_text_roundtrip", 0);
}
// =============================================================================
// Corruption / robustness (Mode::Binary)
// =============================================================================
int test_corruption_empty_buffer() {
	std::vector<std::byte> empty;
	auto r = DeserializeConfig(empty);
	if (r) {
		std::cerr << "empty buffer was accepted\n";
		RETURN_TEST("test_corruption_empty_buffer", 1);
	}
	RETURN_TEST("test_corruption_empty_buffer", 0);
}
int test_corruption_truncated_all_lengths() {
	auto clean = SerializeConfig(MakeSampleConfig());
	int failures = 0;
	for (std::size_t len = 0; len < clean.size(); ++len) {
		auto truncated = Truncate(clean, len);
		auto result = DeserializeConfig(truncated);
		if (result) {
			std::cerr << "truncated size " << len << " accepted\n";
			++failures;
		}
	}
	RETURN_TEST("test_corruption_truncated_all_lengths", failures > 0 ? 1 : 0);
}
int test_corruption_header_bytes() {
	auto clean = SerializeConfig(MakeSampleConfig());
	if (clean.size() < 16)
		RETURN_TEST("test_corruption_header_bytes", 1);
	int accepted = 0;
	for (std::size_t i = 0; i < 16; ++i) {
		for (int v = 0; v < 256; v += 17) {
			auto buf = clean;
			CorruptByte(buf, i, static_cast<std::byte>(v));
			if (DeserializeConfig(buf))
				++accepted;
		}
	}
	if (accepted > 200) {
		std::cerr << "too many corrupted headers accepted (" << accepted << ")\n";
		RETURN_TEST("test_corruption_header_bytes", 1);
	}
	RETURN_TEST("test_corruption_header_bytes", 0);
}
int test_corruption_no_crash_single_bit_flip() {
	auto clean = SerializeConfig(MakeSampleConfig());
	for (std::size_t i = 0; i < clean.size(); ++i) {
		for (unsigned bit = 0; bit < 8; ++bit) {
			auto buf = clean;
			FlipBit(buf, i, bit);
			(void)DeserializeConfig(buf);
		}
	}
	RETURN_TEST("test_corruption_no_crash_single_bit_flip", 0);
}
int test_corruption_no_crash_single_byte_overwrite() {
	auto clean = SerializeConfig(MakeListConfig());
	for (std::size_t i = 0; i < clean.size(); ++i) {
		for (int v = 0; v < 256; v += 23) {
			auto buf = clean;
			CorruptByte(buf, i, static_cast<std::byte>(v));
			(void)DeserializeConfig(buf);
		}
	}
	RETURN_TEST("test_corruption_no_crash_single_byte_overwrite", 0);
}
int test_corruption_payload_still_safe() {
	Config cfg;
	cfg.Add(Item::Value<std::string>("key", "Hello, StormByte!"));
	auto clean = SerializeConfig(cfg);
	std::size_t start = clean.size() / 2;
	for (std::size_t i = start; i < clean.size(); ++i) {
		auto buf = clean;
		FlipBit(buf, i, 0);
		(void)DeserializeConfig(buf);
	}
	RETURN_TEST("test_corruption_payload_still_safe", 0);
}
int test_corruption_payload_binary_still_safe() {
	std::vector<std::byte> data(64);
	for (std::size_t i = 0; i < data.size(); ++i)
		data[i] = static_cast<std::byte>(i);
	Config cfg;
	cfg.Add(Item::Value<std::vector<std::byte>>("bin", data));
	auto clean = SerializeConfig(cfg);
	std::size_t start = clean.size() / 3;
	for (std::size_t i = start; i < clean.size(); ++i) {
		auto buf = clean;
		CorruptByte(buf, i, std::byte{0xAA});
		(void)DeserializeConfig(buf);
	}
	RETURN_TEST("test_corruption_payload_binary_still_safe", 0);
}
int test_corruption_huge_claimed_size() {
	auto clean = SerializeConfig(MakeSampleConfig());
	int accepted = 0;
	if (clean.size() >= sizeof(std::uint64_t)) {
		auto buf = clean;
		std::uint64_t huge = 0xFFFFFFFFFFFFFFFFULL;
		std::memcpy(buf.data(), &huge, sizeof(huge));
		if (DeserializeConfig(buf))
			++accepted;
	}
	for (std::size_t i = 0; i + sizeof(std::uint64_t) <= std::min<std::size_t>(24, clean.size()); i += sizeof(std::uint64_t)) {
		auto buf = clean;
		std::uint64_t huge = 0xFFFFFFFFFFFFFFFFULL;
		std::memcpy(buf.data() + i, &huge, sizeof(huge));
		if (DeserializeConfig(buf))
			++accepted;
	}
	if (accepted > 0) {
		std::cerr << accepted << " buffers with huge size accepted\n";
		RETURN_TEST("test_corruption_huge_claimed_size", 1);
	}
	RETURN_TEST("test_corruption_huge_claimed_size", 0);
}
int test_corruption_deep_nested() {
	auto clean = SerializeConfig(MakeSampleConfig());
	std::size_t start = (clean.size() * 3) / 4;
	for (std::size_t i = start; i < clean.size(); ++i) {
		auto buf = clean;
		CorruptByte(buf, i, std::byte{0xFF});
		(void)DeserializeConfig(buf);
	}
	RETURN_TEST("test_corruption_deep_nested", 0);
}
int test_comment_name_survives_roundtrip() {
	Config cfg;
	Item::Comment<Item::CommentType::SingleLineBash> original("important note");
	original.Name("mycomment");
	cfg.Add(std::move(original));
	auto buffer = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_comment_name_survives_roundtrip", 1);
	}
	ASSERT_TRUE("test_comment_name_survives_roundtrip", cfg == expected.value());
	RETURN_TEST("test_comment_name_survives_roundtrip", 0);
}
int test_corruption_random_stress() {
	auto clean = SerializeConfig(MakeSampleConfig());
	std::mt19937 rng(0xC0FFEE);
	std::uniform_int_distribution<std::size_t> pos_dist(0, clean.size() - 1);
	std::uniform_int_distribution<int> val_dist(0, 255);
	constexpr int ITERATIONS = 500;
	for (int i = 0; i < ITERATIONS; ++i) {
		auto buf = clean;
		int count = 1 + (i % 5);
		for (int c = 0; c < count; ++c)
			CorruptByte(buf, pos_dist(rng), static_cast<std::byte>(val_dist(rng)));
		(void)DeserializeConfig(buf);
	}
	RETURN_TEST("test_corruption_random_stress", 0);
}
int test_serialize_idempotent_roundtrip() {
	Config original;
	original.Add(Item::Value<std::string>("name", "StormByte"));
	original.Add(Item::Value<int>("n", 7));
	original.Add(Item::Comment<Item::CommentType::SingleLineBash>("note"));
	auto buf1 = SerializeConfig(original);
	auto d1 = DeserializeConfig(buf1);
	if (!d1) {
		std::cerr << d1.error()->what() << std::endl;
		RETURN_TEST("test_serialize_idempotent_roundtrip", 1);
	}
	auto buf2 = SerializeConfig(d1.value());
	auto d2 = DeserializeConfig(buf2);
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
	Config cfg;
	Item::Group root("l0");
	Item::Group* current = &root;
	for (int i = 1; i < DEPTH; ++i) {
		Item::Group child("l" + std::to_string(i));
		current->Add(std::move(child));
		current = &current->Items().back()->Value<Item::Group>();
	}
	current->Add(Item::Value<int>("leaf", 42));
	cfg.Add(std::move(root));
	auto buf = SerializeConfig(cfg);
	auto expected = DeserializeConfig(buf);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_deep_nesting_group", 1);
	}
	ASSERT_TRUE("test_deep_nesting_group", cfg == expected.value());
	RETURN_TEST("test_deep_nesting_group", 0);
}
int test_trailing_garbage_ignored_or_rejected() {
	auto clean = SerializeConfig(MakeSampleConfig());
	auto dirty = clean;
	dirty.push_back(std::byte{0xDE});
	dirty.push_back(std::byte{0xAD});
	dirty.push_back(std::byte{0xBE});
	dirty.push_back(std::byte{0xEF});
	(void)DeserializeConfig(dirty);
	RETURN_TEST("test_trailing_garbage_ignored_or_rejected", 0);
}
int test_double_corruption_size_and_payload() {
	auto clean = SerializeConfig(MakeSampleConfig());
	if (clean.size() < 16)
		RETURN_TEST("test_double_corruption_size_and_payload", 0);
	auto buf = clean;
	for (std::size_t i = 0; i < 4; ++i)
		buf[i] = std::byte{0xFF};
	for (std::size_t i = 0; i < 4; ++i)
		buf[buf.size() - 1 - i] = std::byte{0xAA};
	(void)DeserializeConfig(buf);
	RETURN_TEST("test_double_corruption_size_and_payload", 0);
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
	result += test_config_binary_deserialize();
	result += test_serialize_value_binary();
	result += test_serialize_value_binary_empty();
	result += test_serialize_value_binary_all_bytes();
	result += test_serialize_value_binary_large();
	result += test_serialize_group_with_binary();
	result += test_serialize_list_with_binary();
	result += test_serialize_nested_with_binary();
	result += test_serialize_binary_roundtrip_text_and_binary();
	result += test_text_to_binary_to_text_roundtrip();
	result += test_corruption_empty_buffer();
	result += test_corruption_truncated_all_lengths();
	result += test_corruption_header_bytes();
	result += test_corruption_no_crash_single_bit_flip();
	result += test_corruption_no_crash_single_byte_overwrite();
	result += test_corruption_payload_still_safe();
	result += test_corruption_payload_binary_still_safe();
	result += test_corruption_huge_claimed_size();
	result += test_corruption_deep_nested();
	result += test_comment_name_survives_roundtrip();
	result += test_corruption_random_stress();
	result += test_serialize_idempotent_roundtrip();
	result += test_deep_nesting_group();
	result += test_trailing_garbage_ignored_or_rejected();
	result += test_double_corruption_size_and_payload();
	if (result == 0)
		std::cout << "All tests passed!" << std::endl;
	else
		std::cout << result << " tests failed." << std::endl;
	return result;
}
