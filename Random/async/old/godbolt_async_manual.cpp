#include <atomic>
#include <chrono>
#include <cstdio>
#include <functional>
#include <thread>

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
        std::puts(x);
        sleep_ms(ms);
    }
}

int main()
{
    ll::pool p;

    ([&] {
        ll::print_sleep_ms(250, "A");
        p.post([&] {
            ll::print_sleep_ms(250, "B");
            p.post([&] { ll::print_sleep_ms(250, "C"); });
        });
    })();

    ll::sleep_ms(1000);
}
