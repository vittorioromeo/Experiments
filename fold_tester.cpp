#include <iostream>

template <typename F>
struct tester : F
{
    constexpr tester(F&& f) : F{std::move(f)}
    {
    }

    template <typename X>
    constexpr bool is(X&& x) &&
    {
        return (*this)([&x](auto&& y) { return x == y; });
    }
};

template <typename... Xs>
constexpr auto any_of(Xs&&... xs)
{
    return tester{[&](auto&& f) { return (f(xs) || ...); }};
}


int main()
{
    if constexpr(any_of(1, 2, 3).is(4))
    {
        std::cout << 1 << std::endl;
    }
    else
    {
        std::cout << 0 << std::endl;
    }

    if constexpr(any_of(1, 2, 3).is(2))
    {
        std::cout << 1 << std::endl;
    }
    else
    {
        std::cout << 0 << std::endl;
    }
}
