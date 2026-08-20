#include <StormByte/config/config.hxx>
#include <StormByte/system.hxx>
#include <StormByte/test_handlers.h>

#include <fstream>
#include <vector>

using namespace StormByte::Config;

namespace {
	std::vector<std::byte> ReadFile(const std::filesystem::path& path) {
		std::ifstream in(path, std::ios::binary | std::ios::ate);
		if (!in)
			throw std::runtime_error("cannot open " + path.string());
		const auto size = static_cast<std::size_t>(in.tellg());
		in.seekg(0);
		std::vector<std::byte> buf(size);
		if (size && !in.read(reinterpret_cast<char*>(buf.data()), static_cast<std::streamsize>(size)))
			throw std::runtime_error("cannot read " + path.string());
		return buf;
	}

	ExpectedConfig LoadBinaryFile(const std::filesystem::path& path) {
		std::ifstream in(path, std::ios::binary);
		if (!in) {
			std::cerr << "cannot open " << path << std::endl;
			return StormByte::Unexpected<StormByte::DeserializeError>("cannot open file");
		}
		return Config::Load(in, Mode::Binary);
	}
}

int test_binary_rejects_legacy_payload() {
	const auto path = CurrentFileDirectory / "files" / "config_example_legacy.bin";
	auto result = LoadBinaryFile(path);
	if (result) {
		std::cerr << "test_binary_rejects_legacy_payload: legacy payload was accepted\n";
		RETURN_TEST("test_binary_rejects_legacy_payload", 1);
	}
	RETURN_TEST("test_binary_rejects_legacy_payload", 0);
}

int test_binary_rejects_bad_magic() {
	const auto path = CurrentFileDirectory / "files" / "config_example_bad_magic.bin";
	auto result = LoadBinaryFile(path);
	if (result) {
		std::cerr << "test_binary_rejects_bad_magic: corrupted magic was accepted\n";
		RETURN_TEST("test_binary_rejects_bad_magic", 1);
	}
	RETURN_TEST("test_binary_rejects_bad_magic", 0);
}

int test_binary_rejects_bad_version() {
	const auto path = CurrentFileDirectory / "files" / "config_example_bad_version.bin";
	auto result = LoadBinaryFile(path);
	if (result) {
		std::cerr << "test_binary_rejects_bad_version: version 0 was accepted\n";
		RETURN_TEST("test_binary_rejects_bad_version", 1);
	}
	RETURN_TEST("test_binary_rejects_bad_version", 0);
}

int test_binary_accepts_golden() {
	const auto path = CurrentFileDirectory / "files" / "config_example.bin";
	auto result = LoadBinaryFile(path);
	if (!result) {
		std::cerr << "test_binary_accepts_golden: " << result.error()->what() << std::endl;
		RETURN_TEST("test_binary_accepts_golden", 1);
	}

	const auto text_path = CurrentFileDirectory / "files" / "complex_conf1.conf";
	std::ifstream text_in(text_path);
	if (!text_in) {
		std::cerr << "cannot open " << text_path << std::endl;
		RETURN_TEST("test_binary_accepts_golden", 1);
	}
	Config from_text;
	text_in >> from_text;

	ASSERT_EQUAL("test_binary_accepts_golden", result.value(), from_text);
	RETURN_TEST("test_binary_accepts_golden", 0);
}

int test_binary_save_load_roundtrip_stream() {
	Config original;
	original.Add(Item::Value<std::string>("name", "StormByte"));
	original.Add(Item::Value<int>("n", 7));

	const auto tmp = CurrentFileDirectory / "files" / "config_roundtrip_tmp.bin";
	{
		std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
		if (!out) {
			std::cerr << "cannot write " << tmp << std::endl;
			RETURN_TEST("test_binary_save_load_roundtrip_stream", 1);
		}
		original.Save(out, Mode::Binary);
	}

	std::ifstream in(tmp, std::ios::binary);
	if (!in) {
		std::cerr << "cannot read " << tmp << std::endl;
		RETURN_TEST("test_binary_save_load_roundtrip_stream", 1);
	}
	auto loaded = Config::Load(in, Mode::Binary);
	std::remove(tmp.string().c_str());

	if (!loaded) {
		std::cerr << loaded.error()->what() << std::endl;
		RETURN_TEST("test_binary_save_load_roundtrip_stream", 1);
	}
	ASSERT_TRUE("test_binary_save_load_roundtrip_stream", original == loaded.value());
	RETURN_TEST("test_binary_save_load_roundtrip_stream", 0);
}

int main() {
	int result = 0;
	result += test_binary_accepts_golden();
	result += test_binary_rejects_legacy_payload();
	result += test_binary_rejects_bad_magic();
	result += test_binary_rejects_bad_version();
	result += test_binary_save_load_roundtrip_stream();

	if (result == 0)
		std::cout << "All tests passed!" << std::endl;
	else
		std::cout << result << " tests failed." << std::endl;
	return result;
}
