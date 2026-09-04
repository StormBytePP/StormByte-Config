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

#pragma once

#include <StormByte/config/alias.hxx>
#include <StormByte/config/exception.hxx>
#include <StormByte/config/item/comment.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/config/item/list.hxx>
#include <StormByte/config/item/value.hxx>
#include <StormByte/config/parser/tokenizer.hxx>
#include <StormByte/config/parser/type.hxx>
#include <StormByte/config/typedefs.hxx>
#include <StormByte/expected.hxx>

#include <istream>
#include <string>

/**
 * @brief Text parser internals.
 */
namespace StormByte::Config::Parser {
	/**
	 * @class Parser
	 * @brief Recursive descent parser: tokens → item tree.
	 *
	 * Not a public API. Use the free `Parse()` functions.
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
			Tokenizer& m_tokenizer;							///< Tokenizer
			const OnExistingAction c_on_existing_action;	///< Collision policy
			unsigned int m_container_level = 0;				///< Nesting depth

			/**
			 * @brief Private constructor.
			 * @param tokenizer Tokenizer that supplies tokens.
			 * @param action Action when an item name already exists.
			 */
			explicit Parser(Tokenizer& tokenizer, const OnExistingAction& action);

			/**
			 * @brief Main recursive parse.
			 * @param container Destination container.
			 * @param mode Named (groups) or Unnamed (lists).
			 * @return Empty Expected on success, ParseError otherwise.
			 */
			Expected<void, ParseError> Parse(Item::Container& container, Mode mode);

			/**
			 * @brief Expects the next token to be of the given type.
			 * @param expected Expected token type.
			 * @return The token if it matches, or a ParseError.
			 */
			Expected<Token, ParseError> Expect(TokenType expected);

			/**
			 * @brief Converts a Comment token into a Comment item.
			 * @param token Comment token.
			 * @return Shared pointer to the created Comment item.
			 */
			Item::Base::PointerType MakeComment(const Token& token);
	};

	/**
	 * @brief Forwards to Parser::Parse (stream).
	 */
	Expected<void, ParseError> STORMBYTE_CONFIG_PRIVATE Parse(
		std::istream& stream,
		Item::Group& root,
		const OnExistingAction& action,
		const HookFunctions& before,
		const HookFunctions& after,
		const OptionalFailureHook& on_failure);

	/**
	 * @brief Forwards to Parser::Parse (string).
	 */
	Expected<void, ParseError> STORMBYTE_CONFIG_PRIVATE Parse(
		const std::string& string,
		Item::Group& root,
		const OnExistingAction& action,
		const HookFunctions& before,
		const HookFunctions& after,
		const OptionalFailureHook& on_failure);
}
