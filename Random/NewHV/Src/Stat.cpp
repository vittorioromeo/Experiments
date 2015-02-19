// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "../../NewHV/Inc/Common.hpp"
#include "../../NewHV/Inc/Stat.hpp"

namespace ssvu
{
	namespace Internal
	{	
		SSVU_INLINE HVStat::HVStat(HIdx mMarkIdx) noexcept : markIdx{mMarkIdx} { }	
	}
}