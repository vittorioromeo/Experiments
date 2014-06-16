// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_RULE
#define TESTSCRIPT_ENGINE_RULE

namespace Eng
{
	template<typename TL> class Rule
	{
		public:
			using RuleFunc = ssvu::Func<ASTNode<TL>&(NodeCtx<TL>&)>;

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
}

#endif
