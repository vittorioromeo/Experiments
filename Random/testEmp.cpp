#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

volatile int state{0};

struct TypeA
{
    volatile char x[16];
    volatile void ok() {}
};

template <typename T, typename TC>
inline T& tNew(TC& mContainer)
{
    mContainer.emplace_back(ssvu::mkUPtr<TypeA>());
    return *mContainer[mContainer.size() - 1].get();
}

template <typename T, typename TC>
inline T& tNew2(TC& mContainer)
{
    mContainer.emplace_back(ssvu::mkUPtr<TypeA>());
    return *mContainer.back();
}


int main()
{
    using namespace std;
    using namespace ssvu;

    Benchmark::groupReset("current");
    Benchmark::groupReset("new");
    Benchmark::groupReset("new2");

    for(int n = 0; n < 100; ++n)
    {
        {
            vector<UPtr<TypeA>> v;
            constexpr int ff = 1000000;
            v.reserve(ff);

            {
                SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("current");
                for(int f = 0; f < ff; ++f)
                {
                    auto& k = getEmplaceUPtr<TypeA>(v);
                    k.ok();
                }
            }
        }

        {
            vector<UPtr<TypeA>> v;
            constexpr int ff = 1000000;
            v.reserve(ff);

            {
                SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("new");
                for(int f = 0; f < ff; ++f)
                {
                    auto& k = tNew<TypeA>(v);
                    k.ok();
                }
            }
        }

        {
            vector<UPtr<TypeA>> v;
            constexpr int ff = 1000000;
            v.reserve(ff);

            {
                SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("new2");
                for(int f = 0; f < ff; ++f)
                {
                    auto& k = tNew<TypeA>(v);
                    k.ok();
                }
            }
        }
    }

    Benchmark::groupEndLo("current");
    Benchmark::groupEndLo("new");
    Benchmark::groupEndLo("new2");

    return 0;
}

// SSVU_BENCHMARK_LOG_SCOPE_EXIT
// SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT