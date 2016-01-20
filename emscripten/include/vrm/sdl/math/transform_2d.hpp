// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cassert>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/math/sin_cos_table.hpp>
#include <vrm/sdl/math/vec.hpp>
#include <vrm/sdl/math/mat.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        auto make_2d_projection(float width, float height)
        {
            return glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
        }

        VRM_SDL_ALWAYS_INLINE auto translation_matrix_2d(
            const vec2f& position) noexcept
        {
            return mat3f{
                // .
                1.f, 0.f, 0.f,              // .
                0.f, 1.f, 0.f,              // .
                position.x, position.y, 1.f // .
            };
        }

        VRM_SDL_ALWAYS_INLINE auto rotation_matrix_2d(float radians) noexcept
        {
            return mat3f{
                // .
                tbl_cos(radians), tbl_sin(radians), 0.f,  // .
                -tbl_sin(radians), tbl_cos(radians), 0.f, // .
                0.f, 0.f, 1.f                             // .
            };
        }

        VRM_SDL_ALWAYS_INLINE auto scaling_matrix_2d(const vec2f& size) noexcept
        {
            return mat3f{
                // .
                size.x, 0.f, 0.f, // .
                0.f, size.y, 0.f, // .
                0.f, 0.f, 1.f     // .
            };
        }

        VRM_SDL_ALWAYS_INLINE auto shearing_matrix_2d(
            const vec2f& shear) noexcept
        {
            mat3f shearing_x{
                // .
                1.f, 0.f, 0.f,      // .
                -shear.x, 1.f, 0.f, // .
                0.f, 0.f, 1.f       // .
            };

            mat3f shearing_y{
                // .
                1.f, shear.y, 0.f, // .
                0.f, 1.f, 0.f,     // .
                0.f, 0.f, 1.f      // .
            };

            return shearing_x * shearing_y;
        }

        VRM_SDL_ALWAYS_INLINE auto trasform_matrix_2d(const vec2f& position,
            const vec2f& origin, const vec2f& size, float radians,
            const vec2f& shear) noexcept
        {
            return                                            // .
                translation_matrix_2d(position) *             // .
                rotation_matrix_2d(radians) *                 // .
                translation_matrix_2d(origin - size * 0.5f) * // .
                scaling_matrix_2d(size) *                     // .
                shearing_matrix_2d(shear);
        }
    }
}
VRM_SDL_NAMESPACE_END