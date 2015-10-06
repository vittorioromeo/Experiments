// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_STAT
#define SSVU_NEWHV_STAT

#include "../../NewHV/Inc/Common.hpp"

namespace ssvu
{
    namespace Internal
    {
        /// @brief Data structure keeping track of the status of an item.
        struct HVStat
        {
            HIdx markIdx;
            bool alive{false};

            HVStat(HIdx mMarkIdx) noexcept;
        };
    }
}

#endif