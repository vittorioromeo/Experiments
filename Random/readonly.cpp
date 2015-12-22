#include <type_traits>
#include <chrono>
#include <iostream>

using hrc = std::chrono::high_resolution_clock;

double copy_acc = 0;
double cref_acc = 0;
double roly_acc = 0;

template <typename TF>
inline void bench(const char* title, double& acc, TF&& f)
{
    auto tp_begin(hrc::now());
    f();

    auto elapsed(hrc::now() - tp_begin);
    auto as_ns(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed));
    auto cnt(as_ns.count());

    // std::cout << "(" << title << "): " << cnt << "\n";
    acc += cnt;
}

template <typename T>
using readonly = std::conditional_t // .
    <                               // .
        std::is_trivially_copyable<T>{},
        std::conditional_t                // .
        <                                 // .
            (sizeof(T) <= sizeof(void*)), // .
            const T,                      // .
            const T&                      // .
            >,
        const T&>;

template <typename T>
__attribute_noinline__ auto triple_always_copy(T x) noexcept
{
    return x;
}

template <typename T>
__attribute_noinline__ auto triple_always_const_ref(const T& x) noexcept
{
    return x;
}

template <typename T>
__attribute_noinline__ auto triple_readonly(readonly<T> x) noexcept
{
    return x;
}

using cheap0 = int;
using cheap1 = float;
using cheap2 = double;

namespace impl
{
    template <typename T>
    struct expensive
    {
        T x, y, z, w;
    };
}

using expensive0 = impl::expensive<int>;
using expensive1 = impl::expensive<float>;
using expensive2 = impl::expensive<double>;

constexpr std::size_t times(10000000);



template <typename T>
inline void my_bench(const char* title) noexcept
{
    // std::cout << title << "\n";

    bench("copy", copy_acc, []
        {
            for(std::size_t i = 0; i < times; ++i) triple_always_copy<T>(T{});
        });

    bench("cref", cref_acc, []
        {
            for(std::size_t i = 0; i < times; ++i)
                triple_always_const_ref<T>(T{});
        });

    bench("roly", roly_acc, []
        {
            for(std::size_t i = 0; i < times; ++i) triple_readonly<T>(T{});
        });
}

#define DO_BENCH(type) my_bench<type>(#type);

int main()
{
    for(std::size_t i = 0; i < 1; ++i)
    {
        DO_BENCH(cheap0)
        DO_BENCH(cheap1)
        DO_BENCH(cheap2)

        DO_BENCH(expensive0)
        DO_BENCH(expensive1)
        DO_BENCH(expensive2)
    }

    std::cout << "copy_acc: " << copy_acc << "\n";
    std::cout << "cref_acc: " << cref_acc << "\n";
    std::cout << "roly_acc: " << roly_acc << "\n";

    if(copy_acc < cref_acc)
    {
        if(roly_acc < copy_acc)
            std::cout << "roly wins\n";
        else
            std::cout << "copy wins\n";
    }
    else
    {
        if(roly_acc < cref_acc)
            std::cout << "roly wins\n";
        else
            std::cout << "cref wins\n";
    }

    return 0;
}