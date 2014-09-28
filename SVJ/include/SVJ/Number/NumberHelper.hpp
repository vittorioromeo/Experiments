// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_NUMBER_NUMBERHELPER
#define SVJ_NUMBER_NUMBERHELPER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			#define SVJ_DEFINE_NUMBERHELPER(mType, mRepresentation) \
				template<> struct NumberHelper<mType> \
				{ \
					inline static void set(Number& mN, mType mX) noexcept { return SSVPP_CAT(mN.set, mRepresentation)(mX); } \
					inline static auto get(const Number& mN) noexcept { return SSVPP_CAT(mN.get, mRepresentation)(); } \
				};

			SVJ_DEFINE_NUMBERHELPER(char, IntS)
			SVJ_DEFINE_NUMBERHELPER(int, IntS)
			SVJ_DEFINE_NUMBERHELPER(long int, IntS)

			SVJ_DEFINE_NUMBERHELPER(unsigned char, IntU)
			SVJ_DEFINE_NUMBERHELPER(unsigned int, IntU)
			SVJ_DEFINE_NUMBERHELPER(unsigned long int, IntU)

			SVJ_DEFINE_NUMBERHELPER(float, Real)
			SVJ_DEFINE_NUMBERHELPER(double, Real)

			#undef SVJ_DEFINE_NUMBERHELPER
		}
	}
}

#endif
