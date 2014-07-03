#include <SSVUtils/SSVUtils.hpp>
#include "TestScript/Engine/Engine.hpp"
#include "TestScript/Language/Language.hpp"


template<bool TFmt, typename TN> inline void printNode(std::ostream& mStream, TN& mValue, std::size_t mDepth)
{
	using namespace ssvu;
	using namespace ssvu::Internal;

	for(auto i(0u); i < mDepth; ++i) callStringifyImpl<TFmt>(mStream, "|\t");

	callStringifyImpl<TFmt>(mStream, "L______o> ");

	if(dynamic_cast<Lang::ASTExpr*>(&mValue))
	{
		callStringifyImpl<TFmt>(mStream, mValue.template getAs<Lang::ASTExpr>().getName());
	}
	else if(dynamic_cast<Eng::ASTTokenNode<Lang::Spec>*>(&mValue))
	{
		callStringifyImpl<TFmt>(mStream, Lang::tknToStr(mValue.template getAs<Eng::ASTTokenNode<Lang::Spec>>().getToken().getType()));
	}

	callStringifyImpl<TFmt>(mStream, "\n");

	for(auto i(0u); i < mValue.getChildren().size(); ++i)
	{
		printNode<TFmt>(mStream, *(mValue.getChildren().at(i)), mDepth + 1);
		if(i == mValue.getChildren().size() - 1)
		{
			for(auto i(0u); i < mDepth + 1; ++i) callStringifyImpl<TFmt>(mStream, "|\t");
			callStringifyImpl<TFmt>(mStream, "\n");
		}
	}
}



int main()
{
	using namespace Eng;
	using namespace Lang;


	Parser<Spec> parser;

	auto& numberToExpr(parser.createRule());
	numberToExpr.setPredicate([](Ctx<Spec>& mCtx)
	{
		if(mCtx.getSize() < 1) return false;

		return mCtx[0].isToken(Tkn::Number);
	});
	numberToExpr.setAction([](Ctx<Spec>& mCtx)
	{
		mCtx.pop();
		mCtx.pushCreate<ASTNumber>(2);
	});

	auto& parenthesizedExpr(parser.createRule());
	parenthesizedExpr.setPredicate([](Ctx<Spec>& mCtx)
	{
		if(mCtx.getSize() < 3) return false;
		return mCtx[0].isToken(Tkn::POpen) && mCtx[1].isDerivedFrom<ASTExpr>() && mCtx[2].isToken(Tkn::PClose);
	});
	parenthesizedExpr.setAction([](Ctx<Spec>& mCtx)
	{
		mCtx.pop();
		auto& expr1(mCtx.popAs<ASTExpr>());
		mCtx.pop();
		mCtx.pushCreate<ASTParenthesizedExpr>(expr1);
	});

	#define RULE_CREATE_BIN_OP(mName, mTknName, mOpName, mLookAhead) \
		auto& mName(parser.createRule()); \
		mName.setPredicate([](Ctx<Spec>& mCtx) \
		{ \
			if(mCtx.getSize() < 3) return false; \
			return mCtx[0].isDerivedFrom<ASTExpr>() && mCtx[1].isToken(Tkn::mTknName) && mCtx[2].isDerivedFrom<ASTExpr>(); \
		}); \
		mName.setAction([](Ctx<Spec>& mCtx) \
		{ \
			auto& expr1(mCtx.popAs<ASTExpr>()); \
			mCtx.pop(); \
			auto& expr2(mCtx.popAs<ASTExpr>()); \
			mCtx.pushCreate<ASTBinaryOp<mOpName<int>>>(expr1, expr2); \
		}); \
		if(mLookAhead) mName.setLookAhead([](Ctx<Spec>& mCtx, LookAheadResults& mResults) \
		{ \
			if(!mCtx.canLookAhead(3)) return; \
			if(mCtx.getAhead(3).isToken(Tkn::OpMul) || mCtx.getAhead(3).isToken(Tkn::OpDiv) || mCtx.getAhead(3).isToken(Tkn::OpMod)) \
			{ \
				mResults.forceShift = true; \
			} \
		})

	RULE_CREATE_BIN_OP(binMul, OpMul, OpMul, false);
	RULE_CREATE_BIN_OP(binDiv, OpDiv, OpDiv, false);
	RULE_CREATE_BIN_OP(binMod, OpMod, OpMod, false);
	RULE_CREATE_BIN_OP(binAdd, OpAdd, OpAdd, true);
	RULE_CREATE_BIN_OP(binSub, OpSub, OpSub, true);

	std::vector<Token<Spec>> src
	{
		{Tkn::Number},
		{Tkn::OpAdd},
		{Tkn::Number},
		{Tkn::OpMul},
		{Tkn::Number},
		{Tkn::OpAdd},
		{Tkn::Number},
		{Tkn::OpAdd},
		{Tkn::Number},
		{Tkn::OpDiv},
		{Tkn::Number},
		{Tkn::OpAdd},
		{Tkn::Number}
	};

	std::vector<Token<Spec>> src2
	{
		{Tkn::POpen},
		{Tkn::POpen},
		{Tkn::Number},
		{Tkn::OpAdd},
		{Tkn::Number},
		{Tkn::PClose},
		{Tkn::OpMul},
		{Tkn::POpen},
		{Tkn::Number},
		{Tkn::OpAdd},
		{Tkn::Number},
		{Tkn::PClose},
		{Tkn::PClose},
		{Tkn::OpMul},
		{Tkn::Number}
	};

	std::vector<Token<Spec>> src3
	{
		{Tkn::POpen},
		{Tkn::Number},
		{Tkn::OpAdd},
		{Tkn::Number},
		{Tkn::PClose},
		{Tkn::OpMul},
		{Tkn::POpen},
		{Tkn::Number},
		{Tkn::OpAdd},
		{Tkn::Number},
		{Tkn::PClose}
	};

	parser.run(src3);

	auto& b(parser.getParseStack().back());
	ssvu::lo("RESULT") <<	b->getAs<ASTExpr>().eval() << std::endl;
	b->getAs<ASTExpr>().toBytecode();

	printNode<true>(std::cout, *b, 0);

	ssvu::lo() << std::endl;

	return 0;
}

