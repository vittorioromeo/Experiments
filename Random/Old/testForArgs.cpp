#include <SSVUtils/Core/Core.hpp>

// http://stackoverflow.com/a/29901074/598696

namespace ssvu
{
	namespace Impl
	{
		template<typename, typename> struct ForNArgsHlpr;

		template<SizeT... Bs, SizeT... Cs> struct ForNArgsHlpr<IdxSeq<Bs...>, IdxSeq<Cs...>>
		{
			using Swallow = bool[];

			#define IMPL_IMPL_FORNARGS_EXECN_BODY() \
				FWD(mFn)(std::get<N + Cs>(FWD(mXs))...)

		    template<SizeT N, typename TF, typename TTpl, typename... Ts>
		    inline static constexpr void execN(TF&& mFn, TTpl&& mXs)
		    	noexcept(noexcept(IMPL_IMPL_FORNARGS_EXECN_BODY()))
		    {
		        IMPL_IMPL_FORNARGS_EXECN_BODY();
		    }

		    #undef IMPL_IMPL_FORNARGS_EXECN_BODY

		    #define IMPL_IMPL_FORNARGS_EXEC_BODY() \
		    	(void) Swallow{(execN<(Bs * sizeof...(Cs))>(FWD(mFn), FWD(mXs)), true)..., true}

		    template<typename TF, typename TTpl, typename... Ts>
		    inline static constexpr void exec(TF&& mFn, TTpl&& mXs)
		    	noexcept(noexcept(IMPL_IMPL_FORNARGS_EXEC_BODY()))
		    {		       
		        IMPL_IMPL_FORNARGS_EXEC_BODY();
		    }

		    #undef IMPL_IMPL_FORNARGS_EXEC_BODY
		};
	}

	#define IMPL_FORNARGS_BODY() \
		Impl::ForNArgsHlpr<MkIdxSeq<sizeof...(Ts) / N>, MkIdxSeq<N>>::exec(FWD(mFn), fwdAsTpl(FWD(mXs)...))

	template<std::size_t N, typename TF, typename... Ts>
	inline constexpr void forNArgs(TF&& mFn, Ts&&... mXs)
		noexcept(noexcept(IMPL_FORNARGS_BODY()))
	{
	    SSVU_ASSERT_STATIC(sizeof...(Ts) % N == 0, "Unallowed arity: not divisible by number of arguments");	    
	    IMPL_FORNARGS_BODY();
	}

	#undef IMPL_FORNARGS_BODY
}

int main()
{
	using namespace ssvu;

	int x = 0;
	forNArgs<1>([&x](auto&& mA1)
	{
		lo("a1") << mA1 << "\n";
		

		x += mA1;
	}, 1, 1, 4, 6);

	forNArgs<2>([&x](auto&& mA1, auto&& mA2)
	{
		lo("a1") << mA1 << "\n";
		lo("a2") << mA2 << "\n";

		x += mA1 * mA2;
	}, 1, 1, 4, 6);

	forNArgs<4>([&x](auto&& mA1, auto&& mA2, auto&& mA3, auto&& mA4)
	{		
		x += mA1 * mA2 + mA3 + mA4;
	}, 1, 1, 4, 6);

	forNArgs<4>([&x](auto&& mA1, auto&& mA2, auto&& mA3, auto&& mA4)
	{		
		x += mA1 * mA2 + mA3 + mA4;
	});

	lo() << x << std::endl;	


	return 0;
}

// TODO: check generated assembly (does the tuple get optimized out?)
// TODO: tests