// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_COMMON
#define TESTSCRIPT_ENGINE_COMMON

namespace Eng
{
	enum class RPType{Token, Node};

	using ASTTypeId = std::size_t;

	namespace Internal
	{
		inline ASTTypeId getNextASTTypeId() noexcept { static ASTTypeId lastId{0}; return lastId++; }
	}

	template<typename T> inline const ASTTypeId& getASTTypeId() noexcept { static ASTTypeId id{Internal::getNextASTTypeId()}; return id; }

	template<typename TTokenType, typename TTokenData> struct LangSpec
	{
		using TokenType = TTokenType;
		using TokenData	= TTokenData;
	};

	template<typename TL> using TokenType = typename TL::TokenType;
	template<typename TL> using TokenData = typename TL::TokenData;

	template<typename TL> class ASTNode;

	template<typename TL> using ASTNodePtr = ASTNode<TL>*;
	template<typename TL> using ASTNodeUptr = ssvu::Uptr<ASTNode<TL>>;
}

#endif
