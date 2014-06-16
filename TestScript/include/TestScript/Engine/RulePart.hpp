// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_RULEPART
#define TESTSCRIPT_ENGINE_RULEPART

namespace Eng
{
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

			inline RPType getType() const noexcept							{ return type; }
			inline const decltype(tokenType)& getTokenType() const noexcept	{ SSVU_ASSERT(type == RPType::Token); return tokenType; }
			inline ASTTypeId getASTTypeId() const noexcept					{ SSVU_ASSERT(type == RPType::Node); return astTypeId; }

			inline bool matchesNode(ASTNodePtr<TL> mNode) const
			{
				if(mNode->getTypeId() == Eng::getASTTypeId<ASTTokenNodeImpl<TL>>())
				{
					if(type != RPType::Token) return false;

					auto& pTokenImpl(mNode->template getAs<ASTTokenNodeImpl<TL>>());
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
}

#endif
