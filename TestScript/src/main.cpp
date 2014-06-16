#include <SSVUtils/SSVUtils.hpp>
#include "TestScript/Engine/Engine.hpp"
#include "TestScript/Language/Language.hpp"


template<bool TFmt, typename TN> inline void printNode(std::ostream& mStream, TN& mValue, std::size_t mDepth)
{
	for(auto i(0u); i < mDepth; ++i) ssvu::Internal::callStringifyImpl<TFmt>(mStream, "|\t");

	ssvu::Internal::callStringifyImpl<TFmt>(mStream, "L______o> ");

	if(dynamic_cast<Lang::ASTExpr*>(&mValue))
	{
		ssvu::Internal::callStringifyImpl<TFmt>(mStream, mValue.template getAs<Lang::ASTExpr>().getName());
	}
	else if(dynamic_cast<Eng::ASTTokenNodeImpl<Lang::Spec>*>(&mValue))
	{
		ssvu::Internal::callStringifyImpl<TFmt>(mStream, Lang::tknToStr(mValue.template getAs<Eng::ASTTokenNodeImpl<Lang::Spec>>().getToken().getType()));
	}


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

	ruleSet.createRule(RuleKey<Spec>{Tkn::Number}, [](NodeCtx<Spec>& mCtx) -> ASTNode<Spec>&
	{
		ssvu::lo("NUMB") << "" << std::endl;
		return mCtx.createReduction<ASTNumber>(15);
	});
	ruleSet.createRule(RuleKey<Spec>{Tkn::POpen, getASTTypeId<ASTExpr>(), Tkn::PClose}, [](NodeCtx<Spec>& mCtx) -> ASTNode<Spec>&
	{
		ssvu::lo("PEXPR") << "" << std::endl;
		return mCtx.createReduction<ASTParenthesizedExpr>(mCtx.get<ASTExpr>(1));
	});
	ruleSet.createRule(RuleKey<Spec>{getASTTypeId<ASTNumber>()}, [](NodeCtx<Spec>& mCtx) -> ASTNode<Spec>&
	{
		ssvu::lo("EXPR") << "" << std::endl;
		return mCtx.createReduction<ASTExpr>();
	});
	ruleSet.createRule(RuleKey<Spec>{getASTTypeId<ASTBinaryOp<OpAddition<int>>>()}, [](NodeCtx<Spec>& mCtx) -> ASTNode<Spec>&
	{
		ssvu::lo("EXPR") << "" << std::endl;
		return mCtx.createReduction<ASTExpr>();
	});
	ruleSet.createRule(RuleKey<Spec>{getASTTypeId<ASTExpr>(), Tkn::OpPlus, getASTTypeId<ASTExpr>()}, [](NodeCtx<Spec>& mCtx) -> ASTNode<Spec>&
	{
		ssvu::lo("PLUSEXPR") << "" << std::endl;
		return mCtx.createReduction<ASTBinaryOp<OpAddition<int>>>(mCtx.get<ASTExpr>(0), mCtx.get<ASTExpr>(2));
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
	ssvu::lo("RESULT") <<	b->getAs<ASTExpr>().eval() << std::endl;
	//ssvu::lo() << b->getAsNode().getNode().getChildren().size() << std::endl;
	//ssvu::lo() << int(b->getAsNode().getNode().getType());
	b->getAs<ASTExpr>().toBytecode();

	printNode<true>(std::cout, *b, 0);

	ssvu::lo() << std::endl;

	return 0;
}

/*
OpMinus,
		OpMult,
		OpDiv,
		OpMod*/
