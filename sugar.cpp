#include <tuple>

template <typename F, typename Tuple>
constexpr void for_tuple(F&& f, Tuple&& tuple)
{
    std::apply([&f](auto&&... xs) { (f(std::forward<decltype(xs)>(xs)), ...); },
        std::forward<Tuple>(tuple));
}

template <typename... Ts>
struct any_of
{
    std::tuple<const Ts&...> _data;
    constexpr any_of(const Ts&... xs) noexcept : _data{xs...}
    {
    }

    template <typename T>
    constexpr bool is(const T& x) const
    {
        return std::apply(
            [&x](auto&&... ys) { return ((x == ys) || ...); }, _data);
    }
};

int main()
{
    constexpr int a = 0;
    constexpr int b = 2;
    constexpr int c = 3;
    constexpr int d = 4;

    if constexpr(any_of(a, b, c, d).is(5))
    {
        // noice.
    }
}
