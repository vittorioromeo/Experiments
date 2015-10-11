// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/math.hpp>
#include <vrm/sdl/dependencies.hpp>
#include <vrm/sdl/common.hpp>

namespace vrm
{
    namespace sdl
    {
        class glcontext
        {
        private:
            SDL_GLContext _glcontext;

        public:
            glcontext(window& w) noexcept
            {
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
                SDL_GL_SetSwapInterval(0);
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

                _glcontext = SDL_GL_CreateContext(w);
            }

            auto context() const noexcept { return _glcontext; }
        };
    }
}