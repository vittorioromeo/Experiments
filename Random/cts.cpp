#include <SSVUtils/Core/Core.hpp>

namespace ssvu
{
	template<char...> struct CharList;

	namespace Impl
	{	
		template<typename> struct CLPopFrontHlpr;
		template<char TL, char... TLAs> struct CLPopFrontHlpr<CharList<TL, TLAs...>>	{ using Type = CharList<TLAs...>; };
		template<> struct CLPopFrontHlpr<CharList<>>									{ using Type = CharList<>; };
	
		template<typename, typename> struct RemoveLastHlpr;
		template<char TLA1, char TLA2, char... TLAs1, char... TLAs2> struct RemoveLastHlpr<CharList<TLA1, TLA2, TLAs1...>, CharList<TLAs2...>>
		{
			using List1 = CharList<TLA1, TLA2, TLAs1...>;
			using List2 = CharList<TLAs2...>;
			using List1WithPop = typename CLPopFrontHlpr<List1>::Type;
			using List2WithPush = typename List2::template PushBack<List1::head>;
			using Type = typename RemoveLastHlpr<List1WithPop, List2WithPush>::Type;
		};
		template<char TL1, char... TLAs2> struct RemoveLastHlpr<CharList<TL1>, CharList<TLAs2...>> { using Type = CharList<TLAs2...>; };
		
		template<typename> struct CLPopBackHlpr;
		template<char... TLAs> struct CLPopBackHlpr<CharList<TLAs...>> { using Type = typename RemoveLastHlpr<CharList<TLAs...>, CharList<>>::Type; };
		template<> struct CLPopBackHlpr<CharList<>> { using Type = CharList<>; };

		template<char T, char... Ts> struct CLContainsHlpr : TrueT { };
		template<char T, char THead, char... Ts> struct CLContainsHlpr<T, THead, Ts...> : Conditional<T == THead, TrueT, CLContainsHlpr<T, Ts...>> { };
		template<char T> struct CLContainsHlpr<T> : FalseT { };

		template<typename, typename> struct CLTrimExtraZeros;
		template<typename TResult> struct CLTrimExtraZeros<CharList<>, TResult>
		{
			using Type = TResult;
		};
		template<char T, char... T1s, typename TResult> struct CLTrimExtraZeros<CharList<T, T1s...>, TResult>
		{
			using AddedList = typename CLTrimExtraZeros<CharList<T1s...>, typename TResult::template PushBack<T>>::Type;
			using Type = Conditional<T == '\0', typename TResult::template PushBack<'\0'>, AddedList>;
		};

		// h e l l o w o r l d 0 
		// ^
		// l l o 0
		// ^

		// 0 1 2 3 
		// h e y
		// y 

		template<int, int, typename, typename> struct CLContainsLHlpr;
		template<bool, int, int, typename, typename> struct Test2;
		template<bool, int, int, typename, typename> struct Test1;

		template<int TI1, int TI2, char... TCs1, char... TCs2> struct Test2<false, TI1, TI2, CharList<TCs1...>, CharList<TCs2...>>
		{
			using L1 = CharList<TCs1...>;
			using L2 = CharList<TCs2...>;
			static constexpr int sz1{L1::size};
			static constexpr int sz2{L2::size};

			using Type = typename CLContainsLHlpr<TI1 + 1, TI2 + 1, L1, L2>::Type;
		};
		template<int TI1, int TI2, char... TCs1, char... TCs2> struct Test2<true, TI1, TI2, CharList<TCs1...>, CharList<TCs2...>>
		{
			using Type = TrueT;
		};


		template<int TI1, int TI2, char... TCs1, char... TCs2> struct Test1<false, TI1, TI2, CharList<TCs1...>, CharList<TCs2...>>
		{
			using L1 = CharList<TCs1...>;
			using L2 = CharList<TCs2...>;
			static constexpr int sz1{L1::size};
			static constexpr int sz2{L2::size};

			using Type = Conditional
			<
				(L1::template at<TI1>() != L2::template at<TI2>()),
				typename CLContainsLHlpr<TI1 + 1, 0, L1, L2>::Type,
				typename Test2<(TI2 == sz2 - 1), TI1, TI2, L1, L2>::Type
			>;
		};
		template<int TI1, int TI2, char... TCs1, char... TCs2> struct Test1<true, TI1, TI2, CharList<TCs1...>, CharList<TCs2...>>
		{
			using Type = FalseT;
		};

		// 0 1 2 3 
		// h e y
		// h e y

		
		template<int TI1, int TI2, char... TCs1, char... TCs2> struct CLContainsLHlpr<TI1, TI2, CharList<TCs1...>, CharList<TCs2...>>
		{
			using L1 = CharList<TCs1...>;
			using L2 = CharList<TCs2...>;
			static constexpr int sz1{L1::size};
			static constexpr int sz2{L2::size};
	
			using Type = typename Test1<(TI1 >= sz1), TI1, TI2, L1, L2>::Type;

			/*
			using Type2 = Conditional
			<
				(TI1 >= (sz1 - sz2)),
				FalseT,
				Conditional
				<
					(L1::template at<TI1>() != L2::template at<TI2>()),
					typename CLContainsLHlpr<TI1 + 1, 0, L1, L2>::Type,
					Conditional
					<
						(TI2 == sz2 - 1),
						TrueT,
						typename CLContainsLHlpr<TI1 + 1, TI2 + 1, L1, L2>::Type
					>
				>
			>;			
			*/
		};

		template<SizeT, SizeT, char...> struct CLNth;
		template<SizeT TI, SizeT TTrg> struct CLNth<TI, TTrg> : IntegralConstant<char, '\0'> { };
		template<SizeT TI, SizeT TTrg, char TC, char... TCs> struct CLNth<TI, TTrg, TC, TCs...> : IntegralConstant<char, TI == TTrg ? TC : CLNth<TI + 1, TTrg, TCs...>{}()> { };
	}

	template<char... TChars> struct CharList 
	{ 
		using Type = CharList<TChars...>;



		static constexpr SizeT size{sizeof...(TChars)};
		template<SizeT TI> inline static constexpr char at() { return Impl::CLNth<0, TI, TChars...>::value; }
		static constexpr char head{size > 0 ? at<0>() : '\0'};
		static constexpr char tail{size > 0 ? at<size - 1>() : '\0'};
		template<char TC> inline static constexpr bool has() { return Impl::CLContainsHlpr<TC, TChars...>(); }
		// TODO: template<char TC> static constexpr bool countOf{Impl::CLContainsHlpr<TC, TChars...>()};

		template<char TC> using PushFront = CharList<TC, TChars...>;
		template<char TC> using PushBack = CharList<TChars..., TC>;
		using PopFront = typename Impl::CLPopFrontHlpr<Type>::Type;
		using PopBack = typename Impl::CLPopBackHlpr<Type>::Type;
		using TrimExtraZeros = typename Impl::CLTrimExtraZeros<Type, CharList<>>::Type;

		template<typename TL> inline static constexpr bool has() noexcept 
		{ 
			return typename Impl::CLContainsLHlpr<0, 0, Type::PopBack, typename TL::PopBack>::Type{}(); 
		}


		static constexpr const char cstr[]{TChars...};
		inline static std::string toStr() { return std::string{TChars...}; }

	};

	namespace Impl
	{
		template<SizeT TN> inline constexpr char getCTStrC(const char (&mS)[TN], SizeT mI) noexcept
		{
			return mI >= TN ? '\0' : mS[mI];
		}

		template<SizeT TN> inline constexpr char getCTStrSize(const char (&)[TN]) noexcept
		{
			return TN;
		}
	}

	namespace CTLiteral
	{
		template<typename T, T... TChars> inline constexpr typename CharList<TChars...>::TrimExtraZeros operator""_cts() { return {}; }
	}
}

#define SSVPP_IMPL_REPEAT_DEC_0(mAction, mData) 
#define SSVPP_IMPL_REPEAT_DEC_1(mAction, mData) mAction(0, mData) SSVPP_IMPL_REPEAT_DEC_0(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_2(mAction, mData) mAction(1, mData) SSVPP_IMPL_REPEAT_DEC_1(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_3(mAction, mData) mAction(2, mData) SSVPP_IMPL_REPEAT_DEC_2(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_4(mAction, mData) mAction(3, mData) SSVPP_IMPL_REPEAT_DEC_3(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_5(mAction, mData) mAction(4, mData) SSVPP_IMPL_REPEAT_DEC_4(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_6(mAction, mData) mAction(5, mData) SSVPP_IMPL_REPEAT_DEC_5(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_7(mAction, mData) mAction(6, mData) SSVPP_IMPL_REPEAT_DEC_6(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_8(mAction, mData) mAction(7, mData) SSVPP_IMPL_REPEAT_DEC_7(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_9(mAction, mData) mAction(8, mData) SSVPP_IMPL_REPEAT_DEC_8(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_10(mAction, mData) mAction(9, mData) SSVPP_IMPL_REPEAT_DEC_9(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_11(mAction, mData) mAction(10, mData) SSVPP_IMPL_REPEAT_DEC_10(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_12(mAction, mData) mAction(11, mData) SSVPP_IMPL_REPEAT_DEC_11(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_13(mAction, mData) mAction(12, mData) SSVPP_IMPL_REPEAT_DEC_12(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_14(mAction, mData) mAction(13, mData) SSVPP_IMPL_REPEAT_DEC_13(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_15(mAction, mData) mAction(14, mData) SSVPP_IMPL_REPEAT_DEC_14(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_16(mAction, mData) mAction(15, mData) SSVPP_IMPL_REPEAT_DEC_15(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_17(mAction, mData) mAction(16, mData) SSVPP_IMPL_REPEAT_DEC_16(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_18(mAction, mData) mAction(17, mData) SSVPP_IMPL_REPEAT_DEC_17(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_19(mAction, mData) mAction(18, mData) SSVPP_IMPL_REPEAT_DEC_18(mAction, mData)
#define SSVPP_IMPL_REPEAT_DEC_20(mAction, mData) mAction(19, mData) SSVPP_IMPL_REPEAT_DEC_19(mAction, mData)

#define SSVPP_IMPL_REPEAT_DEC(mTimes, mAction, mData) SSVPP_TKNCAT_2(SSVPP_IMPL_REPEAT_DEC_, mTimes) (mAction, mData)
#define SSVPP_REPEAT_DEC(...) SSVPP_IMPL_REPEAT_DEC(__VA_ARGS__)


#define SSVPP_IMPL_REPEAT_INC_0(mAction, mData, mLast) 
#define SSVPP_IMPL_REPEAT_INC_1(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_0(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_2(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_1(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_3(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_2(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_4(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_3(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_5(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_4(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_6(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_5(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_7(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_6(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_8(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_7(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_9(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_8(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_10(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_9(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_11(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_10(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_12(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_11(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_13(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_12(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_14(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_13(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_15(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_14(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_16(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_15(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_17(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_16(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_18(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_17(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_19(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_18(mAction, mData, SSVPP_INCREMENT(mLast))
#define SSVPP_IMPL_REPEAT_INC_20(mAction, mData, mLast) mAction(mLast, mData) SSVPP_IMPL_REPEAT_INC_19(mAction, mData, SSVPP_INCREMENT(mLast))

#define SSVPP_IMPL_REPEAT_INC(mTimes, mAction, mData) SSVPP_TKNCAT_2(SSVPP_IMPL_REPEAT_INC_, mTimes) (mAction, mData, 0)
#define SSVPP_REPEAT_INC(...) SSVPP_IMPL_REPEAT_INC(__VA_ARGS__)



#define CTSTR_MAX_SIZE 20

#define SSVU_CTSTR_CHARS_IMPL(mIdx, mData) SSVPP_COMMA_IF(mIdx) ::ssvu::Impl::getCTStrC(mData, mIdx) 
#define SSVU_CTSTR_CHARS_WLEN(mLen, mStr) SSVPP_IMPL_REPEAT_INC(mLen, SSVU_CTSTR_CHARS_IMPL, mStr)
#define SSVU_CTSTR_CHARS(mStr) SSVU_CTSTR_CHARS_WLEN(CTSTR_MAX_SIZE, mStr)
// #define SSVU_CTSTR_WLEN(mLen, mStr) ::ssvu::CTString<SSVU_CTSTR_CHARS_WLEN(mLen, mStr)>
#define SSVU_CHARLIST(mStr) ::ssvu::CharList<SSVU_CTSTR_CHARS(mStr)>::TrimExtraZeros
#define SSVU_CTSTR(mStr) SSVU_CHARLIST(mStr)


int main()
{
	SSVU_ASSERT_STATIC_NM(ssvu::isSame<ssvu::CharList<'h', 'e', 'y'>::PushBack<'u'>, ssvu::CharList<'h', 'e', 'y', 'u'>>());
	SSVU_ASSERT_STATIC_NM(ssvu::isSame<ssvu::CharList<'h', 'e', 'e', 'y', '\0', '\0', '\0'>::TrimExtraZeros, ssvu::CharList<'h', 'e', 'e', 'y', '\0'>>());

	SSVU_ASSERT_STATIC_NM(ssvu::CharList<'h', 'e', 'y'>::at<0>() == 'h');
	SSVU_ASSERT_STATIC_NM(ssvu::CharList<'h', 'e', 'y'>::at<1>() == 'e');
	SSVU_ASSERT_STATIC_NM(ssvu::CharList<'h', 'e', 'y'>::at<2>() == 'y');

	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::at<0>() == 'h');
	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::at<1>() == 'e');
	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::at<2>() == 'y');

	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::has<'h'>());
	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::has<'e'>());
	SSVU_ASSERT_STATIC_NM(!SSVU_CTSTR("hey")::has<'z'>());
	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::has<'\0'>());

	using namespace ssvu::CTLiteral;
	SSVU_ASSERT_STATIC_NM(decltype("hey"_cts)::at<0>() == 'h');
	SSVU_ASSERT_STATIC_NM(decltype("hey"_cts)::at<1>() == 'e');
	SSVU_ASSERT_STATIC_NM(decltype("hey"_cts)::at<2>() == 'y');

	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::has<SSVU_CTSTR("h")>());
	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::has<SSVU_CTSTR("e")>());
	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::has<SSVU_CTSTR("y")>());
	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::has<SSVU_CTSTR("he")>());
	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::has<SSVU_CTSTR("ey")>());
	SSVU_ASSERT_STATIC_NM(SSVU_CTSTR("hey")::has<SSVU_CTSTR("hey")>());
	SSVU_ASSERT_STATIC_NM(!SSVU_CTSTR("hey")::has<SSVU_CTSTR("z")>());
	SSVU_ASSERT_STATIC_NM(!SSVU_CTSTR("hey")::has<SSVU_CTSTR("\0")>());
	SSVU_ASSERT_STATIC_NM(!SSVU_CTSTR("hey")::has<SSVU_CTSTR("hhey")>());
	SSVU_ASSERT_STATIC_NM(!SSVU_CTSTR("hey")::has<SSVU_CTSTR("")>());

	//using TestT = SSVU_CTSTR_WLEN(5, "hello");
	using TestT = SSVU_CTSTR("hello");

	ssvu::lo() << std::string{TestT::toStr()} << std::endl;

//	using KK = ssvu::CharList<'a', 'b', 'c'>;

	//using hello_world = decltype("Hello world!"_cts);
	//using hello_world = ssvu::CTString
}