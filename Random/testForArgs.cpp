#include <SSVUtils/Core/Core.hpp>

template<typename TF> 
inline auto forArgs2(TF&& mFn)
{
	return FWD(mFn);
}

template<typename TF, typename T1, typename T2, typename... TArgs> 
inline auto forArgs2(TF&& mFn, T1&& mA1, T2&& mA2, TArgs&&... mArgs)
{
	FWD(mFn)(mA1, mA2);
	return forArgs2(FWD(mFn), FWD(mArgs)...);
}

template<bool TDone, ssvu::SizeT TC, ssvu::SizeT TItr, typename TF, typename... TArgs>
struct ForNArgsHlpr;

template<ssvu::SizeT TC, ssvu::SizeT TItr, typename TF, typename... TArgs>
struct ForNArgsHlpr<false, TC, TItr, TF, TArgs...>
{
	template<ssvu::SizeT... TIdxs> inline static auto impl(TF&& mFn, ssvu::Tpl<TArgs...>& mTpl, ssvu::IdxSeq<TIdxs...>)
	{
		FWD(mFn)(std::get<TItr + TIdxs>(mTpl)...);
		return ForNArgsHlpr<(TItr + TC) == sizeof...(TArgs), TC, TItr + TC, TF, TArgs...>::template impl(FWD(mFn), mTpl, ssvu::MkIdxSeq<TC>{}); 
	}
};

template<ssvu::SizeT TC, ssvu::SizeT TItr, typename TF, typename... TArgs>
struct ForNArgsHlpr<true, TC, TItr, TF, TArgs...>
{
	template<ssvu::SizeT... TIdxs> inline static auto impl(TF&& mFn, ssvu::Tpl<TArgs...>&, ssvu::IdxSeq<TIdxs...>)
	{
		return FWD(mFn);
	}
};

/*
template<ssvu::SizeT TC, ssvu::SizeT TItr, typename TF, typename... TArgs, ssvu::SizeT... TIdxs>
inline auto forNArgsImpl(TF&& mFn, ssvu::Tpl<TArgs...>&)
{
	return FWD(mFn);
}

template<ssvu::SizeT TC, ssvu::SizeT TStart, typename TF, typename... TArgs, ssvu::SizeT... TIdxs>
inline auto forNArgsImpl(TF&& mFn, ssvu::Tpl<TArgs...>& mTpl)
{
	FWD(mFn)(std::get<TStart + TIdxs>(mTpl)...);
	return forNArgsImpl<TC, TStart + TC, TArgs..., TIdxs...>(FWD(mFn), mTpl); 
}*/

template<ssvu::SizeT TC, typename TF, typename... TArgs>
inline auto forNArgs(TF&& mFn, TArgs&&... mArgs)
{
	SSVU_ASSERT_STATIC((sizeof...(mArgs) % TC) == 0, "Number of arguments is not divisible by TC");
	// TODO: check function arity
	auto tpl(ssvu::mkTpl(FWD(mArgs)...));
	return ForNArgsHlpr<false, TC, 0, TF, TArgs...>::template impl(FWD(mFn), tpl, ssvu::MkIdxSeq<TC>{});
}

int main()
{
	int x = 0;
	forNArgs<1>([&x](auto&& mA1, auto&& mA2)
	{
		ssvu::lo("a1") << mA1 << "\n";
		ssvu::lo("a2") << mA2 << "\n";

		x += mA1 * mA2;
	}, 1, 1, 4, 6);

	ssvu::lo() << x << std::endl;	


	return 0;
}

// TODO: check generated assembly (does the tuple get optimized out?)
// TODO: tests