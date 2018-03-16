#include <iostream>
#include <type_traits>
#include <utility>
#include <tuple>

// TODO article:
// * iterate over types
// * iterate over values
// * iterate over range of values
// * expand integer sequence in place
// * expand tuple in place
// * expand typelist in place

template <typename... Ts, typename F>
constexpr void for_types(F&& f)
{
    (f.template operator()<Ts>(), ...);
}

template <auto... Ts, typename F>
constexpr void for_values(F&& f)
{
    (f.template operator()<Ts>(), ...);
}

template <auto B, auto E, typename F>
constexpr void for_range(F&& f)
{
    using t = std::common_type_t<decltype(E), decltype(B)>;
    using seq = std::make_integer_sequence<t, E - B>;

    [&]<auto... Xs>(std::integer_sequence<t, Xs...>)
    {
        for_values<(B + Xs)...>(f);
    }
    (seq{});
}

template <typename...>
struct typelist
{
};

template <typename Typelist, typename F>
constexpr void for_typelist(F&& f)
{
    [&f]<typename... Ts>(typelist<Ts...>)
    {
        (f.template operator()<Ts>(), ...);
    }
    (Typelist{});
}

template <typename Tuple, typename F>
constexpr void for_tuple(Tuple&& t, F&& f)
{
    std::apply([&f](auto&&... xs) { (f(xs), ...); }, t);
}

int main()
{
    int acc = 0;

    for_types<int, float, char>([&acc]<typename T>() { acc += sizeof(T); });

    for_values<0, 2u, 5ul>([]<auto X>() { std::cout << X << " "; });
    std::cout << '\n';

    for_range<(-5), 5>([]<auto X>() { std::cout << X << " "; });
    std::cout << '\n';

    using tl = typelist<int, float, char>;
    for_typelist<tl>([]<typename T>()
    {
        std::cout << sizeof(T) << ' ';
    });
    std::cout << '\n';

    return acc;
}
