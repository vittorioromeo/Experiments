// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_COMMON
#define TESTSCRIPT_ENGINE_COMMON

namespace Eng
{
	using NodeTID = std::size_t;

	namespace Internal
	{
		inline NodeTID getNextNodeTID() noexcept { static NodeTID lastId{0}; return lastId++; }
	}

	template<typename T> inline const NodeTID& getNodeTID() noexcept { static NodeTID id{Internal::getNextNodeTID()}; return id; }

	template<typename TTokenType, typename TTokenData> struct LangSpec
	{
		using TokenType = TTokenType;
		using TokenData	= TTokenData;
	};

	template<typename TL> using TokenType = typename TL::TokenType;
	template<typename TL> using TokenData = typename TL::TokenData;

	template<typename TL> class Node;

	template<typename TL> using NodePtr = Node<TL>*;
	template<typename TL> using NodeUPtr = ssvu::UPtr<Node<TL>>;

	template<typename TL> class Parser;
}

#endif
