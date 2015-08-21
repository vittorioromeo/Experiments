#include <SSVUtils/Core/Core.hpp>

namespace Exp
{
	using namespace ssvu;

	template<typename T> struct TypeWrapper
	{
		using Type = T;
	};	

	template<typename TState, typename TF>
	inline auto forTypes2(TF&&)
	{
		return TState{};
	}
	
	template<typename TState, typename T, typename... Ts, typename TF>
	inline auto forTypes2(TF&& mFn)
	{
		auto result(mFn(TypeWrapper<T>{}, TState{}));
		return forTypes2<decltype(result), Ts...>(mFn);
	}


	template<typename... Ts> struct State
	{
		template<SizeT TI> inline constexpr auto get() const noexcept
		{
			return MPL::List<Ts...>::template At<TI>::value;
		}
	};
}

template<ssvu::SizeT TS> struct Acc 
{
	inline constexpr auto get() const noexcept { return TS; }
};

int main()
{
	using namespace ssvu;
	using namespace Exp;

	auto result = forTypes2
	<
		Acc<0>,
		int, char, float, double
	>
	(
		[](auto t, auto s)
		{
			using Type = typename decltype(t)::Type;			
			return Acc<MPL::getMax(s.get(), sizeof(Type))>{};
		}
	);

	using TestRes = decltype(result);

	ssvu::lo() << result.get() << "\n";
	ssvu::lo() << TestRes{}.get() << "\n";

	return 0;
}