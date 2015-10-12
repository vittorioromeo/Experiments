// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/check.hpp>
#include <vrm/sdl/gl/shader.hpp>

namespace vrm
{
    namespace sdl
    {
        class attribute
        {
        private:
            GLuint _location;

        public:
            attribute() = default;
            attribute(GLuint location) noexcept : _location{location} {}

            auto& enable() noexcept
            {
                VRM_SDL_GLCHECK(glEnableVertexAttribArray(_location));
                return *this;
            }

            auto& disable() noexcept
            {
                VRM_SDL_GLCHECK(glDisableVertexAttribArray(_location));
                return *this;
            }

            auto& vertex_attrib_pointer(sz_t n_components, GLenum type,
                bool normalized = true, sz_t stride = 0,
                const GLvoid* first_element = nullptr) noexcept
            {
                assert(n_components > 0 && n_components < 5);

                VRM_SDL_GLCHECK(glVertexAttribPointer(_location, n_components,
                    type, normalized, stride, first_element));

                return *this;
            }

            auto& vertex_attrib_pointer_float(sz_t n_components,
                bool normalized = true,
                const GLvoid* first_element = nullptr) noexcept
            {
                return vertex_attrib_pointer(n_components, GL_FLOAT, normalized,
                    sizeof(float) * n_components, first_element);
            }

            auto location() const noexcept { return _location; }
        };

        class uniform
        {
        private:
            GLuint _location;

        public:
            uniform() = default;
            uniform(GLuint location) noexcept : _location{location} {}

            void matrix4fv(sz_t count, bool transpose, const GLfloat* value)
            {
                VRM_SDL_GLCHECK(
                    glUniformMatrix4fv(_location, count, transpose, value));
            }

            void matrix4fv(const glm::mat4& m) noexcept
            {
                matrix4fv(1, false, glm::value_ptr(m));
            }

            void integer(int x) noexcept
            {
                VRM_SDL_GLCHECK(glUniform1i(_location, x));
            }

            void floating(float x) noexcept
            {
                VRM_SDL_GLCHECK(glUniform1f(_location, x));
            }

            void vec4(const glm::vec4& x) noexcept
            {
                VRM_SDL_GLCHECK(glUniform4f(_location, x.x, x.y, x.z, x.w));
            }


            auto location() const noexcept { return _location; }
        };
    }
}