// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cassert>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/math/angle.hpp>

VRM_SDL_NAMESPACE
{
    template <sz_t TPrecision>
    struct trig_table
    {
    private:
        static constexpr sz_t count{TPrecision};
        static constexpr float ratio{TPrecision / tau};
        std::array<float, count> arr;

    public:
        template <typename TF>
        trig_table(TF&& f) noexcept
        {
            for(auto i(0u); i < count; ++i) arr[i] = f(i / ratio);
        }

        VRM_SDL_ALWAYS_INLINE auto get(float x) const noexcept
        {
            auto idx(vrmc::to_sz_t(x * ratio));
            VRM_CORE_ASSERT(idx < count);

            return arr[idx];
        }
    };
}
VRM_SDL_NAMESPACE_END
