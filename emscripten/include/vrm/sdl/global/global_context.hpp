// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/resource.hpp>
#include <vrm/sdl/elements.hpp>
#include <vrm/sdl/context.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        std::function<void()> global_context_fn;
        void run_global_context_loop() noexcept { global_context_fn(); }
    }

    template <typename TSettings, typename... Ts>
    auto make_global_context(Ts && ... xs)
    {
        auto uptr(std::make_unique<impl::context<TSettings>>(FWD(xs)...));

        auto ptr(uptr.get());

        impl::global_context_fn = [ptr]
        {
            get_global_event_manager().run();
            ptr->run();
        };

        return uptr;
    }

    void __attribute__((noreturn)) run_global_context() noexcept
    {
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(impl::run_global_context_loop, 0, true);
#else
        while(true)
        {
            impl::run_global_context_loop();
        }
#endif
    }

    void __attribute__((noreturn)) stop_global_context() noexcept
    {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#else
        std::terminate();
#endif
    }
}
VRM_SDL_NAMESPACE_END
