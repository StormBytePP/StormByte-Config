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

#include <StormByte/config/item/value.hxx>

#include <string>

/**
 * @brief Configuration items (values, comments, groups, lists).
 */
namespace StormByte::Config::Item {
	/**
	 * @class Comment
	 * @brief Comment item (`#`, `//` or `/* *\/`).
	 * @tparam T Comment syntax (`CommentType`).
	 */
	template<CommentType T>
	class STORMBYTE_CONFIG_PUBLIC Comment final: public Value<std::string> {
		public:
			/**
			 * @name Construction
			 * @{
			 */
			/**
			 * @brief Constructs a Comment with the given string.
			 * @param comment The comment string.
			 */
			Comment(const std::string& comment): Value<std::string>(comment) {}

			/**
			 * @brief Move constructor from comment string.
			 * @param comment Comment string.
			 */
			Comment(std::string&& comment): Value<std::string>(std::move(comment)) {}

			/**
			 * @brief Copy constructor.
			 * @param base Comment to copy.
			 */
			Comment(const Comment& base) = default;

			/**
			 * @brief Move constructor.
			 * @param base Comment to move.
			 */
			Comment(Comment&& base) = default;

			/**
			 * @brief Copy assignment operator.
			 * @param base Comment to copy.
			 * @return Reference to this Comment.
			 */
			Comment& operator=(const Comment& base) = default;

			/**
			 * @brief Move assignment operator.
			 * @param base Comment to move.
			 * @return Reference to this Comment.
			 */
			Comment& operator=(Comment&& base) = default;

			/**
			 * @brief Destructor.
			 */
			~Comment() noexcept override = default;
			/** @} */

			/**
			 * @name Access
			 * @{
			 */
			/**
			 * @brief Polymorphic equality comparison.
			 * @param other The other item to compare against.
			 * @return true if both are comments of the same type and content.
			 */
			bool Equals(const Base& other) const noexcept override {
				if (this->Type() != other.Type())
					return false;
				if (this->Name() != other.Name())
					return false;

				auto other_comment_type = other.GetCommentType();
				if (!other_comment_type || *other_comment_type != T)
					return false;

				const Comment<T>& other_comment = static_cast<const Comment<T>&>(other);
				return **this == *other_comment;
			}

			/**
			 * @brief Serializes the comment item.
			 * @param indent_level Indentation level.
			 * @return Serialized string.
			 */
			std::string Serialize(const int& indent_level) const noexcept override;

			/**
			 * @brief Gets the item type.
			 * @return Item type.
			 */
			constexpr Item::Type Type() const noexcept override {
				return Type::Comment;
			}

			/**
			 * @brief Returns the concrete comment type of this Comment specialization.
			 * @return The CommentType corresponding to the template parameter T.
			 */
			std::optional<CommentType> GetCommentType() const noexcept override {
				return T;
			}

			/**
			 * @brief Gets the comment syntax.
			 * @return Comment syntax.
			 */
			constexpr StormByte::Config::Item::CommentType CommentType() const noexcept {
				return T;
			}

			/**
			 * @brief Converts comment syntax to string.
			 * @return Comment syntax string.
			 */
			constexpr std::string CommentTypeToString() const noexcept {
				return Item::TypeToString(T);
			}

			/**
			 * @brief Clones the comment.
			 * @return Cloned comment.
			 */
			PointerType Clone() const override {
				return MakePointer<Comment<T>>(*this);
			}

			/**
			 * @brief Moves the comment.
			 * @return Moved comment.
			 */
			PointerType Move() override {
				return MakePointer<Comment<T>>(std::move(*this));
			}
			/** @} */
	};
}
