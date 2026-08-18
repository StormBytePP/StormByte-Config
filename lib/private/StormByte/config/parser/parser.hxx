#pragma once

#include <StormByte/config/parser/tokenizer.hxx>
#include <StormByte/config/alias.hxx>
#include <StormByte/config/exception.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/config/parser/type.hxx>
#include <StormByte/config/type.hxx>
#include <StormByte/expected.hxx>

#include <istream>
#include <string>

/**
 * @namespace Parser
 * @brief Configuration file parser internals.
 */
namespace StormByte::Config::Parser {
	/**
	 * @class Parser
	 * @brief Recursive descent parser that consumes tokens from a Tokenizer
	 *        and builds the configuration item tree.
	 */
	class STORMBYTE_CONFIG_PRIVATE Parser {
		public:
			Parser(const Parser&) = delete;
			Parser(Parser&&) = default;
			Parser& operator=(const Parser&) = delete;
			Parser& operator=(Parser&&) = default;
			~Parser() = default;

			/**
			 * @brief Parse a configuration from an input stream.
			 */
			static Expected<void, ParseError> Parse(
				std::istream& stream,
				Item::Group& root,
				const OnExistingAction& action,
				const HookFunctions& before,
				const HookFunctions& after,
				const OptionalFailureHook& on_failure);

			/**
			 * @brief Parse a configuration from a string.
			 */
			static Expected<void, ParseError> Parse(
				const std::string& string,
				Item::Group& root,
				const OnExistingAction& action,
				const HookFunctions& before,
				const HookFunctions& after,
				const OptionalFailureHook& on_failure);

		private:
			Tokenizer&              m_tokenizer;
			const OnExistingAction  c_on_existing_action;
			unsigned int            m_container_level = 0;

			explicit Parser(Tokenizer& tokenizer, const OnExistingAction& action);

			/**
			 * @brief Main recursive parsing function.
			 * @param container Destination container.
			 * @param mode Named (Group) or Unnamed (List).
			 */
			Expected<void, ParseError> Parse(Item::Container& container, Mode mode);

			/**
			 * @brief Parses a single value/item and returns it as a Base pointer.
			 */
			Expected<Item::Base::PointerType, ParseError> ParseValue();

			/**
			 * @brief Expects the next token to be of the given type.
			 * @return The token or a ParseError.
			 */
			Expected<Token, ParseError> Expect(TokenType expected);

			/**
			 * @brief Converts a Comment token into the corresponding Comment item.
			 */
			Item::Base::PointerType MakeComment(const Token& token);
	};

	/**
	 * @brief Free function entry point (stream version).
	 */
	Expected<void, ParseError> STORMBYTE_CONFIG_PRIVATE Parse(
		std::istream& stream,
		Item::Group& root,
		const OnExistingAction& action,
		const HookFunctions& before,
		const HookFunctions& after,
		const OptionalFailureHook& on_failure);

	/**
	 * @brief Free function entry point (string version).
	 */
	Expected<void, ParseError> STORMBYTE_CONFIG_PRIVATE Parse(
		const std::string& string,
		Item::Group& root,
		const OnExistingAction& action,
		const HookFunctions& before,
		const HookFunctions& after,
		const OptionalFailureHook& on_failure);
}
