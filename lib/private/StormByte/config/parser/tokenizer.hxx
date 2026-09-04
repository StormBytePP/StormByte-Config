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

#include <StormByte/config/exception.hxx>
#include <StormByte/config/parser/type.hxx>
#include <StormByte/expected.hxx>

#include <istream>
#include <string>
#include <vector>

/**
 * @brief Text parser internals.
 */
namespace StormByte::Config::Parser {
	/**
	 * @enum TokenType
	 * @brief Types of tokens produced by the Tokenizer.
	 */
	enum class TokenType {
		Identifier,	///< Item name (left side of `=`)
		String,		///< Quoted string value
		Binary,		///< Base64 payload with `b"..."` prefix
		Integer,	///< Integer number
		Double,		///< Floating-point number
		Bool,		///< `true` / `false`
		Comment,	///< Any comment
		Equal,		///< `=`
		LBrace,		///< `{`
		RBrace,		///< `}`
		LBracket,	///< `[`
		RBracket,	///< `]`
		EndOfFile,	///< End of input
		Unknown		///< Unrecognized character
	};

	/**
	 * @struct Token
	 * @brief A single lexical token.
	 */
	struct Token {
		TokenType type = TokenType::Unknown;			///< Token type
		std::string value;								///< Lexeme / content
		CommentType comment_type = CommentType::None;	///< Valid when type == Comment
		unsigned int line = 1;							///< Line where the token starts
	};

	/**
	 * @class Tokenizer
	 * @brief Turns an input stream into a token sequence.
	 */
	class STORMBYTE_CONFIG_PRIVATE Tokenizer {
		public:
			/**
			 * @brief Constructs a tokenizer for the given input stream.
			 * @param stream Input stream to tokenize.
			 */
			explicit Tokenizer(std::istream& stream);

			Tokenizer(const Tokenizer&) = delete;
			Tokenizer(Tokenizer&&) = default;
			Tokenizer& operator=(const Tokenizer&) = delete;
			Tokenizer& operator=(Tokenizer&&) = default;
			~Tokenizer() = default;

			/**
			 * @brief Returns the next token from the stream.
			 * @return Next token. `type == EndOfFile` when finished.
			 */
			Token Next();

			/**
			 * @brief Returns the current line number (1-based).
			 * @return Current line number.
			 */
			unsigned int CurrentLine() const noexcept { return m_line; }

		private:
			std::istream& m_stream;		///< Underlying input stream
			unsigned int m_line = 1;	///< Current line number

			/**
			 * @brief Skips whitespace and updates the line counter.
			 */
			void SkipWhitespace();

			/**
			 * @brief Reads a quoted string (handles escapes).
			 * @return Token of type String, or a parse error.
			 */
			Expected<Token, ParseError> ReadString();

			/**
			 * @brief Reads a binary literal (`b"..."`).
			 * The quoted payload is Base64; decoding happens in the parser.
			 * @return Token of type Binary, or a parse error.
			 */
			Expected<Token, ParseError> ReadBinary();

			/**
			 * @brief Reads a number (integer or double).
			 * @return Token of type Integer or Double.
			 */
			Token ReadNumber();

			/**
			 * @brief Reads an identifier or keyword (`true`/`false`).
			 * @return Token of type Identifier or Bool.
			 */
			Token ReadIdentifierOrKeyword();

			/**
			 * @brief Reads a single-line comment (`#` or `//`).
			 * @param type Comment subtype.
			 * @return Token of type Comment.
			 */
			Token ReadSingleLineComment(CommentType type);

			/**
			 * @brief Reads a multi-line comment (`/*` … `*\/`).
			 * @return Token of type Comment, or a parse error if unclosed.
			 */
			Expected<Token, ParseError> ReadMultiLineComment();
	};
}
