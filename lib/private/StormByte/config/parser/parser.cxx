#include <StormByte/config/parser/parser.hxx>
#include <sstream>

using namespace StormByte::Config;
using namespace StormByte::Config::Parser;

StormByte::Config::Parser::Parser::Parser(Tokenizer& tokenizer, const OnExistingAction& action)
	: m_tokenizer(tokenizer), c_on_existing_action(action) {}

// ----------------------------------------------------------------------
// Public entry points
// ----------------------------------------------------------------------

StormByte::Expected<void, ParseError> StormByte::Config::Parser::Parser::Parse(
	std::istream& stream,
	Item::Group& root,
	const OnExistingAction& action,
	const HookFunctions& before,
	const HookFunctions& after,
	const OptionalFailureHook& on_failure) {

	Tokenizer tokenizer(stream);
	Parser parser(tokenizer, action);

	for (const auto& hook : before)
		hook(root);

	auto res = parser.Parse(root, Mode::Named);

	if (!res) {
		bool should_throw = true;
		if (on_failure)
			should_throw = (*on_failure)(root);

		if (should_throw)
			return Unexpected(std::move(res.error()));
		return {};
	}

	for (const auto& hook : after)
		hook(root);

	return {};
}

StormByte::Expected<void, ParseError> StormByte::Config::Parser::Parser::Parse(
	const std::string& string,
	Item::Group& root,
	const OnExistingAction& action,
	const HookFunctions& before,
	const HookFunctions& after,
	const OptionalFailureHook& on_failure) {

	std::istringstream iss(string);
	return Parse(iss, root, action, before, after, on_failure);
}

// ----------------------------------------------------------------------
// Core recursive parser
// ----------------------------------------------------------------------

StormByte::Expected<void, ParseError> StormByte::Config::Parser::Parser::Parse(Item::Container& container, Mode mode) {
	while (true) {
		Token token = m_tokenizer.Next();

		// End of current container
		if (token.type == TokenType::RBrace || token.type == TokenType::RBracket) {
			if (m_container_level == 0) {
				return Unexpected<ParseError>("Unexpected container end on line {}", token.line);
			}
			--m_container_level;
			return {};
		}

		if (token.type == TokenType::EndOfFile) {
			if (m_container_level > 0) {
				return Unexpected<ParseError>("Unexpected EOF (unclosed container)");
			}
			return {};
		}

		// Comments can appear anywhere
		if (token.type == TokenType::Comment) {
			container.Add(MakeComment(token), c_on_existing_action);
			continue;
		}

		std::string name;
		if (mode == Mode::Named) {
			if (token.type != TokenType::Identifier) {
				return Unexpected<ParseError>("Expected item name on line {}, got something else", token.line);
			}
			name = std::move(token.value);

			auto eq = Expect(TokenType::Equal);
			if (!eq) return Unexpected(std::move(eq.error()));

			token = m_tokenizer.Next(); // value token
		}

		// Now token holds the value (or opening of a container)
		Expected<Item::Base::PointerType, ParseError> item_res;

		switch (token.type) {
			case TokenType::String:
				item_res = Item::Base::MakePointer<Item::Value<std::string>>(std::move(token.value));
				break;

			case TokenType::Integer: {
				try {
					int v = std::stoi(token.value);
					item_res = Item::Base::MakePointer<Item::Value<int>>(v);
				} catch (...) {
					return Unexpected<ParseError>("Invalid integer '{}' on line {}", token.value, token.line);
				}
				break;
			}

			case TokenType::Double: {
				try {
					double v = std::stod(token.value);
					item_res = Item::Base::MakePointer<Item::Value<double>>(v);
				} catch (...) {
					return Unexpected<ParseError>("Invalid double '{}' on line {}", token.value, token.line);
				}
				break;
			}

			case TokenType::Bool:
				item_res = Item::Base::MakePointer<Item::Value<bool>>(token.value == "true");
				break;

			case TokenType::LBrace: {
				++m_container_level;
				Item::Group group;
				auto res = Parse(group, Mode::Named);
				if (!res) return Unexpected(std::move(res.error()));
				item_res = group.Move();
				break;
			}

			case TokenType::LBracket: {
				++m_container_level;
				Item::List list;
				auto res = Parse(list, Mode::Unnamed);
				if (!res) return Unexpected(std::move(res.error()));
				item_res = list.Move();
				break;
			}

			case TokenType::Comment:
				item_res = MakeComment(token);
				break;

			default:
				return Unexpected<ParseError>("Unexpected token on line {}", token.line);
		}

		if (!item_res) return Unexpected(std::move(item_res.error()));

		auto item = std::move(item_res.value());
		if (mode == Mode::Named)
			item->Name(std::move(name));

		container.Add(item, c_on_existing_action);
	}
}

// ----------------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------------

StormByte::Expected<Token, ParseError> StormByte::Config::Parser::Parser::Expect(TokenType expected) {
	Token token = m_tokenizer.Next();
	if (token.type != expected) {
		return Unexpected<ParseError>("Unexpected token on line {}", token.line);
	}
	return token;
}

Item::Base::PointerType StormByte::Config::Parser::Parser::MakeComment(const Token& token) {
	switch (token.comment_type) {
		case CommentType::SingleLineBash:
			return Item::Base::MakePointer<Item::Comment<Item::CommentType::SingleLineBash>>(token.value);
		case CommentType::SingleLineC:
			return Item::Base::MakePointer<Item::Comment<Item::CommentType::SingleLineC>>(token.value);
		case CommentType::MultiLineC:
			return Item::Base::MakePointer<Item::Comment<Item::CommentType::MultiLineC>>(token.value);
		default:
			return Item::Base::MakePointer<Item::Comment<Item::CommentType::SingleLineBash>>(token.value);
	}
}

// ----------------------------------------------------------------------
// Free functions
// ----------------------------------------------------------------------

namespace StormByte::Config::Parser {
	StormByte::Expected<void, ParseError> Parse(
		std::istream& stream,
		Item::Group& root,
		const OnExistingAction& action,
		const HookFunctions& before,
		const HookFunctions& after,
		const OptionalFailureHook& on_failure) {
		return Parser::Parse(stream, root, action, before, after, on_failure);
	}

	StormByte::Expected<void, ParseError> Parse(
		const std::string& string,
		Item::Group& root,
		const OnExistingAction& action,
		const HookFunctions& before,
		const HookFunctions& after,
		const OptionalFailureHook& on_failure) {
		return Parser::Parse(string, root, action, before, after, on_failure);
	}
}