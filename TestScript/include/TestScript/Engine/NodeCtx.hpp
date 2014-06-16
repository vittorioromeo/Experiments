// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_NODECTX
#define TESTSCRIPT_ENGINE_NODECTX

namespace Eng
{
	template<typename> class Parser;

	template<typename TL> class NodeCtx
	{
		private:
			Parser<TL>& parser;
			std::vector<ASTNodePtr<TL>>& usedNodes;

		public:
			inline NodeCtx(Parser<TL>& mParser, decltype(usedNodes)& mUsedNodes) : parser(mParser), usedNodes(mUsedNodes) { }
			template<typename T> T& get(std::size_t mIdx) noexcept
			{
				SSVU_ASSERT(mIdx < usedNodes.size());
				return usedNodes[mIdx]->template getAs<T>();
			}

			template<typename T, typename... TArgs> inline ASTNode<TL>& createReduction(TArgs&&... mArgs)
			{
				return parser.template createNode<T>(std::forward<TArgs>(mArgs)...);
			}
	};
}

#endif
