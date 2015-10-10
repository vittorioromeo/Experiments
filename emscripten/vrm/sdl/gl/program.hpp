// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/shader.hpp>
#include <vrm/sdl/gl/attribute.hpp>

namespace vrm
{
    namespace sdl
    {
        class program
        {
        private:
            impl::unique_program id;

        public:
            program() = default;

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
                        VRM_SDL_GLCHECK(glAttachShader(*id, FWD(s)));
                    },
                    FWD(mShaders)...);

                VRM_SDL_GLCHECK(glLinkProgram(*id));

                for_args(
                    [this](auto&& s)
                    {
                        VRM_SDL_GLCHECK(glDetachShader(*id, FWD(s)));
                    },
                    FWD(mShaders)...);
            }

            void use() noexcept { glUseProgram(*id); }

            auto get_attribute(const std::string& a_name) const noexcept
            {
                GLuint location;

                VRM_SDL_GLCHECK(
                    location = glGetAttribLocation(*id, a_name.c_str()););

                assert(location != GL_INVALID_OPERATION);

                return attribute{location};
            }

            auto get_uniform(const std::string& a_name) const noexcept
            {
                GLuint location;

                VRM_SDL_GLCHECK(
                    location = glGetUniformLocation(*id, a_name.c_str()););

                assert(location != GL_INVALID_OPERATION);

                return uniform{location};
            }
        };

        template <typename... TShaders>
        auto make_program(TShaders&&... mShaders) noexcept
        {
            GLuint id;
            VRM_SDL_GLCHECK(id = glCreateProgram(););

            impl::unique_program res{id};
            program p{std::move(res)};

            p.attach_and_link(FWD(mShaders)...);

            return p;
        }
    }
}