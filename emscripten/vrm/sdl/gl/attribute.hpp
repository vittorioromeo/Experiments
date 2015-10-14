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

            auto& enable(int size = 1) noexcept
            {
                for(auto i(0); i < size; ++i)
                {
                    VRM_SDL_GLCHECK(glEnableVertexAttribArray(_location + i));
                }
                return *this;
            }

            auto& disable() noexcept
            {
                VRM_SDL_GLCHECK(glDisableVertexAttribArray(_location));
                return *this;
            }


            auto& vertex_attrib_pointer(sz_t n_components, GLenum type,
                bool normalized = true, sz_t stride = 0,
                const GLvoid* first_element = nullptr,
                sz_t layout_offset = 0) noexcept
            {
                assert(n_components > 0 && n_components < 5);

                /*
                std::cout << "glVertexAttribPointer("
                          << _location + layout_offset << ", " << n_components
                          << ", " << (int)type << ", " << normalized << ", "
                          << stride << ", " << (long)first_element << ");\n";
                          */

                VRM_SDL_GLCHECK(glVertexAttribPointer(_location + layout_offset,
                    n_components, type, normalized, stride, first_element));

                return *this;
            }

            template <typename T>
            auto& vertex_attrib_pointer_in(sz_t n_components, GLenum type,
                bool normalized = true, sz_t offset = 0,
                sz_t layout_offset = 0) noexcept
            {
                static_assert(std::is_standard_layout<T>{}, "");
        
                return vertex_attrib_pointer(n_components, type, normalized,
                    sizeof(T), reinterpret_cast<const void*>(offset), layout_offset);
            }

            auto& vertex_attrib_pointer_float(sz_t n_components,
                bool normalized = true,
                const GLvoid* first_element = nullptr) noexcept
            {
                // return *this;
                return vertex_attrib_pointer(n_components, GL_FLOAT, normalized,
                    sizeof(float) * n_components, first_element);
            }

            auto location() const noexcept { return _location; }
        };
    }
}