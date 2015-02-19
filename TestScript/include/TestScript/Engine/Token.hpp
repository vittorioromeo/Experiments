// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_TOKEN
#define TESTSCRIPT_ENGINE_TOKEN

namespace Eng
{
	template<typename TL> class Token
	{
		private:
			TokenType<TL> type;
			TokenData<TL> data;

		public:
			inline Token(const TokenType<TL>& mType) noexcept : type{mType} { }
			inline Token(const TokenType<TL>& mType, const TokenData<TL>& mData) : type{mType}, data{mData} { }

			inline const TokenType<TL>& getType() const noexcept { return type; }
			inline const TokenData<TL>& getData() const noexcept { return data; }
	};

	template<typename TL> class ASTTokenNode : public Node<TL>
	{
		private:
			Token<TL> token;

		public:
			inline ASTTokenNode(Token<TL> mToken) : token{std::move(mToken)} { }
			inline decltype(token)& getToken() noexcept { return token; }
	};
}

#endif
