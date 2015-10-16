// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/math.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/check.hpp>
#include <vrm/sdl/gl/shader.hpp>

VRM_SDL_NAMESPACE
{
    class uniform
    {
    private:
        GLuint _location;

    public:
        uniform() = default;
        uniform(GLuint location) noexcept : _location{location} {}

    private:
        void matrix2fv(
            sz_t count, bool transpose, const GLfloat* value) noexcept
        {
            VRM_SDL_GLCHECK(
                glUniformMatrix2fv(_location, count, transpose, value));
        }

        void matrix3fv(
            sz_t count, bool transpose, const GLfloat* value) noexcept
        {
            VRM_SDL_GLCHECK(
                glUniformMatrix3fv(_location, count, transpose, value));
        }

        void matrix4fv(
            sz_t count, bool transpose, const GLfloat* value) noexcept
        {
            VRM_SDL_GLCHECK(
                glUniformMatrix4fv(_location, count, transpose, value));
        }

    public:
        void mat2(
            const glm::mat2& m, sz_t count = 1, bool transpose = false) noexcept
        {
            matrix2fv(count, transpose, glm::value_ptr(m));
        }

        void mat3(
            const glm::mat3& m, sz_t count = 1, bool transpose = false) noexcept
        {
            matrix3fv(count, transpose, glm::value_ptr(m));
        }

        void mat4(
            const glm::mat4& m, sz_t count = 1, bool transpose = false) noexcept
        {
            matrix4fv(count, transpose, glm::value_ptr(m));
        }

        void integer(int x) noexcept
        {
            VRM_SDL_GLCHECK(glUniform1i(_location, x));
        }

        void floating(float x) noexcept
        {
            VRM_SDL_GLCHECK(glUniform1f(_location, x));
        }

        void vec2(const vec2f& x) noexcept
        {
            VRM_SDL_GLCHECK(glUniform2f(_location, x.x, x.y));
        }

        void vec3(const vec3f& x) noexcept
        {
            VRM_SDL_GLCHECK(glUniform3f(_location, x.x, x.y, x.z));
        }

        void vec4(const vec4f& x) noexcept
        {
            VRM_SDL_GLCHECK(glUniform4f(_location, x.x, x.y, x.z, x.w));
        }

        auto location() const noexcept { return _location; }
    };
}
VRM_SDL_NAMESPACE_END