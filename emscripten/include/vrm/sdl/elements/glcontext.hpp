// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/math.hpp>
#include <vrm/sdl/dependencies.hpp>
#include <vrm/sdl/common.hpp>

VRM_SDL_NAMESPACE
{
    class glcontext
    {
    private:
        SDL_GLContext _glcontext;

    public:
        glcontext(sdl_window& w) noexcept
        {
            _glcontext = SDL_GL_CreateContext(w);

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            SDL_GL_SetSwapInterval(0);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_SCISSOR_TEST);
        }

        auto context() const noexcept { return _glcontext; }
    };

    namespace impl
    {
        struct glcontext_deleter
        {
            void operator()(glcontext& p) noexcept
            {
                SDL_GL_DeleteContext(p.context());
            }
        };

        using unique_glcontext = unique_resource<glcontext, glcontext_deleter>;
    }
}
VRM_SDL_NAMESPACE_END