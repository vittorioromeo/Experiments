#include <atomic>
#include <chrono>
#include <cstdio>
#include <experimental/tuple>
#include <functional>
#include <thread>
#include <tuple>

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

    auto cont = [&](const auto& x, auto& ctr) {
        ll::print_sleep_ms(250, x);
        if(--ctr == 0)
        {
            p.post([&] { ll::print_sleep_ms(250, "D"); });
        }
    };

    ([&] {
        ll::print_sleep_ms(250, "A");
        p.post([&] {
            ll::print_sleep_ms(250, "B");
            p.post([&] {
                std::atomic<int> ctr{3};
                cont("C0", ctr);
                cont("C1", ctr);
                cont("C2", ctr);
            });
        });
    })();

    ll::sleep_ms(1000);
}
