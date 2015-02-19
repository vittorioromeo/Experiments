// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_MARK
#define SSVU_NEWHV_MARK

#include "../../NewHV/Inc/Common.hpp"

namespace ssvu
{
	namespace Internal
	{
		/// @brief Data structure keeping track of the validity of an item.
		struct HVMark
		{
			HIdx statIdx;
			HCtr ctr;

			HVMark(HIdx mStatIdx) noexcept;
		};
	}
}

#endif