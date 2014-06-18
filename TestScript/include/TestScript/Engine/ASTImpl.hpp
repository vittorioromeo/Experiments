// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_ASTIMPL
#define TESTSCRIPT_ENGINE_ASTIMPL

namespace Eng
{
	template<typename TL> class ASTTokenNode : public ASTNode<TL>
	{
		private:
			Token<TL> token;

		public:
			inline ASTTokenNode(Token<TL> mToken) : token{std::move(mToken)} { }
			inline decltype(token)& getToken() noexcept { return token; }
	};
}

#endif
