// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_RULE
#define TESTSCRIPT_ENGINE_RULE

namespace Eng
{
	template<typename TL> class Rule
	{
		template<typename> friend class Parser;

		private:
			ssvu::Func<bool(Ctx<TL>&)> fnPredicate;
			ssvu::Func<void(Ctx<TL>&)> fnAction;
			ssvu::Func<void(Ctx<TL>&, LookAheadResults&)> fnLookAhead;

			inline bool matches(Ctx<TL>& mCtx) const { return fnPredicate(mCtx); }
			inline void action(Ctx<TL>& mCtx) { fnAction(mCtx); }
			inline LookAheadResults lookAhead(Ctx<TL>& mCtx)
			{
				LookAheadResults result;
				if(fnLookAhead != nullptr) fnLookAhead(mCtx, result);
				return result;
			}

		public:
			template<typename T> inline void setPredicate(const T& mFnPredicate) noexcept	{ fnPredicate = mFnPredicate; }
			template<typename T> inline void setAction(const T& mFnAction) noexcept			{ fnAction = mFnAction; }
			template<typename T> inline void setLookAhead(const T& mFnLookAhead) noexcept	{ fnLookAhead = mFnLookAhead; }
	};
}

#endif
