// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#ifndef NDEBUG

#define VRM_SDL_GLCHECK(...)                                                \
    do                                                                      \
    {                                                                       \
        __VA_ARGS__;                                                        \
        ::vrm::sdl::impl::gl_check_error(__FILE__, __LINE__, #__VA_ARGS__); \
    } while(false)

#else

#define VRM_SDL_GLCHECK(...) \
    do                       \
    {                        \
        __VA_ARGS__;         \
    } while(false)

#endif