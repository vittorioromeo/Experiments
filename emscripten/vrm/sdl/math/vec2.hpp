// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ostream>

namespace vrm
{
    namespace sdl
    {
        template <typename T>
        class vec2
        {
        private:
            T _x, _y;

        public:
            vec2() noexcept : _x{T(0)}, _y{T(0)} {};
            vec2(T x, T y) noexcept : _x{x}, _y{y} {}

            template <typename TS>
            vec2(const vec2<TS>& v) noexcept : _x(v.x()), _y(v.y())
            {
            }

            template <typename TS>
            vec2& operator=(const vec2<TS>& v) noexcept
            {
                _x = v.x();
                _y = v.y();
                return *this;
            }

            vec2(const vec2&) = default;
            vec2& operator=(const vec2&) = default;

            vec2(vec2&&) = default;
            vec2& operator=(vec2&&) = default;

            auto& x() noexcept { return _x; }
            const auto& x() const noexcept { return _x; }

            auto& y() noexcept { return _y; }
            const auto& y() const noexcept { return _y; }

            template <typename TV>
            vec2& operator+=(const TV& v) noexcept
            {
                _x += v._x;
                _y += v._y;
                return *this;
            }

            template <typename TV>
            vec2& operator-=(const TV& v) noexcept
            {
                _x -= v._x;
                _y -= v._y;
                return *this;
            }

            template <typename TS>
            vec2& operator*=(TS s) noexcept
            {
                _x *= s;
                _y *= s;
                return *this;
            }

            template <typename TS>
            vec2& operator/=(TS s) noexcept
            {
                _x /= s;
                _y /= s;
                return *this;
            }
        };

        using vec2i = vec2<int>;
        using vec2u = vec2<unsigned int>;
        using vec2f = vec2<float>;

        template <typename T0, typename T1>
        auto make_vec2(T0 x, T1 y)
        {
            return vec2<std::common_type_t<T0, T1>>(x, y);
        }

        template <typename TV0, typename TV1>
        auto operator+(const TV0& v0, const TV1& v1)
        {
            return make_vec2(v0.x() + v1.x(), v0.y() + v1.y());
        }

        template <typename TV0, typename TV1>
        auto operator-(const TV0& v0, const TV1& v1)
        {
            return make_vec2(v0.x() - v1.x(), v0.y() - v1.y());
        }

        template <typename TV, typename TS>
        auto operator*(const TV& v, TS s)
        {
            return make_vec2(v.x() * s, v.y() * s);
        }

        template <typename TV, typename TS>
        auto operator/(const TV& v, TS s)
        {
            return make_vec2(v.x() / s, v.y() / s);
        }

        template <typename T>
        auto& operator<<(std::ostream& o, const vec2<T>& v)
        {
            o << "(" << v.x() << ", " << v.y() << ")";
            return o;
        }
    }
}