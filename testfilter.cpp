#include <iostream>
#include <type_traits>
#include <utility>
#include <experimental/tuple>

namespace impl
{
    template <typename T>
    struct tw
    {
        using type = T;
    };
};

template <typename T>
static constexpr impl::tw<T> tw{};

namespace impl
{
    template <typename TF, typename... Ts>
    constexpr void foreach(TF&& f, Ts && ... xs)
    {
        (f(xs), ...);
    }
}

template <typename TF, typename TC>
constexpr void foreach(TF&& f, TC && c)
{
    std::experimental::apply(
        [&f](auto&&... xs)
        {
            impl::foreach(f, xs...);
        },
        c);
}

template <typename... Ts>
constexpr auto mk_c(Ts&&... xs)
{
    return std::make_tuple(xs...);
}

template <typename... Ts>
constexpr auto cat(Ts&&... xs)
{
    return std::tuple_cat(xs...);
}

template <int TI>
static constexpr std::integral_constant<int, TI> int_v{};

namespace impl
{
    template <typename TF>
    constexpr auto filter(TF&&)
    {
        return mk_c();
    }

    template <typename TF, typename T, typename... Ts>
    constexpr auto filter(TF&& f, T&& x, Ts&&... xs)
    {
        if(f(x))
        {
            return cat(mk_c(x), filter(f, xs...));
        }
        else
        {
            return cat(filter(f, xs...));
        }
    }
}

template <typename TF, typename TC>
constexpr auto filter(TF&& f, TC&& c)
{
    return std::experimental::apply(
        [&f](auto&&... xs)
        {
            return impl::filter(f, xs...);
        },
        c);
}

int main()
{
    // Print run-time values
    foreach(
        [](auto&& x)
        {
            std::cout << x << ", ";
        },
        mk_c(1, 2, 3, 4, 5))
        ;

    // Print compile-time values
    foreach(
        [](auto x)
        {
            std::cout << decltype(x){} << ", ";
        },
        mk_c(int_v<1>, int_v<2>, int_v<3>, int_v<4>, int_v<5>))
        ;

    // Filter run-time values
    {
        auto fc = filter(
            [](auto x)
            {
                return x % 2 == 0;
            },
            mk_c(1, 2, 3, 4, 5));
    }
}