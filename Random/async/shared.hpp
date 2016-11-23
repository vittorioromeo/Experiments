#pragma once

#include <atomic>
#include <functional>
#include <iostream>

#include <tuple>
#include <experimental/tuple>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <mingw.thread.h>
#include <mingw.mutex.h>
#include <mingw.condition_variable.h>

#include <ecst/thread_pool.hpp>
#include <ecst/utils.hpp>

namespace ll
{
      struct pool
    {
        template <typename TF>
        void post(TF&& f)
        {
            f();
        }
    };


    inline void sleep_ms(int ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    template <typename T>
    inline void print_sleep_ms(int ms, const T& x)
    {
        std::cout << x << std::endl;
        sleep_ms(ms);
    }
}
