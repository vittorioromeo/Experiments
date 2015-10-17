// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <chrono>
#include <vrm/sdl/math.hpp>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/utils.hpp>
#include <vrm/sdl/resource.hpp>
#include <vrm/sdl/elements.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {


        struct renderer_deleter
        {
            void operator()(renderer& p) noexcept
            {
                SDL_DestroyRenderer(p.ptr());
            }
        };

        struct surface_deleter
        {
            void operator()(surface& p) noexcept { SDL_FreeSurface(p.ptr()); }
        };



        struct ttffont_deleter
        {
            void operator()(ttffont& p) noexcept { TTF_CloseFont(p.ptr()); }
        };

        

        


        using unique_renderer = unique_resource<renderer, renderer_deleter>;

        using unique_surface = unique_resource<surface, surface_deleter>;



        using unique_ttffont = unique_resource<ttffont, ttffont_deleter>;



        
    }
}
VRM_SDL_NAMESPACE_END