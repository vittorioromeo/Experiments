// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
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
            attribute(GLuint location) noexcept : _location{location} {}

            void enable() { glEnableVertexAttribArray(_location); }

            void disable() { glDisableVertexAttribArray(_location); }

            void vertex_attrib_pointer(sz_t n_components, GLenum type,
                bool normalized = true, sz_t stride = 0,
                const GLvoid* first_element = nullptr)
            {
                assert(n_components > 0 && n_components < 5);

                glVertexAttribPointer(_location, n_components, type, normalized,
                    stride, first_element);
            }
        };

        class program
        {
        private:
            impl::unique_program id;

        public:
            program(impl::unique_program&& mResource) : id{std::move(mResource)}
            {
            }

            operator GLint() const noexcept { return *id; }

            template <typename... TShaders>
            inline void attach_and_link(TShaders&&... mShaders) noexcept
            {
                for_args(
                    [this](auto&& s)
                    {
                        glAttachShader(*id, FWD(s));
                    },
                    FWD(mShaders)...);

                glLinkProgram(*id);

                for_args(
                    [this](auto&& s)
                    {
                        glDetachShader(*id, FWD(s));
                    },
                    FWD(mShaders)...);
            }

            void use() noexcept { glUseProgram(*id); }

            auto get_attribute(const std::string& a_name) const noexcept
            {
                auto location(glGetAttribLocation(*id, a_name.c_str()));
                assert(location != GL_INVALID_OPERATION);

                return attribute{static_cast<GLuint>(location)};
            }
        };

        template <typename... TShaders>
        auto make_program(TShaders&&... mShaders) noexcept
        {
            auto id(glCreateProgram());

            impl::unique_program res{id};
            program p{std::move(res)};

            p.attach_and_link(FWD(mShaders)...);

            return p;
        }
    }
}