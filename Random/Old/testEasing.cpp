// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// Thanks to sim642 -
// http://www.facepunch.com/showthread.php?t=1239244&p=39204496&viewfull=1#post39204496

#ifndef SSVU_EASING
#define SSVU_EASING

namespace ssvu
{
    namespace Easing
    {
        template <typename T>
        struct Linear
        {
            inline static constexpr T in(
                const T& t, const T& b, const T& c, const T& d) noexcept
            {
                return c * t / d + b;
            }
            inline static constexpr T out(
                const T& t, const T& b, const T& c, const T& d) noexcept
            {
                return in(t, b, c, d);
            }
            inline static constexpr T inOut(
                const T& t, const T& b, const T& c, const T& d) noexcept
            {
                return in(t, b, c, d);
            }
        };

        template <typename T>
        struct Sine
        {
            inline static constexpr T in(
                const T& t, const T& b, const T& c, const T& d) noexcept
            {
                return -c * std::cos(t / d * (ssvu::pi / T(2))) + c + b;
            }
            inline static constexpr T out(
                const T& t, const T& b, const T& c, const T& d) noexcept
            {
                return c * std::sin(t / d * (ssvu::pi / T(2))) + b;
            }
            inline static constexpr T inOut(
                const T& t, const T& b, const T& c, const T& d) noexcept
            {
                return -c / T(2) * (std::cos(ssvu::pi * t / d) - T(1)) + b;
            }
        };
    }
}

#endif
