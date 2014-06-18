// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_PARSER
#define TESTSCRIPT_ENGINE_PARSER

namespace Eng
{
	template<typename> class Parser;

	template<typename TL> class Ctx
	{
		template<typename> friend class Parser;

		private:
			Parser<TL>& parser;
			std::size_t offset;

			class Proxy
			{
				private:
					Ctx<TL>& ctx;
					ASTNodePtr<TL> nodePtr;

				public:
					inline Proxy(Ctx<TL>& mCtx, ASTNodePtr<TL> mNodePtr) : ctx(mCtx), nodePtr{mNodePtr} { }
					inline bool isTknNode(TokenType<TL> mTknType)
					{
						if(dynamic_cast<ASTTokenNode<TL>*>(nodePtr))
						{
							auto tknNode(reinterpret_cast<ASTTokenNode<TL>*>(nodePtr));
							if(tknNode->getToken().getType() == mTknType) return true;
						}

						return false;
					}
					template<typename T> inline bool isDerivedFrom()
					{
						return dynamic_cast<T*>(nodePtr);
					}

			};

		public:
			inline Ctx(Parser<TL>& mParser, std::size_t mOffset) : parser(mParser), offset{mOffset} { }

			inline std::size_t getSize() const noexcept { return parser.parseStack.size() - offset; }
			inline Proxy operator[](std::size_t mIdx) { return Proxy{*this, parser.parseStack[mIdx + offset]}; }
			inline void pop()
			{
				parser.parseStack.erase(std::begin(parser.parseStack) + offset);
			}
			template<typename T> inline T& popAs()
			{
				auto itr(std::begin(parser.parseStack) + offset);
				auto result(reinterpret_cast<T*>(*itr));
				pop();
				return *result;
			}
			template<typename T, typename... TArgs> inline T& pushCreate(TArgs&&... mArgs)
			{
				auto& node(parser.template createNode<T>(std::forward<TArgs>(mArgs)...));
				parser.parseStack.insert(std::begin(parser.parseStack) + offset, &node);
				return reinterpret_cast<T&>(node);
			}


			inline Proxy getAhead(std::size_t mIdx)
			{
				if(parser.parseStack.size() > mIdx + offset)
				{
					return Proxy{*this, parser.parseStack[mIdx + offset]};
				}
				else
				{
					int kk = mIdx + offset;
					ssvu::lo() << kk << std::endl;
					return Proxy{*this, parser.sourceStack[kk]};
				}
			}

			inline bool canLookAhead(std::size_t mIdx) const noexcept
			{
				int kk = mIdx + offset - parser.parseStack.size() + 1;
				return parser.sourceStack.size() > kk;
			}

	};

	struct LookAheadResults
	{
		bool forceShift{false};
	};

	template<typename TL> class Rule
	{
		private:
			ssvu::Func<bool(Ctx<TL>&)> fnPredicate;
			ssvu::Func<void(Ctx<TL>&)> fnAction;
			ssvu::Func<void(Ctx<TL>&, LookAheadResults&)> fnLookAhead;

		public:
			template<typename T> inline void setPredicate(const T& mFnPredicate) { fnPredicate = mFnPredicate; }
			template<typename T> inline void setAction(const T& mFnAction) { fnAction = mFnAction; }
			template<typename T> inline void setLookAhead(const T& mFnLookAhead) { fnLookAhead = mFnLookAhead; }

			inline bool matches(Ctx<TL>& mCtx) const { return fnPredicate(mCtx); }
			inline void action(Ctx<TL>& mCtx) { fnAction(mCtx); }
			inline LookAheadResults lookAhead(Ctx<TL>& mCtx)
			{
				LookAheadResults result;
				if(fnLookAhead != nullptr) fnLookAhead(mCtx, result);
				return result;
			}
	};
}

namespace Eng
{
	template<typename TL> class Parser
	{
		template<typename> friend class Ctx;
		template<typename> friend class Rule;

		private:
			ssvu::PolyManager<ASTNode<TL>> nodeManager;
			std::vector<ASTNodePtr<TL>> sourceStack, parseStack;//, nodeStack;
			ssvu::VecUptr<Rule<TL>> rules;

		public:
			inline Rule<TL>& createRule() { rules.emplace_back(new Rule<TL>{}); return *rules.back(); }

			inline void shift()
			{
				SSVU_ASSERT(!sourceStack.empty());

				// Push source stack top node both on parse and node stacks
				parseStack.emplace_back(sourceStack.back());

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
	};
}

#endif
