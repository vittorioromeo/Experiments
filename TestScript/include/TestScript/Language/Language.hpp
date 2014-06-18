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
		OpAdd,
		OpSub,
		OpMul,
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
			"OpAdd",
			"OpSub",
			"OpMul",
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

		inline void toBytecode() override { ssvu::lo() << "push(" << value << ");" << std::endl; }
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

		inline void toBytecode() override { lhs->toBytecode(); rhs->toBytecode(); ssvu::lo() << "popOp(" << TOp::getStr() << ");" << std::endl; }
	};
}

#endif
