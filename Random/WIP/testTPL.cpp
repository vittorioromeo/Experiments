#include <SSVUtils/Core/Core.hpp>

template<int TN> struct Abs
{
	SSVU_ASSERT_STATIC(TN != std::numeric_limits<int>::min(), "");
	static constexpr int value{(TN < 0) ? -TN : TN};
};

template<int TM, int TN> struct GCD
{
	static constexpr int value{GCD<TN, TM % TN>::value};
};
template<int TM> struct GCD<TM, 0>
{
	static constexpr int value{TM};
};

template<typename T> struct Rank
{
	static constexpr std::size_t value{0u};
};
template<typename T, std::size_t TN> struct Rank<T[TN]>
{
	static constexpr std::size_t value{1u + Rank<T>::value};
};

template<bool TP, typename TT, typename TF> struct If { };
template<typename TT, typename TF> struct If<true, TT, TF> { using Type = TT; };
template<typename TT, typename TF> struct If<false, TT, TF> { using Type = TF; };
template<bool TP, typename TT, typename TF> using IfT = typename If<TP, TT, TF>::Type;

template<bool TP, typename T = void> struct EnableIfImpl { using Type = T; };
template<typename T> struct EnableIfImpl<false, T> { }; // SFINAE!

template<bool TP, typename T> using EnableIfT = typename EnableIfImpl<TP, T>::Type;

template<typename T> EnableIfT<std::is_integral<T>::value, int> fnEifTest(T mA)
{
	return 1;
}
template<typename T> EnableIfT<std::is_floating_point<T>::value, int> fnEifTest(T mA)
{
	return 2;
}

int main() 
{
	if(false)
	{
		ssvu::lo() << int(Abs<-15>::value) << std::endl;
		ssvu::lo() << int(GCD<15, 5>::value) << std::endl;

		ssvu::lo() << int(Rank<int>::value) << std::endl;
		ssvu::lo() << int(Rank<int[2]>::value) << std::endl;
		ssvu::lo() << int(Rank<int[2][5]>::value) << std::endl;

		auto f1([]{ return 1; });
		auto f2([]{ return 2; });

		ssvu::lo() << int(IfT<false, decltype(f1), decltype(f2)>{}()) << std::endl;

		ssvu::lo() << int(fnEifTest(1)) << std::endl;
		ssvu::lo() << int(fnEifTest(1.5f)) << std::endl;
	}

	return 0;
}