// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/check.hpp>
#include <vrm/sdl/gl/shader.hpp>
#include <vrm/sdl/gl/attribute.hpp>
#include <vrm/sdl/gl/uniform.hpp>

VRM_SDL_NAMESPACE
{
    class program
    {
    private:
        impl::unique_program id;

        bool in_use() const noexcept
        {
            GLint result;
            VRM_SDL_GLCHECK(glGetIntegerv(GL_CURRENT_PROGRAM, &result));
            return result == *id;
        }

    public:
        program() = default;

        program(impl::unique_program&& mResource) noexcept
            : id{std::move(mResource)}
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

        auto nth_attribute(GLuint location) const noexcept
        {
            assert(location != GL_INVALID_OPERATION);
            return sdl::attribute{location};
        }

        auto nth_uniform(GLuint location) const noexcept
        {
            assert(location != GL_INVALID_OPERATION);
            return sdl::uniform{location};
        }

        auto attribute(const char* name) const noexcept
        {
            GLuint location;
            VRM_SDL_GLCHECK(location = glGetAttribLocation(*id, name));

            return nth_attribute(location);
        }

        auto uniform(const char* name) const noexcept
        {
            GLuint location;
            VRM_SDL_GLCHECK(location = glGetUniformLocation(*id, name));

            return nth_uniform(location);
        }
    };

    template <typename... TShaders>
    auto make_program(TShaders && ... mShaders) noexcept
    {
        GLuint id;
        VRM_SDL_GLCHECK(id = glCreateProgram());

        impl::unique_program res{id};
        program p{std::move(res)};

        p.attach_and_link(FWD(mShaders)...);

        return p;
    }
}
VRM_SDL_NAMESPACE_END