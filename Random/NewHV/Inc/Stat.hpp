// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_STAT
#define SSVU_NEWHV_STAT

namespace ssvu
{
	namespace Internal
	{
		/// @brief Data structure keeping track of the status of an item.
		struct HVStat
		{
			HIdx markIdx;
			bool alive{false};

			inline HVStat(HIdx mMarkIdx) noexcept : markIdx{mMarkIdx} { }
		};
	}
}

#endif