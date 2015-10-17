// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/resource.hpp>
#include <vrm/sdl/elements.hpp>
#include <vrm/sdl/gl.hpp>
#include <vrm/sdl/window.hpp>

#include <vrm/sdl/camera/camera_2d.hpp>

VRM_SDL_NAMESPACE
{
    class camera_2d
    {
    private:
        window& _window;
        mat4f _projection;
        vec2f _offset;
        float _scale{1.f};
        float _radians{0.f};

        void refresh_projection()
        {
            _projection = impl::make_2d_projection(
                _window.original_size().x, _window.original_size().y);
        }

        auto origin() const noexcept { return _window.original_size() / 2.f; }

    public:
        auto position() const noexcept { return _offset + origin(); }

    private:
        void translate_to_origin(mat4f& view, float direction) const noexcept
        {
            view =
                glm::translate(view, vec3f(position().xy() * direction, 0.f));
        }

    public:
        camera_2d(window& window) noexcept : _window{window}
        {
            refresh_projection();
        }

        const auto& projection() const noexcept { return _projection; }

        auto& zoom(float factor) noexcept
        {
            _scale += factor;
            return *this;
        }

        auto& move_towards_angle(float radians, float speed)
        {
            radians += _radians;

            _offset +=
                vec2f(speed * std::cos(radians), speed * std::sin(radians));

            return *this;
        }

        auto& move_towards_point(const vec2f& point, float speed)
        {
            auto direction((point - position()));
            auto angle(std::atan2(direction.y, direction.x));
            return move_towards_angle(angle - _radians, speed);
        }


        auto& move(vec2f offset) noexcept
        {
            auto speed(glm::length(offset));

            offset = glm::normalize(offset);
            auto direction(std::atan2(offset.y, offset.x));

            return move_towards_angle(direction, speed);
        }

        auto& offset() noexcept { return _offset; }
        const auto& offset() const noexcept { return _offset; }

        auto& angle() noexcept { return _radians; }
        const auto& angle() const noexcept { return _radians; }

        auto& rotate(float radians) noexcept
        {
            _radians += radians;
            return *this;
        }

        auto view() const noexcept
        {
            mat4f result;

            result = glm::translate(result, vec3f{-_offset, 0.f});

            translate_to_origin(result, 1.f);
            {
                // std::cout << _window.scaling_factor() << "\n";
                // auto sc(_scale * _window.scaling_factor());

                auto sc(_scale);

                result = glm::scale(result, vec3f(sc, sc, 1.0f));

                result = glm::rotate(result, -_radians, vec3f(0.f, 0.f, 1.f));
            }
            translate_to_origin(result, -1.f);

            return result;
        }

        auto projection_view() const noexcept { return projection() * view(); }
    };
}
VRM_SDL_NAMESPACE_END