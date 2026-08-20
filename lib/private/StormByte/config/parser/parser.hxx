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
	 *
	 * This class is not intended to be used directly by the user.
	 * Use the free functions Parse() instead.
	 */
	class STORMBYTE_CONFIG_PRIVATE Parser {
		public:
			/**
			 * @brief Deleted copy constructor.
			 */
			Parser(const Parser&) = delete;

			/**
			 * @brief Default move constructor.
			 */
			Parser(Parser&&) = default;

			/**
			 * @brief Deleted copy assignment operator.
			 */
			Parser& operator=(const Parser&) = delete;

			/**
			 * @brief Default move assignment operator.
			 */
			Parser& operator=(Parser&&) = default;

			/**
			 * @brief Destructor.
			 */
			~Parser() = default;

			/**
			 * @brief Parse a configuration from an input stream.
			 * @param stream Input stream containing the configuration.
			 * @param root Root group where the parsed items will be stored.
			 * @param action Action to take when an item name already exists.
			 * @param before Hooks executed before parsing starts.
			 * @param after Hooks executed after successful parsing.
			 * @param on_failure Optional hook called when a parse error occurs.
			 * @return Empty Expected on success, ParseError otherwise.
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
			 * @param string String containing the configuration.
			 * @param root Root group where the parsed items will be stored.
			 * @param action Action to take when an item name already exists.
			 * @param before Hooks executed before parsing starts.
			 * @param after Hooks executed after successful parsing.
			 * @param on_failure Optional hook called when a parse error occurs.
			 * @return Empty Expected on success, ParseError otherwise.
			 */
			static Expected<void, ParseError> Parse(
				const std::string& string,
				Item::Group& root,
				const OnExistingAction& action,
				const HookFunctions& before,
				const HookFunctions& after,
				const OptionalFailureHook& on_failure);

		private:
			Tokenizer& m_tokenizer; ///< Reference to the tokenizer
			const OnExistingAction c_on_existing_action; ///< Action to take on name collision
			unsigned int m_container_level = 0; ///< Current nesting level of containers

			/**
			 * @brief Private constructor.
			 * @param tokenizer Reference to the tokenizer that will supply tokens.
			 * @param action Action to take when an item name already exists.
			 */
			explicit Parser(Tokenizer& tokenizer, const OnExistingAction& action);

			/**
			 * @brief Main recursive parsing function.
			 * @param container Destination container that will receive the parsed items.
			 * @param mode Parsing mode (Named for Groups, Unnamed for Lists).
			 * @return Empty Expected on success, ParseError otherwise.
			 */
			Expected<void, ParseError> Parse(Item::Container& container, Mode mode);

			/**
			 * @brief Expects the next token to be of the given type.
			 * @param expected The expected token type.
			 * @return The token if it matches, or a ParseError.
			 */
			Expected<Token, ParseError> Expect(TokenType expected);

			/**
			 * @brief Converts a Comment token into the corresponding Comment item.
			 * @param token The comment token.
			 * @return A shared pointer to the created Comment item.
			 */
			Item::Base::PointerType MakeComment(const Token& token);
	};

	/**
	 * @brief Convenience free function that forwards to Parser::Parse (stream version).
	 */
	Expected<void, ParseError> STORMBYTE_CONFIG_PRIVATE Parse(
		std::istream& stream,
		Item::Group& root,
		const OnExistingAction& action,
		const HookFunctions& before,
		const HookFunctions& after,
		const OptionalFailureHook& on_failure);

	/**
	 * @brief Convenience free function that forwards to Parser::Parse (string version).
	 */
	Expected<void, ParseError> STORMBYTE_CONFIG_PRIVATE Parse(
		const std::string& string,
		Item::Group& root,
		const OnExistingAction& action,
		const HookFunctions& before,
		const HookFunctions& after,
		const OptionalFailureHook& on_failure);
}
