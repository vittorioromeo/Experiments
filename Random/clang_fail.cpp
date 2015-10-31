#include <utility>
#include <type_traits>
#include <tuple>
#include <cassert>

#define DO_NOT_OPTIMIZE(x) ((unsigned char volatile&)x) = x

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define ALWAYS_INLINE __attribute__((always_inline)) inline

template <typename... Ts>
ALWAYS_INLINE constexpr decltype(auto) make_ref_tpl(Ts&&... xs) noexcept
{
    return std::tuple<Ts...>(FWD(xs)...);
}

template <std::size_t TN, typename... Ts>
ALWAYS_INLINE constexpr decltype(auto) nth_arg(Ts&&... xs) noexcept
{
    using nth_arg_type =
        std::tuple_element_t<TN, decltype(make_ref_tpl(FWD(xs)...))>;

    return static_cast<nth_arg_type>(
        FWD(std::get<TN>(make_ref_tpl(FWD(xs)...))));
}

void constexpr_tests()
{
    constexpr auto a0 = nth_arg<0>(1, 2, 3);
    constexpr auto a1 = nth_arg<1>(1, 2, 3);
    constexpr auto a2 = nth_arg<2>(1, 2, 3);

    DO_NOT_OPTIMIZE(a0);
    DO_NOT_OPTIMIZE(a1);
    DO_NOT_OPTIMIZE(a2);
}


void non_constexpr_tests()
{
    auto&& a0 = nth_arg<0>(1, 2, 3);
    auto&& a1 = nth_arg<1>(1, 2, 3);
    auto&& a2 = nth_arg<2>(1, 2, 3);

    DO_NOT_OPTIMIZE(a0);
    DO_NOT_OPTIMIZE(a1);
    DO_NOT_OPTIMIZE(a2);
}


void non_constexpr_lv_tests()
{
    auto i0 = 1;
    auto i1 = 2;
    const auto i2 = 3;

    auto& a0 = nth_arg<0>(i0, i1, i2);
    auto& a1 = nth_arg<1>(i0, i1, i2);
    auto& a2 = nth_arg<2>(i0, i1, i2);

    a0 = 11;
    a1 = 22;

    // Does not compile as intended:
    // a2 = 33;

    DO_NOT_OPTIMIZE(a0);
    DO_NOT_OPTIMIZE(a1);
    DO_NOT_OPTIMIZE(a2);
}

int main()
{
    constexpr_tests();
    non_constexpr_tests();
    non_constexpr_lv_tests();
}