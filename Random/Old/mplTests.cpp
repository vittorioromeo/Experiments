#include <tuple>
#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Test/Test.hpp>

namespace ssvu
{
	namespace MPL
	{
		template<typename...> struct List;

		struct Null { };
		template<bool TCond> using BoolResult = Conditional<TCond, TrueT, FalseT>;

		template<int TN1, int TN2> inline constexpr int getMin() noexcept { return TN1 < TN2 ? TN1 : TN2; }
		template<int TN1, int TN2> inline constexpr int getMax() noexcept { return TN1 > TN2 ? TN1 : TN2; }

		namespace Impl
		{
			// VA Args Head
			template<typename... Ts> struct VAHeadHlpr;
			template<typename T, typename... Ts> struct VAHeadHlpr<T, Ts...>	{ using Type = T; };
			template<> struct VAHeadHlpr<>										{ using Type = Null; };
			template<typename... Ts> using VAHead = typename Impl::VAHeadHlpr<Ts...>::Type;

			// VA Args Tail
			template<typename... Ts> struct VATailHlpr;
			template<typename T, typename... Ts> struct VATailHlpr<T, Ts...>	{ using Type = typename VATailHlpr<Ts...>::Type; };
			template<typename T> struct VATailHlpr<T>							{ using Type = T; };
			template<> struct VATailHlpr<>										{ using Type = Null; };
			template<typename... Ts> using VATail = typename Impl::VATailHlpr<Ts...>::Type;

			// List::PopFront
			template<typename> struct PopFrontHlpr;
			template<typename TL, typename... TLAs> struct PopFrontHlpr<List<TL, TLAs...>>	{ using Type = List<TLAs...>; };
			template<> struct PopFrontHlpr<List<>>											{ using Type = List<>; };

			// List::PopBack
			template<typename, typename> struct RemoveLastHlpr;
			template<typename TLA1, typename TLA2, typename... TLAs1, typename... TLAs2> struct RemoveLastHlpr<List<TLA1, TLA2, TLAs1...>, List<TLAs2...>>
			{
				using List1 = List<TLA1, TLA2, TLAs1...>;
				using List2 = List<TLAs2...>;
				using List1WithPop = typename PopFrontHlpr<List1>::Type;
				using List2WithPush = typename List2::template PushBack<typename List1::Head>;
				using Type = typename RemoveLastHlpr<List1WithPop, List2WithPush>::Type;
			};
			template<typename TL1, typename... TLAs2> struct RemoveLastHlpr<List<TL1>, List<TLAs2...>> { using Type = List<TLAs2...>; };
			template<typename> struct PopBackHlpr;
			template<typename... TLAs> struct PopBackHlpr<List<TLAs...>> { using Type = typename RemoveLastHlpr<List<TLAs...>, List<>>::Type; };
			template<> struct PopBackHlpr<List<>> { using Type = List<>; };

			// List::SubList
			template<SizeT, SizeT, SizeT, typename, typename, typename> struct SubListImpl;
			template<SizeT, SizeT, SizeT, typename, typename> struct SubListDispatch;
			template<SizeT TS1, SizeT TS2, SizeT TSC, typename... TLA1s, typename... TLA2s> struct SubListImpl<TS1, TS2, TSC, List<TLA1s...>, List<TLA2s...>, FalseT>
			{
				using Type = typename SubListDispatch<TS1, TS2, TSC + 1, List<TLA1s...>, typename List<TLA2s...>::template PushBack<typename List<TLA1s...>::template Elem<TSC>>>::Type;
			};
			template<SizeT TS1, SizeT TS2, SizeT TSC, typename... TLA1s, typename... TLA2s> struct SubListImpl<TS1, TS2, TSC, List<TLA1s...>, List<TLA2s...>, TrueT>
			{
				using Type = List<TLA2s...>;
			};
			template<SizeT TS1, SizeT TS2, SizeT TSC, typename TL1, typename TL2> struct SubListDispatch
			{
				using Type = typename SubListImpl<TS1, TS2, TSC, TL1, TL2, BoolResult<TSC == TS2 || TSC >= TL1::getSize()>>::Type;
			};
			template<SizeT, SizeT, SizeT, typename> struct SubListHlpr;
			template<SizeT TS1, SizeT TS2, SizeT TSC, typename... TLAs> struct SubListHlpr<TS1, TS2, TSC, List<TLAs...>>
			{
				using Type = typename SubListDispatch<TS1, TS2, TSC, List<TLAs...>, List<>>::Type;
			};


			template<typename T, typename... Ts> struct ContainsHlpr : TrueT { };

			template<typename T, typename THead, typename... Ts> struct ContainsHlpr<T, THead, Ts...>
				: Conditional<isSame<T, THead>(), TrueT, ContainsHlpr<T, Ts...>> { };

			template<typename T> struct ContainsHlpr<T> : FalseT { };


			template<typename, typename> struct AppendHlpr;
			template<typename... Ts1, typename... Ts2> struct AppendHlpr<List<Ts1...>, List<Ts2...>>
			{
				using Type = List<Ts1..., Ts2...>;
			};

			template<typename, typename> struct PrependHlpr;
			template<typename... Ts1, typename... Ts2> struct PrependHlpr<List<Ts1...>, List<Ts2...>>
			{
				using Type = List<Ts2..., Ts1...>;
			};


			template<typename...> struct UniqueHlpr;
		}

		template<typename... Ts> struct List
		{
			using Type = List<Ts...>;

			using Head = Impl::VAHead<Ts...>;
			using Tail = Impl::VATail<Ts...>;

			inline static constexpr SizeT getSize() noexcept { return sizeof...(Ts); }

			template<SizeT TS> using Elem = std::tuple_element_t<TS, std::tuple<Ts...>>;
			template<SizeT TS> using ElemReverse = Elem<getSize() - 1 - TS>;

			template<typename T> using PushBack = List<Ts..., T>;
			template<typename T> using PushFront = List<T, Ts...>;

			template<typename TL> using Append = typename Impl::AppendHlpr<Type, TL>::Type;
			template<typename TL> using Prepend = typename Impl::PrependHlpr<Type, TL>::Type;

			using PopBack = typename Impl::PopBackHlpr<List<Ts...>>::Type;
			using PopFront = typename Impl::PopFrontHlpr<List<Ts...>>::Type;
			using Clear = List<>;

			template<SizeT TS1, SizeT TS2> using SubList = typename Impl::SubListHlpr<TS1, TS2, TS1, List<Ts...>>::Type;

			template<typename T> inline static constexpr bool contains() noexcept { return Impl::ContainsHlpr<T, Ts...>(); }

			// TODO: insert
		};


		namespace Impl
		{

			/*
			template<typename... Ts> struct UniqueHlpr
			{

			};
			template<typename T, typename... Ts> struct UniqueHlpr<T, Ts...>
			{
				using UniqueList = Conditional<
					List<Ts...>::contains<T>(),
					UniqueHlpr<Ts...>::UniqueList,
					List<T>::PushBack<UniqueHlpr<
			};
			template<typename T> struct UniqueHlpr<T>
			{
				using UniqueList = List<T>;
			};
			*/
		}

	}
}

int main()
{
	using namespace ssvu;
	using namespace ssvu::MPL;

	SSVU_ASSERT_STATIC_NM(getMin<15, 10>() == 10);
	SSVU_ASSERT_STATIC_NM(getMin<-15, 10>() == -15);

	SSVU_ASSERT_STATIC_NM(getMax<15, 10>() == 15);
	SSVU_ASSERT_STATIC_NM(getMax<-15, 10>() == 10);

	SSVU_ASSERT_STATIC_NM(isSame<Impl::VAHead<int, float, char>, int>());
	SSVU_ASSERT_STATIC_NM(isSame<Impl::VATail<int, float, char>, char>());

	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::Head, int>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::Tail, char>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::Head, Null>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::Tail, Null>());

	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::Elem<0>, int>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::Elem<1>, float>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::Elem<2>, char>());

	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::ElemReverse<0>, char>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::ElemReverse<1>, float>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::ElemReverse<2>, int>());

	SSVU_ASSERT_STATIC_NM(isSame<List<>::PushBack<int>, List<int>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int>::PushBack<float>, List<int, float>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float>::PushBack<char>, List<int, float, char>>());

	SSVU_ASSERT_STATIC_NM(isSame<List<>::PushFront<int>, List<int>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int>::PushFront<float>, List<float, int>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<float, int>::PushFront<char>, List<char, float, int>>());

	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::PopBack, List<int, float>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float>::PopBack, List<int>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int>::PopBack, List<>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::PopBack, List<>>());

	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char>::PopFront, List<float, char>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<float, char>::PopFront, List<char>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<char>::PopFront, List<>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::PopFront, List<>>());

	//						         0      1     2      3    4
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char, float, int>::SubList<0, 0>, List<>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char, float, int>::SubList<0, 1>, List<int>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char, float, int>::SubList<0, 3>, List<int, float, char>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char, float, int>::SubList<2, 4>, List<char, float>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float, char, float, int>::SubList<0, 999>, List<int, float, char, float, int>>());

	SSVU_ASSERT_STATIC_NM(List<int, float, char, float, int>::contains<int>());
	SSVU_ASSERT_STATIC_NM(List<int, float, char, float, int>::contains<float>());
	SSVU_ASSERT_STATIC_NM(List<int, float, char, float, int>::contains<char>());
	SSVU_ASSERT_STATIC_NM(!List<int, float, char, float, int>::contains<double>());
	SSVU_ASSERT_STATIC_NM(!List<int, float, char, float, int>::contains<unsigned char>());
	SSVU_ASSERT_STATIC_NM(!List<>::contains<unsigned char>());

	SSVU_ASSERT_STATIC_NM(isSame<List<int, float>::Append<List<int, float>>, List<int, float, int, float>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float>::Append<List<>>, List<int, float>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::Append<List<int, float>>, List<int, float>>());

	SSVU_ASSERT_STATIC_NM(isSame<List<int, float>::Prepend<List<char, float>>, List<char, float, int, float>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<int, float>::Prepend<List<>>, List<int, float>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::Prepend<List<int, float>>, List<int, float>>());

	SSVUT_RUN();
	return 0;

}
