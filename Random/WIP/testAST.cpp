#include <SSVUtils/SSVUtils.hpp>

namespace Eng
{
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
	template<typename TL> class ASTImpl;

	template<typename TL> using ASTNodePtr = ASTNode<TL>*;
	template<typename TL> using ASTImplPtr = ASTImpl<TL>*;
	template<typename TL> using ASTNodeUptr = ssvu::Uptr<ASTNode<TL>>;
	template<typename TL> using ASTImplUptr = ssvu::Uptr<ASTImpl<TL>>;

	template<typename TL, typename T, typename... TArgs> inline ASTImplUptr<TL> createASTImpl(TArgs&&... mArgs);

	template<typename TL> class Token
	{
		private:
			TokenType<TL> type;
			TokenData<TL> data;

		public:
			inline Token(const TokenType<TL>& mType) noexcept : type{mType} { }
			inline Token(const TokenType<TL>& mType, const TokenData<TL>& mData) : Token{mType}, data{mData} { }

			inline const TokenType<TL>& getType() const noexcept { return type; }
			inline const TokenData<TL>& getData() const noexcept { return data; }
	};

	template<typename TL> class ASTImpl 
	{
		template<typename> friend class ASTNode;
		template<typename TL2, typename T, typename... TArgs> friend ASTImplUptr<TL2> createASTImpl(TArgs&&... mArgs); 

		private:
			ASTTypeId typeId;
			ASTNodePtr<TL> node{nullptr};

		public:
			inline virtual ~ASTImpl() { }
			inline ASTNode<TL>& getNode() const noexcept 
			{ 
				SSVU_ASSERT(node != nullptr);
				return *node; 
			}
	};

	template<typename TL> class ASTTokenNodeImpl : public ASTImpl<TL>
	{
		private:
			Token<TL> token;

		public:
			inline ASTTokenNodeImpl(Token<TL> mToken) : token{std::move(mToken)} { }
			inline decltype(token)& getToken() noexcept { return token; }
	};

	template<typename TL> class ASTNode
	{
		private:
			ASTImplUptr<TL> impl;
			ASTNodePtr<TL> parent{nullptr};
			std::vector<ASTNodePtr<TL>> children;

		public:
			template<typename T> inline void setImpl(T mImpl) { impl = std::move(mImpl); impl->node = this; }

			inline ASTTypeId getTypeId() const noexcept { return impl->typeId; }

			inline void emplaceChild(ASTNodePtr<TL> mNode)
			{
				mNode->parent = this;				
				children.emplace_back(mNode);				
			}	

			inline ASTImpl<TL>* getImpl() noexcept 				{ return impl.get(); }
			inline ASTNodePtr<TL> getParent() noexcept 			{ return parent; }
			inline decltype(children)& getChildren() noexcept 	{ return children; }

			template<typename T> T& getImplAs() noexcept 
			{
				SSVU_ASSERT(impl != nullptr);
				return *reinterpret_cast<T*>(impl.get());
			}
	};

	enum class RPType{Token, Node};

	template<typename TL> class RulePart
	{
		private:
			RPType type;
			union
			{
				TokenType<TL> tokenType;
				ASTTypeId astTypeId;
			};

		public:
			inline RulePart(TokenType<TL> mTokenType) noexcept : type{RPType::Token}, tokenType{mTokenType} { } 
			inline RulePart(ASTTypeId mASTTypeId) noexcept : type{RPType::Node}, astTypeId{mASTTypeId} { }

			inline RPType getType() const noexcept 							{ return type; }
			inline const decltype(tokenType)& getTokenType() const noexcept	{ SSVU_ASSERT(type == RPType::Token); return tokenType; }
			inline ASTTypeId getASTTypeId() const noexcept 					{ SSVU_ASSERT(type == RPType::Node); return astTypeId; }

			inline bool matchesNode(ASTNodePtr<TL> mNode) const 
			{
				if(mNode->getTypeId() == Eng::getASTTypeId<ASTTokenNodeImpl<TL>>())
				{	
					if(type != RPType::Token) return false;

					auto& pTokenImpl(mNode->template getImplAs<ASTTokenNodeImpl<TL>>());
					if(tokenType != pTokenImpl.getToken().getType()) return false;
				}
				else
				{						
					if(type != RPType::Node) return false;
					if(astTypeId != mNode->getTypeId()) return false;
				}	

				return true;
			}

			inline bool operator==(const RulePart<TL>& mRhs) const noexcept
			{
				if(type != mRhs.type) return false;
				if(type == RPType::Token && tokenType == mRhs.tokenType) return true;
				if(type == RPType::Node && astTypeId == mRhs.astTypeId) return true;
				return false;
			}
			inline bool operator!=(const RulePart<TL>& mRhs) const noexcept { return !this->operator==(mRhs); }
	};

	template<typename TL> class RuleKey
	{
		private:
			std::vector<RulePart<TL>> ruleParts;

			template<typename T> inline void ctorImpl(T mP) { ruleParts.emplace_back(mP); }
			template<typename T1, typename T2, typename... TArgs> inline void ctorImpl(T1 mP1, T2 mP2, TArgs... mArgs) 
			{ 
				ctorImpl(mP1); ctorImpl(mP2, mArgs...); 
			}

			template<std::size_t TIdx> inline bool matchesImpl(const RulePart<TL>& mPart) const { return mPart == ruleParts.at(TIdx); }
			template<std::size_t TIdx, typename T1, typename T2, typename... TArgs> inline bool matchesImpl(T1 mP1, T2 mP2, TArgs... mArgs) const
			{	
				return !matchesImpl<TIdx>(mP1) ? false : matchesImpl<TIdx + 1>(mP2, mArgs...);
			}
		
		public:
			template<typename... TArgs> inline RuleKey(TArgs... mArgs)
			{
				ctorImpl(mArgs...);
			}

			template<typename... TArgs> inline bool matches(TArgs... mArgs)
			{			
				return sizeof...(TArgs) != ruleParts.size() ? false : matchesImpl<0>(mArgs...);
			}

			inline std::size_t getSize() const noexcept { return ruleParts.size(); }

			inline const RulePart<TL>& getPartAt(std::size_t mIdx) const noexcept 
			{ 
				SSVU_ASSERT(ruleParts.size() > mIdx);
				return ruleParts[mIdx]; 
			}
	};	

	template<typename TL> class NodeCtx
	{
		private:
			std::vector<ASTNodePtr<TL>>& usedNodes;

		public:
			inline NodeCtx(decltype(usedNodes)& mUsedNodes) : usedNodes(mUsedNodes) { }
			template<typename T> T& get(std::size_t mIdx) noexcept
			{
				SSVU_ASSERT(mIdx < usedNodes.size());
				return usedNodes[mIdx]->template getImplAs<T>();
			}
	};	

	template<typename TL> class Rule
	{
		public:
			using RuleFunc = ssvu::Func<ASTImplUptr<TL>(NodeCtx<TL>&)>;

		private:
			RuleKey<TL> ruleKey;
			ASTTypeId result;
			RuleFunc func;

		public:
			inline Rule(RuleKey<TL> mKey, const RuleFunc& mFunc) : ruleKey{std::move(mKey)}, func{mFunc} { }
			
			inline std::size_t getSize() const noexcept { return ruleKey.getSize(); }
			inline const RulePart<TL>& getPartAt(std::size_t mIdx) const noexcept { return ruleKey.getPartAt(mIdx); }
			inline RuleFunc& getFunc() noexcept { return func; }
	};

	template<typename TL> class RuleSet
	{
		private:
			std::vector<ssvu::Uptr<Rule<TL>>> rules;
	
		public:
			template<typename... TArgs> inline Rule<TL>& createRule(TArgs&&... mArgs)
			{
				return ssvu::getEmplaceUptr<Rule<TL>>(rules, std::forward<TArgs>(mArgs)...);
			}

			inline const decltype(rules)& getRules() const noexcept { return rules; }
	};


	template<typename TL, typename T, typename... TArgs> inline ASTImplUptr<TL> createASTImpl(TArgs&&... mArgs)
	{
		auto ptr(new T(std::forward<TArgs>(mArgs)...));
		ptr->typeId = Eng::getASTTypeId<T>();
		return ASTImplUptr<TL>(ptr);
	}	

	template<typename TL> class Parser
	{
		private:
			std::vector<ssvu::Uptr<ASTNode<TL>>> nodeManager;

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

			inline void makeReduction(const Rule<TL>& mRule)
			{
				
			}

			inline void reduceRecursively()
			{
				for(const auto& r : ruleSet.getRules())
				{
					//ssvu::lo("matches?") << r.getParselets() << std::endl;

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
						auto& reductionResult(ssvu::getEmplaceUptr<ASTNode<TL>>(nodeManager));
						
						NodeCtx<TL> nodeCtx{usedNodes};
						reductionResult.setImpl(std::move(r->getFunc()(nodeCtx)));	

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
					auto tokenImpl(createASTImpl<TL, ASTTokenNodeImpl<TL>>(t));
					auto& tokenNode(ssvu::getEmplaceUptr<ASTNode<TL>>(nodeManager));				
					tokenNode.setImpl(std::move(tokenImpl));
	
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

namespace Lang
{
	enum class Tkn 
	{
		Number,
		POpen,
		PClose,
		OpPlus,
		OpMinus,
		OpMult,
		OpDiv,
		OpMod
	};	

	struct TknData { };

	using Spec = Eng::LangSpec<Tkn, TknData>;

	struct ASTExpr : public Eng::ASTImpl<Spec>
	{	
		inline virtual std::string getName() { return "ASTExpr"; }
		inline virtual int eval() 
		{ 
			auto& child(getNode().getChildren()[0]->getImplAs<ASTExpr>());
			return child.eval();	
		}
		inline virtual void toBytecode() 
		{ 
			auto& child(getNode().getChildren()[0]->getImplAs<ASTExpr>());
			child.toBytecode();	
		}
	};
			

	struct ASTNumber : public ASTExpr
	{
		int value;

		inline ASTNumber(int mValue) : value{mValue} { }

		inline std::string getName() override { return "ASTNumber"; }
		inline int eval() override 
		{ 
			return value;
		}

		inline void toBytecode() override { ssvu::lo() << "push Number" << std::endl; }
	};

	struct ASTParenthesizedExpr : public ASTExpr
	{
		ASTExpr* innerExpr{nullptr};

		inline ASTParenthesizedExpr(ASTExpr& mInnerExpr) : innerExpr{&mInnerExpr} { }

		inline std::string getName() override { return "ASTParenthesizedExpr"; }
		inline int eval() override { return innerExpr->eval(); }
		inline void toBytecode() override { innerExpr->toBytecode(); }
	};

	template<typename T> struct OpAddition 			{ inline static T get(const T& mA, const T& mB) noexcept { return mA + mB; } };
	template<typename T> struct OpSubtraction 		{ inline static T get(const T& mA, const T& mB) noexcept { return mA - mB; } };
	template<typename T> struct OpMultiplication 	{ inline static T get(const T& mA, const T& mB) noexcept { return mA * mB; } };
	template<typename T> struct OpDivision 			{ inline static T get(const T& mA, const T& mB) noexcept { return mA / mB; } };
	template<typename T> struct OpRemainder 		{ inline static T get(const T& mA, const T& mB) noexcept { return mA % mB; } };	

	template<typename TOp> struct ASTBinaryOp : public ASTExpr
	{
		ASTExpr* lhs{nullptr};
		ASTExpr* rhs{nullptr};

		inline ASTBinaryOp(ASTExpr& mLhs, ASTExpr& mRhs) : lhs{&mLhs}, rhs{&mRhs} { }

		inline std::string getName() override { return "ASTBinaryOp"; }
		inline int eval() override 
		{ 
			ssvu::lo("+EXP") << std::endl;
			return TOp::get(lhs->eval(), rhs->eval());
		}

		inline void toBytecode() override { lhs->toBytecode(); rhs->toBytecode(); ssvu::lo() << "pop Sum" << std::endl; }
	};
}

template<bool TFmt, typename TN> inline void printNode(std::ostream& mStream, TN& mValue, int mDepth)
{	
	for(auto i(0u); i < mDepth; ++i) ssvu::Internal::callStringifyImpl<TFmt>(mStream, "|\t");

	ssvu::Internal::callStringifyImpl<TFmt>(mStream, "L______o> ");	    

	//ssvu::Internal::callStringifyImpl<TFmt>(mStream, mValue.getTypeId() < -1 ? mValue.template getImplAs<Lang::ASTExpr>().getName() 
	//	: ssvu::toStr(int(mValue.template getImplAs<Eng::ASTTokenNodeImpl<Lang::Spec>>().getToken().getType())));	       	

//ssvu::Internal::callStringifyImpl<TFmt>(mStream, mValue.getTypeId());	       	
	ssvu::Internal::callStringifyImpl<TFmt>(mStream, "\n");

	for(auto i(0u); i < mValue.getChildren().size(); ++i) 
	{
		printNode<TFmt>(mStream, *(mValue.getChildren().at(i)), mDepth + 1);
	 	if(i == mValue.getChildren().size() - 1)
	 	{
	 		for(auto i(0u); i < mDepth + 1; ++i) ssvu::Internal::callStringifyImpl<TFmt>(mStream, "|\t");
			ssvu::Internal::callStringifyImpl<TFmt>(mStream, "\n");			 			
	 	}
	}
}

int main() 
{ 
	using namespace Eng;
	using namespace Lang;


	Parser<Spec> parser;
	auto& ruleSet(parser.getRuleSet());

	ruleSet.createRule(RuleKey<Spec>{Tkn::Number}, [](NodeCtx<Spec>& mCtx)
	{
		ssvu::lo("NUMB") << "" << std::endl;
		return createASTImpl<Spec, ASTNumber>(15);
	});
	ruleSet.createRule(RuleKey<Spec>{Tkn::POpen, getASTTypeId<ASTExpr>(), Tkn::PClose}, [](NodeCtx<Spec>& mCtx)
	{	
		ssvu::lo("PEXPR") << "" << std::endl;
		return createASTImpl<Spec, ASTParenthesizedExpr>(mCtx.get<ASTExpr>(1));
	});
	ruleSet.createRule(RuleKey<Spec>{getASTTypeId<ASTNumber>()}, [](NodeCtx<Spec>& mCtx)
	{	
		ssvu::lo("EXPR") << "" << std::endl;
		return createASTImpl<Spec, ASTExpr>();
	});
	ruleSet.createRule(RuleKey<Spec>{getASTTypeId<ASTBinaryOp<OpAddition<int>>>()}, [](NodeCtx<Spec>& mCtx)
	{	
		ssvu::lo("EXPR") << "" << std::endl;
		return createASTImpl<Spec, ASTExpr>();
	});
	ruleSet.createRule(RuleKey<Spec>{getASTTypeId<ASTExpr>(), Tkn::OpPlus, getASTTypeId<ASTExpr>()}, [](NodeCtx<Spec>& mCtx)
	{	
		ssvu::lo("PLUSEXPR") << "" << std::endl;
		return createASTImpl<Spec, ASTBinaryOp<OpAddition<int>>>(mCtx.get<ASTExpr>(0), mCtx.get<ASTExpr>(2));
	});
	
// figure a way of not having to specify all inherited class of a certain class for expansions

	std::vector<Token<Spec>> src
	{
		{Tkn::POpen},
		{Tkn::Number},
		{Tkn::OpPlus},
		{Tkn::Number},
		{Tkn::PClose},
		{Tkn::OpPlus},
		{Tkn::POpen},
		{Tkn::Number},
		{Tkn::OpPlus},
		{Tkn::Number},
		{Tkn::PClose}
	};

	parser.run(src);

	auto& b(parser.getNodeStack().back());
	ssvu::lo("RESULT") <<	b->getImplAs<ASTExpr>().eval() << std::endl;
	//ssvu::lo() << b->getAsNode().getNode().getChildren().size() << std::endl;
	//ssvu::lo() << int(b->getAsNode().getNode().getType());
	b->getImplAs<ASTExpr>().toBytecode();

	printNode<true>(std::cout, *b, 0);

	ssvu::lo() << std::endl;

	return 0;
}

/*
OpMinus,
		OpMult,
		OpDiv,
		OpMod*/