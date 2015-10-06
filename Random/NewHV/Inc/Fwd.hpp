// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_FWD
#define SSVU_NEWHV_FWD

namespace ssvu
{
    namespace Internal
    {
        template <typename, typename>
        class HVImpl;
    }

    template <typename>
    class HVSingle;
    template <typename...>
    class HVMulti;
}

#endif