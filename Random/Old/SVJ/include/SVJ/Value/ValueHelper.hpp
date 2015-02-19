// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_VALUE_VALUEHELPER
#define SVJ_VALUE_VALUEHELPER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			#define SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL(mType) \
				template<> struct ValueHelper<mType> \
				{ \
					inline static void set(Value& mV, const mType& mX) { mV.setNumber(Number{mX}); } \
					inline static decltype(auto) get(const Value& mV) { return mV.getNumber().get<mType>(); } \
					inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Number; } \
				};

			#define SVJ_DEFINE_VALUEHELPER_BIG_MUTABLE(mType) \
				template<> struct ValueHelper<mType> \
				{ \
					template<typename T> inline static void set(Value& mV, T&& mX) { SSVPP_CAT(mV.set, mType)(fwd<T>(mX)); } \
					template<typename T> inline static decltype(auto) get(T&& mV) { return SSVPP_CAT(fwd<T>(mV).get, mType)(); } \
					inline static auto is(const Value& mV) noexcept { return mV.getType() == SSVPP_EXPAND(Value::Type::mType); } \
				};

			#define SVJ_DEFINE_VALUEHELPER_SMALL_IMMUTABLE(mType) \
				template<> struct ValueHelper<mType> \
				{ \
					inline static void set(Value& mV, const mType& mX) { SSVPP_CAT(mV.set, mType)(mX); } \
					inline static decltype(auto) get(const Value& mV) { return SSVPP_CAT(mV.get, mType)(); } \
					inline static auto is(const Value& mV) noexcept { return mV.getType() == SSVPP_EXPAND(Value::Type::mType); } \
				};

			SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL(char)
			SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL(int)
			SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL(long int)
			SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL(unsigned char)
			SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL(unsigned int)
			SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL(unsigned long int)
			SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL(float)
			SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL(double)

			SVJ_DEFINE_VALUEHELPER_BIG_MUTABLE(Object)
			SVJ_DEFINE_VALUEHELPER_BIG_MUTABLE(Array)
			SVJ_DEFINE_VALUEHELPER_BIG_MUTABLE(String)

			SVJ_DEFINE_VALUEHELPER_SMALL_IMMUTABLE(Number)
			SVJ_DEFINE_VALUEHELPER_SMALL_IMMUTABLE(Bool)
			SVJ_DEFINE_VALUEHELPER_SMALL_IMMUTABLE(Null)

			#undef SVJ_DEFINE_VALUEHELPER_NUMBER_IMPL
			#undef SVJ_DEFINE_VALUEHELPER_BIG_MUTABLE
			#undef SVJ_DEFINE_VALUEHELPER_SMALL_IMMUTABLE

			template<> struct ValueHelper<Value>
			{
				template<typename T> inline static void set(Value& mV, T&& mX) { mV.init(fwd<T>(mX)); }
			};

			template<std::size_t TS> struct ValueHelper<char[TS]>
			{
				inline static void set(Value& mV, const char(&mX)[TS]) { mV.setString(mX); }
				inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::String; }
			};
		}
	}
}

#endif

