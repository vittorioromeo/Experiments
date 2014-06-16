// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_LANGUAGE
#define TESTSCRIPT_LANGUAGE

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

	inline std::string tknToStr(Tkn mTkn)
	{
		auto idx(static_cast<int>(mTkn));

		static std::string array[]
		{
			"Number",
			"POpen",
			"PClose",
			"OpPlus",
			"OpMinus",
			"OpMult",
			"OpDiv",
			"OpMod"
		};

		return array[idx];
	}

	struct TknData { };

	using Spec = Eng::LangSpec<Tkn, TknData>;

	struct ASTExpr : public Eng::ASTNode<Spec>
	{
		inline virtual std::string getName() { return "ASTExpr"; }
		inline virtual int eval()
		{
			auto& child(getChildren()[0]->getAs<ASTExpr>());
			return child.eval();
		}
		inline virtual void toBytecode()
		{
			auto& child(getChildren()[0]->getAs<ASTExpr>());
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

	template<typename T> struct OpAddition			{ inline static T get(const T& mA, const T& mB) noexcept { return mA + mB; } };
	template<typename T> struct OpSubtraction		{ inline static T get(const T& mA, const T& mB) noexcept { return mA - mB; } };
	template<typename T> struct OpMultiplication	{ inline static T get(const T& mA, const T& mB) noexcept { return mA * mB; } };
	template<typename T> struct OpDivision			{ inline static T get(const T& mA, const T& mB) noexcept { return mA / mB; } };
	template<typename T> struct OpRemainder			{ inline static T get(const T& mA, const T& mB) noexcept { return mA % mB; } };

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

#endif
