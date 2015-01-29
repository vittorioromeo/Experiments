#include <tuple>
#include <SSVUtils/SSVUtils.hpp>

namespace ssvu
{
	namespace MPL
	{
		template<typename...> struct List;

		struct Null { };
		template<bool TCond> using BoolResult = Conditional<TCond, std::true_type, std::false_type>;

		template<int TN1, int TN2> inline constexpr int getMin() noexcept	{ return TN1 < TN2 ? TN1 : TN2; }
		template<int TN1, int TN2> inline constexpr int getMax() noexcept	{ return TN1 > TN2 ? TN1 : TN2; }

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
			template<SizeT, SizeT, SizeT, typename, typename, typename> struct SubListImpl;
			template<SizeT, SizeT, SizeT, typename, typename> struct SubListDispatch;
			template<SizeT TS1, SizeT TS2, SizeT TSC, typename... TLA1s, typename... TLA2s> struct SubListImpl<TS1, TS2, TSC, List<TLA1s...>, List<TLA2s...>, std::false_type>
			{
				using Type = typename SubListDispatch<TS1, TS2, TSC + 1, List<TLA1s...>, typename List<TLA2s...>::template PushBack<typename List<TLA1s...>::template Elem<TSC>>>::Type;
			};
			template<SizeT TS1, SizeT TS2, SizeT TSC, typename... TLA1s, typename... TLA2s> struct SubListImpl<TS1, TS2, TSC, List<TLA1s...>, List<TLA2s...>, std::true_type>
			{
				using Type = List<TLA2s...>;
			};
			template<SizeT TS1, SizeT TS2, SizeT TSC, typename TL1, typename TL2> struct SubListDispatch
			{
				using Type = typename SubListImpl<TS1, TS2, TSC, TL1, TL2, BoolResult<TSC == TS2 || TSC >= TL1::getSize()>>::Type;
			};
			template<SizeT, SizeT, SizeT, typename> struct SubListHelper;
			template<SizeT TS1, SizeT TS2, SizeT TSC, typename... TLAs> struct SubListHelper<TS1, TS2, TSC, List<TLAs...>>
			{
				using Type = typename SubListDispatch<TS1, TS2, TSC, List<TLAs...>, List<>>::Type;
			};
		}

		template<typename... TArgs> struct List
		{
			using Head = Internal::VAHead<TArgs...>;
			using Tail = Internal::VATail<TArgs...>;

			inline static constexpr SizeT getSize() noexcept { return sizeof...(TArgs); }

			template<SizeT TS> using Elem = std::tuple_element_t<TS, std::tuple<TArgs...>>;
			template<SizeT TS> using ElemReverse = Elem<getSize() - 1 - TS>;

			template<typename T> using PushBack = List<TArgs..., T>;
			template<typename T> using PushFront = List<T, TArgs...>;

			using PopBack = typename Internal::PopBackHelper<List<TArgs...>>::Type;
			using PopFront = typename Internal::PopFrontHelper<List<TArgs...>>::Type;

			template<SizeT TS1, SizeT TS2> using SubList = typename Internal::SubListHelper<TS1, TS2, TS1, List<TArgs...>>::Type;

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

				//						         0      1     2      3    4
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