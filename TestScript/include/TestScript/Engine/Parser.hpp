// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_PARSER
#define TESTSCRIPT_ENGINE_PARSER

namespace Eng
{
	template<typename TL> class Parser
	{
		template<typename> friend class NodeCtx;

		private:
			ssvu::PolyManager<ASTNode<TL>> nodeManager;

			RuleSet<TL> ruleSet;
			std::vector<ASTNodePtr<TL>> sourceStack, parseStack, nodeStack;

		public:
			inline decltype(ruleSet)& getRuleSet() noexcept { return ruleSet; }

			inline void shift()
			{
				SSVU_ASSERT(!sourceStack.empty());

				// Push source stack top node both on parse and node stacks
				parseStack.emplace_back(sourceStack.back());
				nodeStack.emplace_back(sourceStack.back());

				// Pop it from source stack
				sourceStack.pop_back();
			}

			inline bool parseStackMatchesRule(std::size_t mStartAt, const Rule<TL>& mRule)
			{
				const auto& expSize(mRule.getSize());
				if(expSize + mStartAt > parseStack.size()) return false;

				for(auto i(0u); i < expSize; ++i)
				{
					ASTNodePtr<TL> nodeToCheck(parseStack[mStartAt + i]);
					if(!mRule.getPartAt(i).matchesNode(nodeToCheck)) return false;

				}

				return true;
			}

			template<typename T, typename... TArgs> inline ASTNode<TL>& createNode(TArgs&&... mArgs)
			{
				auto& result(nodeManager.template create<T>(std::forward<TArgs>(mArgs)...));
				nodeManager.refresh();

				result.typeId = Eng::getASTTypeId<T>();
				return result;
			}

			inline void makeReduction(const Rule<TL>& mRule)
			{

			}

			inline void reduceRecursively()
			{
				for(const auto& r : ruleSet.getRules())
				{
					for(auto i(0u); i < parseStack.size(); ++i)
					{
						if(!parseStackMatchesRule(i, *r)) continue;

						ssvu::lo("matches!") << "yes!" << std::endl;

						// Remove base nodes from parse stack (they will be substitued with the reduction result)
						std::vector<ASTNodePtr<TL>> usedNodes;
						for(auto i(0u); i < r->getSize(); ++i)
						{
							usedNodes.emplace(std::begin(usedNodes), parseStack.back());
							parseStack.pop_back();
						}

						// Create reduction result node
						NodeCtx<TL> nodeCtx{*this, usedNodes};
						ASTNode<TL>& reductionResult(r->getFunc()(nodeCtx));

						// Remove base nodes from node stack (they will become children of the redution result node)
						std::vector<ASTNodePtr<TL>> removedNodes;
						for(auto i(0u); i < r->getSize(); ++i)
						{
							removedNodes.emplace_back(nodeStack.back());
							nodeStack.pop_back();
						}

						// Add removed nodes as children
						for(const auto& n : removedNodes)
						{
							reductionResult.emplaceChild(n);
						}

						// Push the reduction result node on the stacks
						parseStack.emplace_back(&reductionResult);
						nodeStack.emplace_back(&reductionResult);

						// Pop N nodes from node stack into removedNodes, then try reducing further
						reduceRecursively(); return;
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
				nodeStack.clear();

				// Create nodes for tokens and push them on the source stack
				for(const auto& t : mTokens)
				{
					auto& tokenNode(createNode<ASTTokenNodeImpl<TL>>(t));\
					sourceStack.emplace(std::begin(sourceStack), &tokenNode);
				}

				// Do magic!
				while(!sourceStack.empty())
				{
					shift();
					reduceRecursively();
				}
			}

			inline decltype(nodeStack)& getNodeStack() { return nodeStack; }
	};
}

#endif
