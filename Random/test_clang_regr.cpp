#include <utility>
#include <type_traits>
#include <tuple>
#include <cassert>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define ALWAYS_INLINE __attribute__((always_inline)) inline

#define DEPARENS(...) __VA_ARGS__

#define SA_TYPE(value, type) \
    static_assert(std::is_same<decltype value, DEPARENS type>{}, "")

template <typename... Ts>
ALWAYS_INLINE constexpr decltype(auto) make_ref_tpl(Ts&&... xs) noexcept
{
    return std::tuple<Ts...>(FWD(xs)...);
}

template <std::size_t nth, typename... Ts>
ALWAYS_INLINE constexpr decltype(auto) nth_arg(Ts&&... xs) noexcept
{
    // Forward everything as a tuple, and forward-return the nth element.
    return std::get<nth>(make_ref_tpl(FWD(xs)...));
}

template <std::size_t ibegin, std::size_t... idxs, typename... Ts>
ALWAYS_INLINE constexpr decltype(auto) subpack_build(
    std::index_sequence<idxs...>, Ts&&... xs) noexcept
{
    return make_ref_tpl(nth_arg<ibegin + idxs>(FWD(xs)...)...);
}

template <std::size_t ibegin, std::size_t iend, typename... Ts>
ALWAYS_INLINE constexpr decltype(auto) subpack(Ts&&... xs) noexcept
{
    return subpack_build<ibegin>(
        std::make_index_sequence<iend - ibegin>(), FWD(xs)...);
}

void non_constexpr_tests()
{
    auto&& a0 = nth_arg<0>(1, 2, 3);
    auto&& a1 = nth_arg<1>(1, 2, 3);
    auto&& a2 = nth_arg<2>(1, 2, 3);

    SA_TYPE((nth_arg<0>(1, 2, 3)), (int&&));
    SA_TYPE((nth_arg<1>(1, 2, 3)), (int&&));
    SA_TYPE((nth_arg<2>(1, 2, 3)), (int&&));

    volatile auto r0 = a0;
    volatile auto r1 = a1;
    volatile auto r2 = a2;

    (void)r0;
    (void)r1;
    (void)r2;


    // auto sub0(subpack<1, 3>(2, 4, 6, 8, 10));
    // SA_TYPE((sub0), (std::tuple<int, int>));

    // volatile auto sr0 = std::get<0>(sub0);
    // volatile auto sr1 = std::get<1>(sub0);
}


void non_constexpr_lv_tests()
{


    // Does not compile as intended:
    // a2 = 33;

    // volatile auto r0 = a0;
    // volatile auto r1 = a1;
    // volatile auto r2 = a2;

    // auto sub0(subpack<1, 3>(i0, i1, i2, i0, i1, i2));
    // SA_TYPE((sub0), (std::tuple<int&, const int&>));

    // volatile auto& sr0 = std::get<0>(sub0);
    // volatile auto& sr1 = std::get<1>(sub0);

    // sr0 = 11;

    // Does not compile as intended:
    // sr1 = 22;
}


int main()
{
    auto i0 = 1;
    auto i1 = 2;
    const auto i2 = 3;

    auto& a0 = nth_arg<0>(i0, i1, i2);
    auto& a1 = nth_arg<1>(i0, i1, i2);
    auto& a2 = nth_arg<2>(i0, i1, i2);

    SA_TYPE((nth_arg<0>(i0, i1, i2)), (int&));
    SA_TYPE((nth_arg<1>(i0, i1, i2)), (int&));
    SA_TYPE((nth_arg<2>(i0, i1, i2)), (const int&));

    a0 = 11;
    a1 = 22;
}