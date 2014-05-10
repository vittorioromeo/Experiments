#ifdef WAT

#include <string>
#include <future>
#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Test/Test.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

int main()
{
	ssvu::Benchmark::start("Outer");
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		ssvu::Benchmark::start("Inner 1");
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		ssvu::Benchmark::endLo();

		ssvu::Benchmark::start("Inner 2");
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		ssvu::Benchmark::endLo();
	}
	ssvu::Benchmark::endLo();

	return 0;
}

#endif

#ifdef TESTINGMPL

#include <tuple>
#include <SSVUtils/SSVUtils.hpp>

namespace ssvu
{
	namespace MPL
	{
		template<typename...> struct List;

		namespace Internal
		{
			struct NullType { };
		}

		using Null = typename Internal::NullType;
		template<bool TCond> using BoolResult = Conditional<TCond, std::true_type, std::false_type>;

		template<int TN1, int TN2> inline constexpr int getMin() noexcept		{ return TN1 < TN2 ? TN1 : TN2; }
		template<int TN1, int TN2> inline constexpr int getMax() noexcept		{ return TN1 > TN2 ? TN1 : TN2; }

		namespace Internal
		{
			// VA Args Head
			template<typename... TArgs> struct VAHeadHelper;
			template<typename T, typename... TArgs> struct VAHeadHelper<T, TArgs...>	{ using Type = T; };
			template<> struct VAHeadHelper<>											{ using Type = Null; };
			template<typename... TArgs> using VAHead = typename Internal::VAHeadHelper<TArgs...>::Type;

			// VA Args Tail
			template<typename... TArgs> struct VATailHelper;
			template<typename T, typename... TArgs> struct VATailHelper<T, TArgs...>	{ using Type = typename VATailHelper<TArgs...>::Type; };
			template<typename T> struct VATailHelper<T>									{ using Type = T; };
			template<> struct VATailHelper<>											{ using Type = Null; };
			template<typename... TArgs> using VATail = typename Internal::VATailHelper<TArgs...>::Type;

			// List::PopFront
			template<typename> struct PopFrontHelper;
			template<typename TL, typename... TLAs> struct PopFrontHelper<List<TL, TLAs...>>	{ using Type = List<TLAs...>; };
			template<> struct PopFrontHelper<List<>>											{ using Type = List<>; };

			// List::PopBack
			template<typename, typename> struct RemoveLastHelper;
			template<typename TLA1, typename TLA2, typename... TLAs1, typename... TLAs2> struct RemoveLastHelper<List<TLA1, TLA2, TLAs1...>, List<TLAs2...>>
			{
				using List1 = List<TLA1, TLA2, TLAs1...>;
				using List2 = List<TLAs2...>;
				using List1WithPop = typename PopFrontHelper<List1>::Type;
				using List2WithPush = typename List2::template PushBack<typename List1::Head>;

				using Type = typename RemoveLastHelper<List1WithPop, List2WithPush>::Type;
			};
			template<typename TL1, typename... TLAs2> struct RemoveLastHelper<List<TL1>, List<TLAs2...>> { using Type = List<TLAs2...>; };
			template<typename> struct PopBackHelper;
			template<typename... TLAs> struct PopBackHelper<List<TLAs...>> { using Type = typename RemoveLastHelper<List<TLAs...>, List<>>::Type; };
			template<> struct PopBackHelper<List<>> { using Type = List<>; };

			// List::SubList
			template<std::size_t, std::size_t, std::size_t, typename, typename, typename> struct SubListImpl;
			template<std::size_t, std::size_t, std::size_t, typename, typename> struct SubListDispatch;
			template<std::size_t TS1, std::size_t TS2, std::size_t TSC, typename... TLA1s, typename... TLA2s> struct SubListImpl<TS1, TS2, TSC, List<TLA1s...>, List<TLA2s...>, std::false_type>
			{
				using Type = typename SubListDispatch<TS1, TS2, TSC + 1, List<TLA1s...>, typename List<TLA2s...>::template PushBack<typename List<TLA1s...>::template Elem<TSC>>>::Type;
			};
			template<std::size_t TS1, std::size_t TS2, std::size_t TSC, typename... TLA1s, typename... TLA2s> struct SubListImpl<TS1, TS2, TSC, List<TLA1s...>, List<TLA2s...>, std::true_type>
			{
				using Type = List<TLA2s...>;
			};
			template<std::size_t TS1, std::size_t TS2, std::size_t TSC, typename TL1, typename TL2> struct SubListDispatch
			{
				using Type = typename SubListImpl<TS1, TS2, TSC, TL1, TL2, BoolResult<TSC == TS2 || TSC >= TL1::getSize()>>::Type;
			};
			template<std::size_t, std::size_t, std::size_t, typename> struct SubListHelper;
			template<std::size_t TS1, std::size_t TS2, std::size_t TSC, typename... TLAs> struct SubListHelper<TS1, TS2, TSC, List<TLAs...>>
			{
				using Type = typename SubListDispatch<TS1, TS2, TSC, List<TLAs...>, List<>>::Type;
			};
		}

		template<typename... TArgs> struct List
		{
			using Head = Internal::VAHead<TArgs...>;
			using Tail = Internal::VATail<TArgs...>;

			inline static constexpr std::size_t getSize() noexcept { return sizeof...(TArgs); }

			template<std::size_t TS> using Elem = std::tuple_element_t<TS, std::tuple<TArgs...>>;
			template<std::size_t TS> using ElemReverse = Elem<getSize() - 1 - TS>;

			template<typename T> using PushBack = List<TArgs..., T>;
			template<typename T> using PushFront = List<T, TArgs...>;

			using PopBack = typename Internal::PopBackHelper<List<TArgs...>>::Type;
			using PopFront = typename Internal::PopFrontHelper<List<TArgs...>>::Type;

			template<std::size_t TS1, std::size_t TS2> using SubList = typename Internal::SubListHelper<TS1, TS2, TS1, List<TArgs...>>::Type;

			using Clear = List<>;

			// TODO: insert
		};

		namespace Internal
		{
			namespace Tests
			{
				SSVU_ASSERT_STATIC(getMin<15, 10>() == 10, "");
				SSVU_ASSERT_STATIC(getMin<-15, 10>() == -15, "");

				SSVU_ASSERT_STATIC(getMax<15, 10>() == 15, "");
				SSVU_ASSERT_STATIC(getMax<-15, 10>() == 10, "");

				SSVU_ASSERT_STATIC(isSame<Internal::VAHead<int, float, char>, int>(), "");
				SSVU_ASSERT_STATIC(isSame<Internal::VATail<int, float, char>, char>(), "");

				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::Head, int>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::Tail, char>(), "");
				SSVU_ASSERT_STATIC(isSame<List<>::Head, Null>(), "");
				SSVU_ASSERT_STATIC(isSame<List<>::Tail, Null>(), "");

				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::Elem<0>, int>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::Elem<1>, float>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::Elem<2>, char>(), "");

				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::ElemReverse<0>, char>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::ElemReverse<1>, float>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::ElemReverse<2>, int>(), "");

				SSVU_ASSERT_STATIC(isSame<List<>::PushBack<int>, List<int>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int>::PushBack<float>, List<int, float>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float>::PushBack<char>, List<int, float, char>>(), "");

				SSVU_ASSERT_STATIC(isSame<List<>::PushFront<int>, List<int>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int>::PushFront<float>, List<float, int>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<float, int>::PushFront<char>, List<char, float, int>>(), "");

				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::PopBack, List<int, float>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float>::PopBack, List<int>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int>::PopBack, List<>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<>::PopBack, List<>>(), "");

				SSVU_ASSERT_STATIC(isSame<List<int, float, char>::PopFront, List<float, char>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<float, char>::PopFront, List<char>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<char>::PopFront, List<>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<>::PopFront, List<>>(), "");

				//						  0    1      2     3      4
				SSVU_ASSERT_STATIC(isSame<List<int, float, char, float, int>::SubList<0, 0>, List<>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float, char, float, int>::SubList<0, 1>, List<int>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float, char, float, int>::SubList<0, 3>, List<int, float, char>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float, char, float, int>::SubList<2, 4>, List<char, float>>(), "");
				SSVU_ASSERT_STATIC(isSame<List<int, float, char, float, int>::SubList<0, 999>, List<int, float, char, float, int>>(), "");
			}
		}
	}
}


int main()
{
SSVUT_RUN();
	return 0;

}

#endif

#ifdef TESTING

#include <map>
#include <unordered_map>
#include <SSVUtils/SSVUtils.hpp>

int main()
{
	SSVUT_RUN();

	std::map<int, std::string> m{{1, "aa"}, {2, "bb"}, {3, "cc"}};
	std::unordered_map<int, std::string> um{{1, "aa"}, {2, "bb"}, {3, "cc"}};
	ssvu::Bimap<int, std::string> bm{{1, "aa"}, {2, "bb"}, {3, "cc"}};

	ssvu::lo() << std::vector<int>{1, 2, 3} << std::endl;
	ssvu::lo() << m << std::endl;
	ssvu::lo() << um << std::endl;
	ssvu::lo() << bm << std::endl;

	return 0;
}

#endif

#ifdef TESTING2

#include "SSVBloodshed/OBCommon.hpp"

/*
#define RECURSION_DISPATCHER_IMPL(mFunc, ...) SSVPP_TKNCAT_3(mFunc, _, SSVPP_EXPAND(SSVPP_BOOL(SSVPP_DECREMENT(SSVPP_ARGCOUNT(__VA_ARGS__)))))
#define RECURSION_DISPATCHER(mFunc, ...) RECURSION_DISPATCHER_IMPL(mFunc, __VA_ARGS__)

#define CALL_RECURSION_DISPATCHED_FUNC(mFunc, ...) RECURSION_DISPATCHER(mFunc, __VA_ARGS__) ( __VA_ARGS__ )

#define EXAMPLE_IMPL_0(mArg)	WELP
#define EXAMPLE_IMPL_1(mArg0, mArg1, ...) EXAMPLE_IMPL_0(mArg0) CALL_RECURSION_DISPATCHED_FUNC(EXAMPLE_IMPL, mArg1, __VA_ARGS__)

#define SSVPP_IMPL_VA_ARGS_0(...)
#define SSVPP_IMPL_VA_ARGS_1(...) __VA_ARGS__
#define SSVPP_VA_ARGS(...) SSVPP_EXPAND(SSVPP_TKNCAT_2(SSVPP_IMPL_VA_ARGS_, SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS(__VA_ARGS__))) ( __VA_ARGS__ )
*/
int main()
{
/*
	std::cout << SSVPP_TOSTR(CALL_RECURSION_DISPATCHED_FUNC(EXAMPLE_IMPL, 1, 2, 3)) << std::endl;
	std::cout << SSVPP_TOSTR(CALL_RECURSION_DISPATCHED_FUNC(EXAMPLE_IMPL, 1, 2)) << std::endl;
	std::cout << SSVPP_TOSTR(CALL_RECURSION_DISPATCHED_FUNC(EXAMPLE_IMPL, 1)) << std::endl;

	std::cout << SSVPP_TOSTR(SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS(1, 1, 1)) << std::endl;
	std::cout << SSVPP_TOSTR(SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS(1, 1)) << std::endl;
	std::cout << SSVPP_TOSTR(SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS(1)) << std::endl;
	std::cout << SSVPP_TOSTR(SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS()) << std::endl;

	std::cout << SSVPP_TOSTR_SEP(", ", SSVPP_VA_ARGS(1, 1, 1)) << std::endl;
	std::cout << SSVPP_TOSTR_SEP(", ", SSVPP_VA_ARGS(1, 1)) << std::endl;
	std::cout << SSVPP_TOSTR_SEP(", ", SSVPP_VA_ARGS(1)) << std::endl;
	std::cout << SSVPP_TOSTR(SSVPP_VA_ARGS()) << std::endl;
*/

	//std::cout << SSVPP_TOSTR(SSVPP_ARGCOUNT()) << std::endl;
	return 0;
}

#endif
