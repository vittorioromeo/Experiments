#include <SSVUtils/Core/Core.hpp>

namespace ssvu
{
	template<char TC> using CTChar = IntegralConstant<char, TC>;
	template<char... TCs> using ListChar = MPL::ListIC<char, TCs...>;

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

		template<typename T> struct IsCTNonZero : IntegralConstant<bool, !isSame<T, CTChar<'\0'>>()>
		{
			
		};
	}

	template<typename TL> using TrimExtraZeros = typename TL::template Filter<Impl::IsCTNonZero>;

	namespace CTLiteral
	{
		template<typename T, T... TChars> inline constexpr TrimExtraZeros<ListChar<TChars...>> operator""_cts() { return {}; }
	}


}





#define CTSTR_MAX_SIZE 20

#define SSVCTS_CHARS_IMPL(mIdx, mData) SSVPP_COMMA_IF(mIdx) ::ssvu::Impl::getCTStrC(mData, mIdx) 
#define SSVCTS_CHARS_WLEN(mLen, mStr) SSVPP_IMPL_REPEAT_INC(mLen, SSVCTS_CHARS_IMPL, mStr)
#define SSVCTS_CHARS(mStr) SSVCTS_CHARS_WLEN(CTSTR_MAX_SIZE, mStr)
// #define SSVCTS_WLEN(mLen, mStr) ::ssvu::CTString<SSVCTS_CHARS_WLEN(mLen, mStr)>
#define SSVU_CHARLIST(mStr) ::ssvu::TrimExtraZeros<::ssvu::ListChar<SSVCTS_CHARS(mStr)>>
// #define SSVCTS(mStr) SSVU_CHARLIST(mStr)
#define SSVCTS(mStr) decltype(mStr ## _cts)

#define SSVCTS_ADD(mStr) Append<SSVCTS(mStr)>
#define SSVCTS_HAS(mStr) hasSeq<SSVCTS(mStr)>()
#define SSVCTS_SUBSTR(mStart, mEnd) Slice<mStart, mEnd>
#define SSVCTS_REPLACE_ALL(mSrc, mNew) ReplaceAllOfSeq<SSVCTS(mSrc), SSVCTS(mNew)>

int main()
{
	using namespace ssvu;
	using namespace ssvu::CTLiteral;
	SSVU_ASSERT_STATIC_NM(isSame<ListChar<'h', 'e', 'y'>::PushBack<CTChar<'u'>>, ListChar<'h', 'e', 'y', 'u'>>());
	SSVU_ASSERT_STATIC_NM(isSame<SSVCTS("hey"), SSVCTS("hey")>());
	SSVU_ASSERT_STATIC_NM(isSame<SSVCTS("hey"), ListChar<'h', 'e', 'y'>>());

	SSVU_ASSERT_STATIC_NM(isSame<ListChar<'h', 'e', 'y'>::Append<ListChar<'b', 'r', 'o'>>, ListChar<'h', 'e', 'y', 'b', 'r', 'o'>>());
	SSVU_ASSERT_STATIC_NM(isSame<SSVCTS("hey")::SSVCTS_ADD("bro"), SSVCTS("heybro")>());
	SSVU_ASSERT_STATIC_NM(SSVCTS("heybro")::SSVCTS_HAS("hey"));
	SSVU_ASSERT_STATIC_NM(SSVCTS("heybro")::SSVCTS_HAS("bro"));
	SSVU_ASSERT_STATIC_NM(SSVCTS("heybro")::SSVCTS_HAS("heybro"));
	SSVU_ASSERT_STATIC_NM(SSVCTS("heybro")::SSVCTS_HAS("h"));
	SSVU_ASSERT_STATIC_NM(SSVCTS("heybro")::SSVCTS_HAS("o"));
	SSVU_ASSERT_STATIC_NM(SSVCTS("heybro")::SSVCTS_HAS("eybr"));
	SSVU_ASSERT_STATIC_NM(!SSVCTS("heybro")::SSVCTS_HAS("bvsdd"));

	SSVU_ASSERT_STATIC_NM(isSame<SSVCTS("hey")::SSVCTS_SUBSTR(0, 2), SSVCTS("he")>());

	// If the string is any longer, it kills the compiler. Problem is in MPL ReplaceAllOfSeq
	SSVU_ASSERT_STATIC_NM(isSame<SSVCTS("hey banana")::SSVCTS_REPLACE_ALL("banana", "apple"), SSVCTS("hey apple")>());

	/*SSVU_ASSERT_STATIC_NM(ssvu::isSame<ssvu::ListChar<'h', 'e', 'e', 'y', '\0', '\0', '\0'>::TrimExtraZeros, ssvu::ListChar<'h', 'e', 'e', 'y', '\0'>>());

	SSVU_ASSERT_STATIC_NM(ssvu::ListChar<'h', 'e', 'y'>::at<0>() == 'h');
	SSVU_ASSERT_STATIC_NM(ssvu::ListChar<'h', 'e', 'y'>::at<1>() == 'e');
	SSVU_ASSERT_STATIC_NM(ssvu::ListChar<'h', 'e', 'y'>::at<2>() == 'y');

	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::at<0>() == 'h');
	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::at<1>() == 'e');
	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::at<2>() == 'y');

	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::has<'h'>());
	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::has<'e'>());
	SSVU_ASSERT_STATIC_NM(!SSVCTS("hey")::has<'z'>());
	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::has<'\0'>());

	using namespace ssvu::CTLiteral;
	SSVU_ASSERT_STATIC_NM(decltype("hey"_cts)::at<0>() == 'h');
	SSVU_ASSERT_STATIC_NM(decltype("hey"_cts)::at<1>() == 'e');
	SSVU_ASSERT_STATIC_NM(decltype("hey"_cts)::at<2>() == 'y');

	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::has<SSVCTS("h")>());
	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::has<SSVCTS("e")>());
	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::has<SSVCTS("y")>());
	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::has<SSVCTS("he")>());
	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::has<SSVCTS("ey")>());
	SSVU_ASSERT_STATIC_NM(SSVCTS("hey")::has<SSVCTS("hey")>());
	SSVU_ASSERT_STATIC_NM(!SSVCTS("hey")::has<SSVCTS("z")>());
	SSVU_ASSERT_STATIC_NM(!SSVCTS("hey")::has<SSVCTS("\0")>());
	SSVU_ASSERT_STATIC_NM(!SSVCTS("hey")::has<SSVCTS("hhey")>());
	SSVU_ASSERT_STATIC_NM(!SSVCTS("hey")::has<SSVCTS("")>());

	//using TestT = SSVCTS_WLEN(5, "hello");
	using TestT = SSVCTS("hello");

	ssvu::lo() << std::string{TestT::toStr()} << std::endl;

//	using KK = ssvu::ListChar<'a', 'b', 'c'>;

	//using hello_world = decltype("Hello world!"_cts);
	//using hello_world = ssvu::CTString*/
}