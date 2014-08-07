// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_PARSER
#define TESTSCRIPT_ENGINE_PARSER

namespace Eng
{
	template<typename TL> class Parser
	{
		template<typename> friend class Ctx;
		template<typename> friend class Rule;

		private:
			ssvu::PolyManager<Node<TL>> nodeManager;
			std::vector<NodePtr<TL>> sourceStack, parseStack;//, nodeStack;
			ssvu::VecUPtr<Rule<TL>> rules;

		public:
			inline Rule<TL>& createRule() { return ssvu::getEmplaceUPtr<Rule<TL>>(rules); }

			inline void shift()
			{
				SSVU_ASSERT(!sourceStack.empty());

				// Push source stack top node both on parse (and node stacks)(?)
				parseStack.emplace_back(sourceStack.back());

				// Pop it from source stack
				sourceStack.pop_back();
			}

			template<typename T, typename... TArgs> inline Node<TL>& createNode(TArgs&&... mArgs)
			{
				auto& result(nodeManager.template create<T>(std::forward<TArgs>(mArgs)...));
				nodeManager.refresh();

				result.typeId = Eng::getNodeTID<T>();
				return result;
			}

			inline void reduceRecursively()
			{
				for(const auto& r : rules)
				{
					for(auto i(0u); i < parseStack.size(); ++i)
					{
						Ctx<TL> ctx{*this, i};
						if(!r->matches(ctx)) continue;

						auto lookAheadResults(r->lookAhead(ctx));
						if(lookAheadResults.forceShift)
						{
							ssvu::lo()<<"lookahead shift"<<std::endl;
							shift();
							return;
						}
						else
						{
							r->action(ctx);
							reduceRecursively();
							return;
						}
					}
				}
			}

			inline void run(const std::vector<Token<TL>>& mTokens)
			{
				// Clear memory
				nodeManager.clear();

				// Reset parser state
				sourceStack.clear();
				parseStack.clear();

				// Create nodes for tokens and push them on the source stack
				for(const auto& t : mTokens)
				{
					auto& tokenNode(createNode<ASTTokenNode<TL>>(t));\
					sourceStack.emplace(std::begin(sourceStack), &tokenNode);
				}

				// Do magic!
				while(!sourceStack.empty())
				{
					shift();
					reduceRecursively();
				}
			}

			inline decltype(parseStack)& getParseStack() { return parseStack; }
			inline decltype(sourceStack)& getSourceStack() { return sourceStack; }
	};
}

#endif
