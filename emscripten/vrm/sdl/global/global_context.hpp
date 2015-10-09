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
            std::unique_ptr<context> global_context;
            void run_global_context_loop() { global_context->run(); }
        }

        template <typename... Ts>
        auto& make_global_context(Ts&&... xs)
        {
            assert(impl::global_context == nullptr);
            impl::global_context = std::make_unique<context>(FWD(xs)...);
            return *impl::global_context;
        }

        void run_global_context()
        {
            emscripten_set_main_loop(impl::run_global_context_loop, 0, true);
        }
    }
}