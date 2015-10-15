// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cassert>
#include <vrm/sdl/common.hpp>

namespace vrm
{
    namespace sdl
    {
        constexpr float pi{3.14159265f};
        constexpr float pi_half{pi / 2.f};
        constexpr float tau{pi * 2.f};
        constexpr float rad_deg_ratio{pi / 180.f};

        template <typename T>
        constexpr auto to_rad(const T& x) noexcept
        {
            return x * rad_deg_ratio;
        }

        template <typename T>
        constexpr auto to_deg(const T& x) noexcept
        {
            return x / rad_deg_ratio;
        }

        template <typename T>
        auto wrap_rad(const T& x) noexcept
        {
            assert(x >= -sdl::tau && x <= 2.f * sdl::tau);

            if(x < 0) return x + sdl::tau;
            if(x > sdl::tau) return x - sdl::tau;

            return x;
        }

        template <sz_t TPrecision>
        struct trig_table
        {
        private:
            static constexpr sz_t count{TPrecision};
            static constexpr float ratio{TPrecision / tau};
            std::array<float, count> arr;

        public:
            template <typename TF>
            inline trig_table(TF&& mFn) noexcept
            {
                for(auto i(0u); i < count; ++i) arr[i] = mFn(i / ratio);
            }

            inline auto get(float mX) const noexcept
            {
                auto idx(static_cast<sz_t>(mX * ratio));
                assert(idx < count);

                return arr[idx];
            }
        };
    }
}