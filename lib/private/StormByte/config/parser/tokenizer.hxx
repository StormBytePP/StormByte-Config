#pragma once

#include <StormByte/config/parser/type.hxx>
#include <StormByte/config/exception.hxx>
#include <StormByte/expected.hxx>

#include <istream>
#include <string>
#include <vector>

/**
 * @namespace Parser
 * @brief Configuration file parser internals.
 */
namespace StormByte::Config::Parser {

	/**
	 * @enum TokenType
	 * @brief Types of tokens produced by the Tokenizer.
	 */
	enum class TokenType {
		Identifier,     ///< Item name (left side of '=')
		String,         ///< Quoted string value
		Binary,         ///< Binary data represented as Base64 with b"..." prefix
		Integer,        ///< Integer number
		Double,         ///< Floating-point number
		Bool,           ///< true / false
		Comment,        ///< Any kind of comment
		Equal,          ///< '='
		LBrace,         ///< '{'
		RBrace,         ///< '}'
		LBracket,       ///< '['
		RBracket,       ///< ']'
		EndOfFile,      ///< End of input
		Unknown         ///< Unrecognized character (error)
	};

	/**
	 * @struct Token
	 * @brief A single lexical token.
	 */
	struct Token {
		TokenType       type         = TokenType::Unknown;  ///< Token type
		std::string     value;                              ///< Lexeme / content
		CommentType     comment_type = CommentType::None;   ///< Only valid when type == Comment
		unsigned int    line         = 1;                   ///< Line number where the token starts
	};

	/**
	 * @class Tokenizer
	 * @brief Lexical analyzer for StormByte configuration files.
	 *
	 * Transforms an input stream into a sequence of Tokens.
	 * Designed to be efficient and easy to extend with new token types.
	 */
	class STORMBYTE_CONFIG_PRIVATE Tokenizer {
		public:
			/**
			 * @brief Constructs a tokenizer for the given input stream.
			 * @param stream Input stream to tokenize.
			 */
			explicit Tokenizer(std::istream& stream);

			/**
			 * @brief Deleted copy constructor.
			 */
			Tokenizer(const Tokenizer&) = delete;

			/**
			 * @brief Default move constructor.
			 */
			Tokenizer(Tokenizer&&) = default;

			/**
			 * @brief Deleted copy assignment.
			 */
			Tokenizer& operator=(const Tokenizer&) = delete;

			/**
			 * @brief Default move assignment.
			 */
			Tokenizer& operator=(Tokenizer&&) = default;

			/**
			 * @brief Destructor.
			 */
			~Tokenizer() = default;

			/**
			 * @brief Returns the next token from the stream.
			 * @return The next Token. type == EndOfFile when finished.
			 */
			Token Next();

			/**
			 * @brief Returns the current line number (1-based).
			 * @return Current line number.
			 */
			unsigned int CurrentLine() const noexcept { return m_line; }

		private:
			std::istream&   m_stream;       ///< Underlying input stream
			unsigned int    m_line = 1;     ///< Current line number

			/**
			 * @brief Skips whitespace and updates the line counter.
			 */
			void SkipWhitespace();

			/**
			 * @brief Reads a quoted string (handles escapes).
			 * @return Token of type String or an error.
			 */
			Expected<Token, ParseError> ReadString();

			/**
			 * @brief Reads a binary literal (b"...").
			 *
			 * The content inside the quotes is expected to be Base64.
			 * The actual Base64 decoding is performed later by the Parser.
			 *
			 * @return Token of type Binary or an error.
			 */
			Expected<Token, ParseError> ReadBinary();

			/**
			 * @brief Reads a number (integer or double).
			 * @return Token of type Integer or Double.
			 */
			Token ReadNumber();

			/**
			 * @brief Reads an identifier or keyword (true/false).
			 * @return Token of type Identifier or Bool.
			 */
			Token ReadIdentifierOrKeyword();

			/**
			 * @brief Reads a single-line comment (# or //).
			 * @param type Comment subtype.
			 * @return Token of type Comment.
			 */
			Token ReadSingleLineComment(CommentType type);

			/**
			 * @brief Reads a multi-line comment (/* ... *​/).
			 * @return Token of type Comment or a ParseError if unclosed.
			 */
			Expected<Token, ParseError> ReadMultiLineComment();
	};
}