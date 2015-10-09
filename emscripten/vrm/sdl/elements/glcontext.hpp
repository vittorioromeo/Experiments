// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        class glcontext : public impl::unique_glcontext
        {
        private:
            using base_type = impl::unique_glcontext;
            SDL_GLContext _glcontext;

        public:
            glcontext(window& w) noexcept : _glcontext{SDL_GL_CreateContext(w)},
                                            base_type{&_glcontext}
            {
                SDL_GL_SetSwapInterval(0);
            }
        };
    }
}