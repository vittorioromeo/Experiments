// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
        namespace impl
        {
            context* global_context;
            void run_global_context_loop() { global_context->run(); }
        }

        template <typename... Ts>
        auto make_global_context(Ts&&... xs)
        {
            assert(impl::global_context == nullptr);
            auto uptr(std::make_unique<context>(FWD(xs)...));
            impl::global_context = uptr.get();
            return uptr;
        }

        void run_global_context() noexcept
        {
            emscripten_set_main_loop(impl::run_global_context_loop, 0, true);
        }

        void stop_global_context() noexcept
        {
            emscripten_cancel_main_loop();
        }
    }
}