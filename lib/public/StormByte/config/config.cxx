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
#include <StormByte/config/binary/reader.hxx>
#include <StormByte/config/binary/writer.hxx>
#include <StormByte/config/parser/parser.hxx>
#include <cstring>
using namespace StormByte::Config;
Config::Config(): m_on_existing_action(OnExistingAction::ThrowException) {}
Config& Config::operator<<(const Config& source) {
	for (const auto& item: source.Items())
		Add(*item->Clone());
	return *this;
}
void Config::operator<<(std::istream& istream) {
	auto res = Parser::Parse(istream, m_root, m_on_existing_action, m_before_read_hooks, m_after_read_hooks, m_on_parse_failure_hook);
	if (!res)
		throw *res.error();
}
void Config::operator<<(const std::string& str) {
	auto res = Parser::Parse(str, m_root, m_on_existing_action, m_before_read_hooks, m_after_read_hooks, m_on_parse_failure_hook);
	if (!res)
		throw *res.error();
}
Config& StormByte::Config::operator>>(std::istream& istream, Config& config) {
	config << istream;
	return config;
}
Config& StormByte::Config::operator>>(const std::string& str, Config& config) {
	config << str;
	return config;
}
Config& Config::operator>>(Config& dest) const {
	dest << *this;
	return dest;
}
std::ostream& Config::operator>>(std::ostream& ostream) const {
	ostream << (std::string)*this;
	return ostream;
}
std::string& Config::operator>>(std::string& str) const {
	str += *this;
	return str;
}
std::ostream& StormByte::Config::operator<<(std::ostream& ostream, const Config& config) {
	ostream << (std::string)config;
	return ostream;
}
std::string& operator<<(std::string& str, const Config& config) {
	str += config;
	return str;
}
Config::operator std::string() const {
	std::string serialized = "";
	for (const auto& item : Items()) {
		serialized += item->Serialize(0) + "\n";
	}
	return serialized;
}
void Config::Save(std::ostream& stream, Mode mode) const {
	if (mode == Mode::Text) {
		stream << static_cast<std::string>(*this);
		return;
	}
	const auto buffer = Binary::Writer(*this).Serialize();
	if (!buffer.empty()) {
		stream.write(
			reinterpret_cast<const char*>(buffer.data()),
			static_cast<std::streamsize>(buffer.size()));
	}
}
ExpectedConfig Config::Load(std::istream& stream, Mode mode) {
	if (mode == Mode::Text) {
		Config cfg;
		try {
			cfg << stream; // Same path as operator<< (parser + hooks)
		} catch (const StormByte::Exception& e) {
			return StormByte::Unexpected(e);
		}
		return cfg;
	}
	// Binary: read the entire stream into a buffer
	stream.seekg(0, std::ios::end);
	const std::streamsize size = stream.tellg();
	stream.seekg(0, std::ios::beg);
	if (size < 0) {
		// Non-seekable stream (e.g. pipe): read in chunks
		std::vector<std::byte> buffer;
		char chunk[4096];
		while (stream.read(chunk, sizeof(chunk)) || stream.gcount() > 0) {
			const auto n = static_cast<std::size_t>(stream.gcount());
			const auto* p = reinterpret_cast<const std::byte*>(chunk);
			buffer.insert(buffer.end(), p, p + n);
		}
		auto result = Binary::Reader(buffer).Deserialize();
		if (!result)
			return StormByte::Unexpected(result.error());
		return std::move(result.value());
	}
	std::vector<std::byte> buffer(static_cast<std::size_t>(size));
	if (size > 0 && !stream.read(reinterpret_cast<char*>(buffer.data()), size)) {
		return StormByte::Unexpected<StormByte::DeserializeError>(
			"Failed to read binary config stream");
	}
	auto result = Binary::Reader(buffer).Deserialize();
	if (!result)
		return StormByte::Unexpected(result.error());
	return std::move(result.value());
}
