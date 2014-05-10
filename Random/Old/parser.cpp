#include <iostream>
#include <SSVUtils/SSVUtils.hpp>

namespace Engine 
{
	template<typename TT, typename TG, typename TTD> struct Language
	{
		using Tkn = TT; 
		using Gmr = TG;
		using TknData = TTD;
	};

	template<typename TL> using TknT = typename TL::Tkn;
	template<typename TL> using GmrT = typename TL::Gmr;
	template<typename TL> using TknDataT = typename TL::TknData;

	enum class PsltType{Tkn, Gmr};

	class AST 
	{
		private:
			std::vector<ssvu::Uptr<AST>> children;

		public:
			inline virtual ~AST() { }

			template<typename T, typename... TArgs> T& emplaceChild(TArgs&&... mArgs) 
			{ 			
				return ssvu::getEmplaceUptr<T>(children, std::forward<TArgs>(mArgs)...);
			}

			inline const decltype(children)& getChildren() const noexcept { return children; }
			inline decltype(children)& getChildren() noexcept { return children; }
			template<typename T> T& getChildAs(std::size_t mIdx) { return reinterpret_cast<T&>(*children[mIdx]); }
	};

	template<typename TL> class Parselet
	{
		private:
			PsltType type;
			union { TknT<TL> tkn; GmrT<TL> gmr; };

		public:		
			TknDataT<TL> data;

			inline Parselet() noexcept = default;
			inline Parselet(TknT<TL> mTkn) noexcept : type{PsltType::Tkn}, tkn{mTkn} { }
			inline Parselet(GmrT<TL> mGmr) noexcept : type{PsltType::Gmr}, gmr{mGmr} { }

			inline PsltType getType() const noexcept { return type; }
			inline TknT<TL> getTkn() const noexcept { assert(type == PsltType::Tkn); return tkn; }
			inline GmrT<TL> getGmr() const noexcept { assert(type == PsltType::Gmr); return gmr; }

			inline bool operator==(const Parselet& mP) const noexcept 
			{ 			
				if(type != mP.type) return false;
				if(type == PsltType::Tkn && tkn == mP.tkn) return true;	
				if(type == PsltType::Gmr && gmr == mP.gmr) return true;
				return false;
			}
			inline bool operator!=(const Parselet& mP) const noexcept { return !(*this == mP); }
	};

	template<typename TL> class GmrExpansion
	{ 
		public:
			using FuncType = std::function<std::unique_ptr<AST>()>;

		private:
			FuncType func;
			std::vector<Parselet<TL>> parselets;

			template<typename T1, typename T2, typename... TArgs> inline void createParselet(T1 mArg1, T2 mArg2, TArgs... mArgs)
			{
				createParselet<T1>(mArg1);
				createParselet<T2, TArgs...>(mArg2, mArgs...);
			}
			template<typename T> inline void createParselet(T mArg) { parselets.emplace_back(mArg); }

		public:
			template<typename... TArgs> inline GmrExpansion(const FuncType& mFunc, TArgs... mArgs) noexcept : func{mFunc} { createParselet(mArgs...); }

			inline FuncType& getFunc() noexcept { return func; }
			inline const decltype(parselets)& getParselets() const noexcept { return parselets; }
	};

	template<typename TL> class GmrRules
	{
		private:
			std::vector<GmrExpansion<TL>> expansions;

		public:
	 		template<typename... TArgs> inline void createExpansion(TArgs&&... mArgs)
	 		{
	 			expansions.emplace_back(std::forward<TArgs>(mArgs)...);
	 		}
			inline const decltype(expansions)& getExpansions() const noexcept { return expansions; }
	};

	template<typename TL> class Node
	{
		public:
			using ParseletType = Parselet<TL>;

		private:
			Parselet<TL> parselet;
			Node* parent{nullptr};
			std::vector<ssvu::Uptr<Node>> children;
			int depth{0};

		public:
			template<typename... TArgs> inline Node(TArgs&&... mArgs) : parselet(std::forward<TArgs>(mArgs)...) { }
			template<typename... TArgs> inline Node& createChild(TArgs&&... mArgs)
			{
				auto& result(ssvu::getEmplaceUptr<Node>(children, std::forward<TArgs>(mArgs)...));
				result.depth = depth + 1;
				result.parent = this;
				return result;
			}
			inline const decltype(parselet)& getParselet() const noexcept { return parselet; }
			inline const decltype(children)& getChildren() const noexcept { return children; } 
	};




	template<typename TL> class Parser
	{
		public:
			using ParseletType = Parselet<TL>;
			using NodeType = Node<TL>;
			GmrRules<TL> rules;

		private: 
			std::vector<ParseletType> tokenStack, parseStack;
			std::vector<NodeType> nodeStack;

			inline void shift()
			{
				// Pop a token from tokenStack and push it on parseStack and nodeStack
				parseStack.emplace_back(tokenStack.back());
				nodeStack.emplace_back(tokenStack.back());
				tokenStack.pop_back();
			}

			inline bool parseStackMatchesExpansion(std::size_t mStartAt, const GmrExpansion<TL>& mGmrExpansion)
			{
				const auto& expSize(mGmrExpansion.getParselets().size());
				if(expSize + mStartAt > parseStack.size()) return false;
				
				for(auto i(0u); i < expSize; ++i) 
					if(mGmrExpansion.getParselets().at(i) != parseStack[mStartAt + i]) 
						return false;			

				return true;
			}

			inline void makeReduction(const GmrExpansion<TL>& mRule)
			{
				std::vector<NodeType> removedNodes;
				for(auto k(0u); k < mRule.getParselets().size(); ++k)
				{
					NodeType node{std::move(nodeStack.back())};
					removedNodes.emplace_back(std::move(node));
					nodeStack.pop_back();							
				}
				 
				// Create new node for reduction
				nodeStack.emplace_back(mRule.getGmr());

				// Add removed nodes as children
				for(auto& n : removedNodes) nodeStack.back().createChild(std::move(n));
			}

			inline void reduceRecursively()
			{
				debugPrint();

				for(const auto& r : rules.getExpansions())
				{
					ssvu::lo("matches?") << r.getParselets() << std::endl;

					for(auto i(0u); i < parseStack.size(); ++i)
					{
						if(!parseStackMatchesExpansion(i, r)) continue;
						
						ssvu::lo("matches!") << "yes!" << std::endl;

						// Remove base nodes from parse stack and substitue them with exapansion
						for(auto i(0u); i < r.getParselets().size(); ++i) parseStack.pop_back();
						parseStack.emplace_back(r.getGmr()); 	

						// Pop N nodes from node stack into removedNodes, then try reducing further
						makeReduction(r); reduceRecursively(); return;						
					}
				}

				debugPrint();
			}

			inline void debugPrint()
			{
				ssvu::lo("tokenStack") 	<< tokenStack << "\n";
				ssvu::lo("parseStack") 	<< parseStack << "\n";
				ssvu::lo("nodeStack") 	<< "\n" << nodeStack << "\n" << std::endl;
			}

		public:
			template<typename... TArgs> inline void createRule(TArgs... mArgs) { rules.createExpansion(mArgs...); }	

			inline void parse(const std::vector<TknT<TL>>& mTkns)
			{
				// Reset parser state
				tokenStack.clear();
				parseStack.clear();
				nodeStack.clear();

				// Push all tokens on the token stack
				for(const auto& t : mTkns) tokenStack.emplace(std::begin(tokenStack), t);

				while(!tokenStack.empty())
				{
					shift();
					reduceRecursively();
				}
			}
	};


}	

namespace Lang
{
	enum class Token{Num, POpen, PClose, OpAdd, OpSub};
	enum class Grammar{Expr, Op};
	using Tkn = Token;
	using Gmr = Grammar;

	struct TknData
	{
		int data{1};
	};


	using LangSpec = Engine::Language<Tkn, Gmr, TknData>;
}


SSVUT_TEST(ParseletTests)
{
	Engine::Parselet<Lang::LangSpec> pt{Lang::Tkn::Num};
	Engine::Parselet<Lang::LangSpec> pg{Lang::Gmr::Expr};

	SSVUT_EXPECT(pt.getType() == Engine::PsltType::Tkn);
	SSVUT_EXPECT(pg.getType() == Engine::PsltType::Gmr);
}

SSVUT_TEST(GmrExpansionTests)
{
	Engine::GmrExpansion<Lang::LangSpec> gmr1{Lang::Gmr::Expr, Lang::Gmr::Expr, Lang::Gmr::Op, Lang::Gmr::Op};
	Engine::GmrExpansion<Lang::LangSpec> gmr2{Lang::Gmr::Expr, Lang::Tkn::POpen, Lang::Gmr::Op, Lang::Tkn::PClose};
	Engine::GmrExpansion<Lang::LangSpec> gmr3{Lang::Gmr::Expr, Lang::Gmr::Op};

	SSVUT_EXPECT(gmr1.getParselets().size() == 3);
	SSVUT_EXPECT(gmr2.getParselets().size() == 3);
	SSVUT_EXPECT(gmr3.getParselets().size() == 1);
}

SSVUT_TEST(GmrRulesTests)
{
	Engine::GmrRules<Lang::LangSpec> gr1;
	gr1.createExpansion(Lang::Gmr::Expr, Lang::Gmr::Expr, Lang::Gmr::Op, Lang::Gmr::Op);
	gr1.createExpansion(Lang::Gmr::Expr, Lang::Gmr::Op);
	gr1.createExpansion(Lang::Gmr::Op, Lang::Gmr::Op);
	SSVUT_EXPECT(gr1.getExpansions().size() == 3); 
}




// GRAMMAR:
//
//	Expr 	-> 	Expr Op Expr
//			->	POpen Expr PClose
//  		-> 	Num
//
// 	Op 		->	OpAdd
//			->	OpSub

namespace ssvu
{
	using namespace Engine;
	using namespace Lang;

	template<> struct Stringifier<Tkn> 
	{ 
		template<bool TFmt> inline static void impl(std::ostream& mStream, const Tkn& mValue)
		{
			std::string s;
			switch(mValue)
			{
				case Tkn::Num: 		s = "NUM"; 	break;
				case Tkn::OpAdd: 	s = "+"; 	break;
				case Tkn::OpSub: 	s = "-"; 	break;
				case Tkn::POpen: 	s = "("; 	break;
				case Tkn::PClose: 	s = ")"; 	break;
			}

			Internal::printNonBold<TFmt>(mStream, s, Console::Color::Yellow);
		}
	};
	template<> struct Stringifier<Gmr> 
	{ 
		template<bool TFmt> inline static void impl(std::ostream& mStream, const Gmr& mValue)
		{
			std::string s;
			switch(mValue)
			{
				case Gmr::Expr:	s = "EXPR";	break;
				case Gmr::Op: 	s = "OP"; 	break;
			}

			Internal::printNonBold<TFmt>(mStream, s, Console::Color::Red);
		}
	};
	template<typename TL> struct Stringifier<Parselet<TL>> 
	{ 
		template<bool TFmt> inline static void impl(std::ostream& mStream, const Parselet<TL>& mValue)
		{
			if(mValue.getType() == PsltType::Tkn) 
			{
				Internal::callStringifyImpl<TFmt>(mStream, Engine::TknT<TL>(mValue.getTkn()));			
			}
			else if(mValue.getType() == PsltType::Gmr) 
			{
				Internal::callStringifyImpl<TFmt>(mStream, Engine::GmrT<TL>(mValue.getGmr()));
			}
		}
	};
	template<typename TL> struct Stringifier<Node<TL>> 
	{ 
		template<bool TFmt> inline static void printNode(std::ostream& mStream, const Node<TL>& mValue, int mDepth)
		{	
			for(auto i(0u); i < mDepth; ++i) Internal::callStringifyImpl<TFmt>(mStream, "|\t");

			Internal::callStringifyImpl<TFmt>(mStream, "L______o> ");	    
	       	Internal::callStringifyImpl<TFmt>(mStream, mValue.getParselet());	       	
	       	Internal::callStringifyImpl<TFmt>(mStream, "\n");

			for(auto i(0u); i < mValue.getChildren().size(); ++i) 
			{
				Stringifier<Node<TL>>::printNode<TFmt>(mStream, *(mValue.getChildren().at(i).get()), mDepth + 1);
			 	if(i == mValue.getChildren().size() - 1)
			 	{
			 		for(auto i(0u); i < mDepth + 1; ++i) Internal::callStringifyImpl<TFmt>(mStream, "|\t");
					Internal::callStringifyImpl<TFmt>(mStream, "\n");			 			
			 	}
			}
	  	}	

		template<bool TFmt> inline static void impl(std::ostream& mStream, const Node<TL>& mValue)
		{
			Internal::callStringifyImpl<TFmt>(mStream, "\n\n");

			std::string derp{"\t"};
			Stringifier<Node<TL>>::printNode<TFmt>(mStream, mValue, 0);
		}
	};
}




namespace Lang
{
	struct Value
	{
		int value;
		inline Value operator+(const Value& mOther) const { return Value{value + mOther.value}; }
	};	

	class Expr : public Engine::AST
	{
		public:
			inline virtual Value evaluate() { throw; }
	};

	class ValueExpr : public Expr
	{
		private:
			Value value;

		public:
			inline Value evaluate() override { return value; }			
	};

	class BinaryOpExpr : public Expr
	{	
		public:		
			inline Value evaluate() override { throw; }
			inline Expr& getLhs() { return getChildAs<Expr>(0); }
			inline Expr& getRhs() { return getChildAs<Expr>(1); }
	};

	class BinaryAdditionExpr : public BinaryOpExpr
	{	
		public:
			inline BinaryAdditionExpr
			inline Value evaluate() override { return getLhs().evaluate() + getRhs().evaluate(); }
	};
}







int main()
{
	using namespace Engine;
	using namespace Lang;

	SSVUT_RUN();
	ssvu::lo() << "Start" << std::endl;

	/*std::vector<Tkn> tkns
	{
		Tkn::Num,
		Tkn::OpAdd,
		Tkn::POpen,
		Tkn::POpen,
		Tkn::Num,
		Tkn::OpAdd,
		Tkn::Num,
		Tkn::PClose,
		Tkn::OpAdd,
		Tkn::Num,
		Tkn::PClose
	};*/

	std::vector<Tkn> tkns
	{
		Tkn::POpen,
		Tkn::Num,
		Tkn::OpAdd,
		Tkn::POpen,
		Tkn::Num,
		Tkn::PClose,
		Tkn::PClose,
		Tkn::OpAdd,
		Tkn::Num,
		Tkn::OpAdd,
		Tkn::POpen,
		Tkn::Num,
		Tkn::PClose
	};

	// Desired CST:
	/*
				Expr
		  T------|-------T
		Expr 	Op 		Expr
		  |		 |       |
		  |	   OpAdd     |
		 Num            Num
	*/	

	// Desired AST:
	/*
			(+)
		T----|
		|    |
       Num  Num
	*/

	Parser<LangSpec> parser;

	parser.createRule([]{ return std::make_unique<BinaryAdditionExpr>( },		Gmr::Expr, Gmr::Op, Gmr::Expr);
	parser.createRule(Gmr::Expr,		Tkn::POpen, Gmr::Expr, Tkn::PClose);
	parser.createRule(Gmr::Expr,		Tkn::Num);

	parser.createRule(Gmr::Op,			Tkn::OpAdd);
	parser.createRule(Gmr::Op,			Tkn::OpSub);

	parser.parse(tkns);

	return 0;
}

/*
	( 1 + 3 ) + ( 5 + 2 ) 

	( N + N ) + ( N + N ) 

	( N + N 
	( BinaryAddition[EXPR]

	( BinaryAddition[EXPR] )
	ParenthesizedExpr[EXPR]

	ParenthesizedExpr[EXPR] +
	ParenthesizedExpr[EXPR] + (
	ParenthesizedExpr[EXPR] + ( N 
	ParenthesizedExpr[EXPR] + ( N +
	ParenthesizedExpr[EXPR] + ( N + N 
*/