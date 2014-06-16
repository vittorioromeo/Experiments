#include <SSVUtils/SSVUtils.hpp>

namespace ssvvs
{
	SSVU_FATENUM_MGR(EnumMgr);

	SSVU_FATENUM_DEFS(EnumMgr, TokenMetaType, int, 
		Value,
		Keyword,
		Operator,
		Serparator,
		Special
	);

	SSVU_FATENUM_DEFS(EnumMgr, TokenType, int, 
		// Value tokens
		Int,
		Float,
		String,
		Symbol,

		// Keyword tokens
		If,
		While,
		True,
		False,
		Null,
		Var,

		// Operator tokens
		Plus,
		Minus,
		Star,
		Slash,
		Percent,
		PrRoundL,
		PrRoundR,
		Equal,

		// Separator tokens
		Colon,
		Semicolon
	);

	SSVU_FATENUM_DEFS(EnumMgr, ValueType, int, 
		Null,
		Int,
		Float
	);

	SSVU_FATENUM_DEFS(EnumMgr, OperatorType, int, 
		Plus,
		Minus,
		Star,
		Slash,
		Percent
	);


	struct TokenData
	{

	};

	struct Token
	{
		TokenMetaType metaType;
		TokenType type;
		TokenData data;

		inline Token(TokenMetaType mMetaType, TokenType mType, const TokenData& mData) : metaType{mMetaType}, type{mType}, data(mData) { }
	};

	struct Context;

	class ASTNode
	{	
		friend Context;

		private:
			Context* context;
			ASTNode* parent;
			std::vector<ASTNode*> children;

		public:			
			inline virtual ~ASTNode() { } 
			template<typename T> inline T& getAs() { return *reinterpret_cast<T*>(this); }
	};

	class Context
	{
		private:			
			ssvu::PolyManager<ASTNode> astNodes;


		public:
			inline ASTNode& createASTRoot()
			{
				return create<ASTNode>();
			}

			template<typename T, typename... TArgs> inline ASTNode& create(TArgs&&... mArgs)
			{
				auto& result(astNodes.create<T>(std::forward<TArgs>(mArgs)...));
				result.context = this;
				return result;
			}
	};

	struct Value
	{
		ValueType type;
		union 
		{
			int valueInt;
			float valueFloat;
		};

		inline Value(ValueType mType) : type(mType) { }
		template<typename T> inline Value(ValueType mType, T mValue) : type(mType) 
		{ 
			switch(type)
			{
				case ValueType::Null: 							break;
				case ValueType::Int: 	valueInt = mValue; 		break;
				case ValueType::Float: 	valueFloat = mValue; 	break;
			}
		}
		
		inline int getAsInt() const noexcept
		{
			if(type == ValueType::Int) return valueInt;
			if(type == ValueType::Float) return static_cast<float>(valueInt);
			
			std::terminate();
		}

		inline float getAsFloat() const noexcept
		{
			if(type == ValueType::Int) return static_cast<float>(valueInt);
			if(type == ValueType::Float) return valueFloat;

			std::terminate();
		}

		inline Value operator+(const Value& mRhs) const noexcept 
		{
			if(type == ValueType::Null || mRhs.type == ValueType::Null) return {ValueType::Null}; 			
			if(type == ValueType::Float || mRhs.type == ValueType::Float) return {ValueType::Float, getAsFloat() + mRhs.getAsFloat()};
			return {ValueType::Int, getAsInt() + mRhs.getAsInt()};			
		}
		inline Value operator-(const Value& mRhs) const noexcept 
		{
			if(type == ValueType::Null || mRhs.type == ValueType::Null) return {ValueType::Null}; 			
			if(type == ValueType::Float || mRhs.type == ValueType::Float) return {ValueType::Float, getAsFloat() - mRhs.getAsFloat()};
			return {ValueType::Int, getAsInt() - mRhs.getAsInt()};			
		}
		inline Value operator*(const Value& mRhs) const noexcept 
		{
			if(type == ValueType::Null || mRhs.type == ValueType::Null) return {ValueType::Null}; 			
			if(type == ValueType::Float || mRhs.type == ValueType::Float) return {ValueType::Float, getAsFloat() * mRhs.getAsFloat()};
			return {ValueType::Int, getAsInt() * mRhs.getAsInt()};			
		}
		inline Value operator/(const Value& mRhs) const noexcept 
		{
			if(type == ValueType::Null || mRhs.type == ValueType::Null) return {ValueType::Null}; 			
			if(type == ValueType::Float || mRhs.type == ValueType::Float) return {ValueType::Float, getAsFloat() / mRhs.getAsFloat()};
			return {ValueType::Int, getAsInt() / mRhs.getAsInt()};			
		}
		inline Value operator%(const Value& mRhs) const noexcept 
		{
			if(type == ValueType::Null || mRhs.type == ValueType::Null) return {ValueType::Null}; 			
			if(type == ValueType::Float || mRhs.type == ValueType::Float) return {ValueType::Float, std::fmod(getAsFloat(), mRhs.getAsFloat())};
			return {ValueType::Int, getAsInt() % mRhs.getAsInt()};			
		}
	};

	struct ASTExpr : ASTNode
	{
		inline virtual Value evaluate() { std::terminate(); }
	};

	struct ASTEValue : ASTExpr
	{
		Value value;

		inline ASTEValue(const Value& mValue) : value(mValue) { }
		inline Value evaluate() override { return value; }
	};

	struct ASTEOp : ASTExpr
	{
		OperatorType op;
		ASTExpr* lhs;
		ASTExpr* rhs; 

		inline ASTEOp(OperatorType mOperatorType, ASTExpr* mLhs, ASTExpr* mRhs) : op{mOperatorType}, lhs{mLhs}, rhs{mRhs} 
		{ 

		}
		inline Value evaluate() override 
		{ 
			switch(op)
			{
				case OperatorType::Plus: 	return lhs->evaluate() + rhs->evaluate(); 
				case OperatorType::Minus: 	return lhs->evaluate() - rhs->evaluate(); 
				case OperatorType::Star: 	return lhs->evaluate() * rhs->evaluate(); 
				case OperatorType::Slash: 	return lhs->evaluate() / rhs->evaluate(); 
				case OperatorType::Percent: return lhs->evaluate() % rhs->evaluate(); 
			}

			std::terminate();
		}
	};
}

int main()
{
	using namespace ssvvs;

	/*std::vector<Token> script;
	script.emplace_back(TokenMetaType::Value, TokenType::Int, TokenData{});
	script.emplace_back(TokenMetaType::Operator, TokenType::Plus, TokenData{});
	script.emplace_back(TokenMetaType::Value, TokenType::Int, TokenData{});*/

	Context context;
	ASTNode& root(context.createASTRoot());

	auto& nv1(context.create<ASTEValue>(Value{ValueType::Float, 10.f}).getAs<ASTExpr>());
	auto& nv2(context.create<ASTEValue>(Value{ValueType::Float, 3.f}).getAs<ASTExpr>());
	auto& nop(context.create<ASTEOp>(OperatorType::Star, &nv1, &nv2));

	ssvu::lo() << nop.getAs<ASTExpr>().evaluate().getAsFloat() << std::endl;
}