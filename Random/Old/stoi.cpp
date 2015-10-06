#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

constexpr int times{40000};

inline auto& getSS() noexcept
{
    thread_local std::ostringstream oss;
    return oss;
}
template <typename T>
inline auto ss(T x)
{
    getSS().str("");
    getSS() << x;
    return getSS().str();
}

template <typename T>
inline auto getri() noexcept
{
    return ssvu::getRndI<T, T>(
        ssvu::NumLimits<T>::min(), ssvu::NumLimits<T>::max());
}

template <typename T>
inline void check(T v, const std::string& mX)
{
    // if(mX != std::to_string(v) || mX != ss(v))
    {
        // ssvu::lo() << mX << ", " << std::to_string(v) << ", " << ss(v) <<
        // "\n";
    }
}

namespace ssvu
{
    namespace Impl
    {
        namespace NumToStr
        {
            template <SizeT TS>
            inline auto& getConvBuffer() noexcept
            {
                thread_local std::array<char, TS> result;
                return result;
            }

            template <typename T, class TEnable = void>
            struct Conv;

            constexpr char digits[201]{
                "00010203040506070809"
                "10111213141516171819"
                "20212223242526272829"
                "30313233343536373839"
                "40414243444546474849"
                "50515253545556575859"
                "60616263646566676869"
                "70717273747576777879"
                "80818283848586878889"
                "90919293949596979899"};

            template <typename T>
            struct Conv<T, EnableIf<!isUnsigned<T>()>>
            {
                static constexpr SizeT bufferSize{NumLimits<T>::digits10};

                inline static auto toStr(T val) noexcept
                {
                    auto& buf(getConvBuffer<bufferSize>());
                    char* it{&buf[bufferSize - 2]};

                    if(val >= 0)
                    {
                        auto div(val / 100);

                        while(div)
                        {
                            std::memcpy(it, &digits[2 * (val - div * 100)], 2);
                            val = div;
                            it -= 2;
                            div = val / 100;
                        }

                        std::memcpy(it, &digits[2 * val], 2);
                        if(val < 10) it++;
                    }
                    else
                    {
                        auto div(val / 100);

                        while(div)
                        {
                            std::memcpy(it, &digits[-2 * (val - div * 100)], 2);
                            val = div;
                            it -= 2;
                            div = val / 100;
                        }

                        std::memcpy(it, &digits[-2 * val], 2);
                        if(val <= -10) it--;
                        *it = '-';
                    }

                    return std::string(it, &buf[bufferSize] - it);
                }
            };

            template <typename T>
            struct Conv<T, EnableIf<isUnsigned<T>()>>
            {
                static constexpr SizeT bufferSize{NumLimits<T>::digits10};

                inline static auto toStr(T val) noexcept
                {
                    auto& buf(getConvBuffer<bufferSize>());
                    char* it{&buf[bufferSize - 2]};

                    auto div(val / 100);

                    while(div)
                    {
                        std::memcpy(it, &digits[2 * (val - div * 100)], 2);
                        val = div;
                        it -= 2;
                        div = val / 100;
                    }

                    std::memcpy(it, &digits[2 * val], 2);
                    if(val < 10) it++;

                    return std::string(it, &buf[bufferSize] - it);
                }
            };



            template <typename T>
            inline auto toStr(const T& mX) noexcept
            {
                return Conv<T>::toStr(mX);
            }
        }
    }
}



template <typename T>
inline std::string veenaivei(T x) noexcept
{
    return ssvu::Impl::NumToStr::toStr(x);
}


template <typename T>
inline void runTest() noexcept
{
    {
        SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("stringstream");

        auto min(ssvu::NumLimits<T>::min());
        auto max(ssvu::NumLimits<T>::max());
        check(min, ss(min));
        check(max, ss(max));

        for(int i = 0; i < times; ++i)
        {
            auto t(getri<T>());


            check(t, ss(t));
        }
    }

    {
        SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("to_string");

        auto min(ssvu::NumLimits<T>::min());
        auto max(ssvu::NumLimits<T>::max());
        check(min, std::to_string(min));
        check(max, std::to_string(max));

        for(int i = 0; i < times; ++i)
        {
            auto t(getri<T>());


            check(t, std::to_string(t));
        }
    }

    {
        SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("vee");

        auto min(ssvu::NumLimits<T>::min());
        auto max(ssvu::NumLimits<T>::max());
        check(min, veenaivei(min));
        check(max, veenaivei(max));

        for(int i = 0; i < times; ++i)
        {
            auto t(getri<T>());


            check(t, veenaivei(t));
        }
    }
}



int main()
{


    SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("stringstream");
    SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("to_string");
    SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("vee");

    runTest<int>();
    runTest<unsigned int>();


    runTest<short>();
    runTest<long long>();
    runTest<unsigned short>();

    runTest<long>();
    runTest<unsigned long>();
    runTest<unsigned long long>();



    return 0;
}
