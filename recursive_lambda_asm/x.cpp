#include <boost/hana.hpp>
#include <functional>

// #define USE_YCOMBINATOR

int main()
{
#ifndef USE_YCOMBINATOR
    std::function<int(int)> f = [&](int x)
    {
        if(x == 0)
            return 1;
        return x * f(x - 1);
    };
#else
    auto f = boost::hana::fix([](auto self, int x) -> int
        {
            if(x == 0)
                return 1;
            return x * self(x - 1);
        });
#endif

    volatile auto res = f(6);
}