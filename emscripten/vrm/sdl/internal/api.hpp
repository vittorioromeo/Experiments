// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#define VRM_SDL_NAMESPACE \
    namespace vrm         \
    {                     \
    namespace sdl

#define VRM_SDL_NAMESPACE_END }

#define VRM_SDL_API_HEADERONLY 1

#if defined(VRM_SDL_API_HEADERONLY)
#define VRM_SDL_API_INLINE inline
#else
// TODO:
#define VRM_SDL_API_INLINE inline
#endif