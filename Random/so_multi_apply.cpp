#include <array>
#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

template <typename T0, typename T1, typename TF>
constexpr void for_range(T0 i_begin, T1 i_end, TF&& f)
{
    using ct = std::common_type_t<T0, T1>;
    for(ct i = i_begin; i < i_end; ++i)
    {
        f(i);
    }
}

template <std::size_t S, class... Ts>
constexpr auto make_indices()
{
    constexpr std::size_t sizes[] = {std::tuple_size<std::decay_t<Ts>>{}...};

    using arr_t = std::size_t[S];
    std::pair<arr_t, arr_t> ret{};

    std::size_t i = 0;
    for_range(0, sizeof...(Ts), [&](auto i_tuple) {
        for_range(0, sizes[i_tuple], [&](auto i_element) {
            ret.first[i] = i_tuple;
            ret.second[i] = i_element;
            ++i;
        });
    });

    return ret;
}

template <class F, class... Tuples, std::size_t... OuterIs,
    std::size_t... InnerIs>
constexpr decltype(auto) multi_apply_imp_2(std::index_sequence<OuterIs...>,
    std::index_sequence<InnerIs...>, F&& f, std::tuple<Tuples...>&& t)
{
    return std::forward<F>(f)(
        std::get<InnerIs>(std::get<OuterIs>(std::move(t)))...);
}

template <class F, class... Tuples, std::size_t... Is>
constexpr decltype(auto) multi_apply_imp_1(
    std::index_sequence<Is...>, F&& f, std::tuple<Tuples...>&& t)
{
    constexpr auto indices = make_indices<sizeof...(Is), Tuples...>();
    return multi_apply_imp_2(std::index_sequence<indices.first[Is]...>{},
        std::index_sequence<indices.second[Is]...>{}, std::forward<F>(f),
        std::move(t));
}

template <class F, class... Tuples>
constexpr decltype(auto) multi_apply(F&& f, Tuples&&... ts)
{
    return multi_apply_imp_1(
        std::make_index_sequence<(
            ... + std::tuple_size<std::remove_reference_t<Tuples>>{})>{},
        std::forward<F>(f), std::forward_as_tuple(std::forward<Tuples>(ts)...));
}

int main()
{
    auto t0 = std::make_tuple(1, 2);
    auto t1 = std::make_tuple(3, 6, 4, 5);
    auto sum = [](auto... xs) { return (0 + ... + xs); };

    std::cout << multi_apply(sum, t0, t1, std::make_tuple(7)) << '\n';
}