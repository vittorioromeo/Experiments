// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_LANGUAGE
#define TESTSCRIPT_LANGUAGE

namespace Lang
{
	enum class Tkn : int
	{
		Number = 0,
		POpen = 1,
		PClose = 2,
		OpAdd = 3,
		OpSub = 4,
		OpMul = 5,
		OpDiv = 6,
		OpMod = 7
	};

	inline std::string tknToStr(Tkn mTkn)
	{
		static auto array(ssvu::mkArray
		(
			"Number",
			"POpen",
			"PClose",
			"OpAdd",
			"OpSub",
			"OpMul",
			"OpDiv",
			"OpMod"
		));

		return array[ssvu::castEnum(mTkn)];
	}

	struct TknData { };

	using Spec = Eng::LangSpec<Tkn, TknData>;

	struct ASTExpr : public Eng::Node<Spec>
	{
		inline virtual std::string getName() { return "ASTExpr"; }
		virtual int eval() = 0;
		virtual void toBytecode() = 0;
	};


	struct ASTNumber : public ASTExpr
	{
		int value;

		inline ASTNumber(int mValue) : value{mValue} { }

		inline std::string getName() override { return ssvu::toStr(value); }
		inline int eval() override
		{
			return value;
		}

		inline void toBytecode() override { ssvu::lo() << "push(" << value << ");\n"; }
	};

	struct ASTParenthesizedExpr : public ASTExpr
	{
		ASTExpr* innerExpr{nullptr};

		inline ASTParenthesizedExpr(ASTExpr& mInnerExpr) : innerExpr{&mInnerExpr}
		{
			emplaceChild(innerExpr);
		}

		inline std::string getName() override { return "( ... )"; }
		inline int eval() override { return innerExpr->eval(); }
		inline void toBytecode() override { innerExpr->toBytecode(); }
	};

	#define CREATE_OP_STRUCT(mName, mOp) \
		template<typename T> struct mName \
		{ \
			inline static T get(const T& mA, const T& mB) noexcept { return mA mOp mB; } \
			inline static std::string getStr() noexcept { return SSVPP_TOSTR(mOp); } \
		}

	CREATE_OP_STRUCT(OpAdd, +);
	CREATE_OP_STRUCT(OpSub, -);
	CREATE_OP_STRUCT(OpMul, *);
	CREATE_OP_STRUCT(OpDiv, /);
	CREATE_OP_STRUCT(OpMod, %);

	template<typename TOp> struct ASTBinaryOp : public ASTExpr
	{
		ASTExpr* lhs{nullptr};
		ASTExpr* rhs{nullptr};

		inline ASTBinaryOp(ASTExpr& mLhs, ASTExpr& mRhs) : lhs{&mLhs}, rhs{&mRhs}
		{
			emplaceChild(lhs);
			emplaceChild(rhs);
		}

		inline std::string getName() override { return TOp::getStr(); }
		inline int eval() override { return TOp::get(lhs->eval(), rhs->eval()); }

		inline void toBytecode() override { lhs->toBytecode(); rhs->toBytecode(); ssvu::lo() << "popOp(" << TOp::getStr() << ");\n"; }
	};
}

#endif
