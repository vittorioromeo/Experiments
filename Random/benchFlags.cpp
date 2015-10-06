#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

volatile int vint{0};
volatile void vvoid(bool x) {}
constexpr ssvu::SizeT ntimes{10000};

template <ssvu::SizeT TS>
inline void runBench() noexcept
{
    ssvu::lo() << "\n\n\n\n" << ssvu::hr();
    ssvu::lo() << "\n\nBenchmarking std::bitset<" << TS << ">\n";
    {
        SSVU_BENCHMARK_LOG_SCOPE_EXIT("total");
        std::bitset<TS> flags;

        {
            SSVU_BENCHMARK_LOG_SCOPE_EXIT("set");
            for(auto i(0u); i < ntimes; ++i)
            {
                for(auto s(0u); s < TS; ++s)
                {
                    auto x = s % 2;
                    flags[s] = x;
                    vint += (int)x;
                }
            }
        }
        {
            {
                SSVU_BENCHMARK_LOG_SCOPE_EXIT("get");
                for(auto i(0u); i < ntimes; ++i)
                    for(auto s(0u); s < TS; ++s)
                    {
                        vint += (int)flags[s];
                        vvoid(flags[s]);
                    }
            }
        }
    }

    ssvu::lo() << ssvu::hr();
    ssvu::lo() << "\nBenchmarking std::array<bool, " << TS << ">\n";
    {
        SSVU_BENCHMARK_LOG_SCOPE_EXIT("total");
        std::array<bool, TS> flags;

        {
            SSVU_BENCHMARK_LOG_SCOPE_EXIT("set");
            for(auto i(0u); i < ntimes; ++i)
            {
                for(auto s(0u); s < TS; ++s)
                {
                    auto x = s % 2;
                    flags[s] = x;
                    vint += (int)x;
                }
            }
        }
        {
            {
                SSVU_BENCHMARK_LOG_SCOPE_EXIT("get");
                for(auto i(0u); i < ntimes; ++i)
                    for(auto s(0u); s < TS; ++s)
                    {
                        vint += (int)flags[s];
                        vvoid(flags[s]);
                    }
            }
        }
    }


    ssvu::lo() << ssvu::hr();
    ssvu::lo() << "\nBenchmarking std::vector<bool, " << TS << ">\n";
    {
        SSVU_BENCHMARK_LOG_SCOPE_EXIT("total");
        std::vector<bool> flags;
        flags.resize(TS);

        {
            SSVU_BENCHMARK_LOG_SCOPE_EXIT("set");
            for(auto i(0u); i < ntimes; ++i)
            {
                for(auto s(0u); s < TS; ++s)
                {
                    auto x = s % 2;
                    flags[s] = x;
                    vint += (int)x;
                }
            }
        }
        {
            {
                SSVU_BENCHMARK_LOG_SCOPE_EXIT("get");
                for(auto i(0u); i < ntimes; ++i)
                    for(auto s(0u); s < TS; ++s)
                    {
                        vint += (int)flags[s];
                        vvoid(flags[s]);
                    }
            }
        }
    }
}

int main()
{
    runBench<32>();
    runBench<64>();
    runBench<128>();
    runBench<1000>();
}