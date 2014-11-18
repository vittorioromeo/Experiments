// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_MARK
#define SSVU_NEWHV_MARK

namespace ssvu
{
	namespace Internal
	{
		/// @brief Data structure keeping track of the validity of an item.
		struct HVMark
		{
			HIdx statIdx;
			HCtr ctr;

			inline HVMark(HIdx mStatIdx) noexcept : statIdx{mStatIdx} { }
		};
	}
}

#endif