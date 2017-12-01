#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------
// Compile-time iteration
// ----------------------------------------------------------------------------
namespace detail
{
    template <typename F, auto... Is>
    void repeat_impl(F&& f, std::index_sequence<Is...>)
    {
        (f(std::integral_constant<std::size_t, Is>{}), ...);
    }
} // namespace detail

template <auto N, typename F>
void repeat(F&& f)
{
    detail::repeat_impl(f, std::make_index_sequence<N>{});
}
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// Looping over the elements of a tuple
// ----------------------------------------------------------------------------
template <typename F, typename Tuple>
void for_tuple(F&& f, Tuple&& tuple)
{
    std::apply([&f](auto&&... xs) { (f(std::forward<decltype(xs)>(xs)), ...); },
        std::forward<Tuple>(tuple));
}
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// Looping over a set of types
// ----------------------------------------------------------------------------
template <typename T>
struct type_wrapper
{
    using type = T;
};

template <typename T>
inline constexpr type_wrapper<T> t{};

template <typename T>
using unwrap = typename type_wrapper<T>::type;

template <typename... Ts, typename F>
void for_types(F&& f)
{
    (f(t<Ts>), ...);
}
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// Checking for typelist uniqueness
// ----------------------------------------------------------------------------
template <typename...>
inline constexpr auto is_unique = std::true_type{};

template <typename T, typename... Rest>
inline constexpr auto is_unique<T, Rest...> =
    std::bool_constant<(!std::is_same_v<T, Rest> && ...) &&
                       is_unique<Rest...>>{};

static_assert(is_unique<>);
static_assert(is_unique<int>);
static_assert(is_unique<int, float, double>);
static_assert(!is_unique<int, float, double, int>);
static_assert(!is_unique<int, float, double, int, char, char>);
static_assert(is_unique<int, float, double, char>);
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// Short-circuiting JSON visitation
// ----------------------------------------------------------------------------
struct json
{
    template <typename T>
    json operator[](T&&)
    {
        return {};
    }

    const char* key() const
    {
        return "";
    }
};

template <typename F>
struct handler : F
{
    handler(const char*, F&& f) : F{std::move(f)}
    {
    }

    const char* key() const
    {
        return "";
    }
};

struct on
{
    on(const char*)
    {
    }
};

template <typename F>
auto operator|(const on&, F&& f)
{
    return handler{"", std::move(f)};
}

template <typename... Handlers>
void json_switch(const json& j, Handlers&&... hs)
{
    (void)((j.key() == hs.key() ? (hs(j), true) : false) || ...);
}
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// Print comma separated
// ----------------------------------------------------------------------------
struct todo0
{
};
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// Arbitrary `constexpr` folding
// ----------------------------------------------------------------------------
template <typename F, typename Acc, typename... Ts>
constexpr auto lfold(F&& f, Acc acc, Ts&&... ts)
{
    (f(acc, std::forward<Ts>(ts)), ...);
    return acc;
}
// ----------------------------------------------------------------------------

int main()
{
    for_types<int, float>([](auto t) { using type = unwrap<decltype(t)>; });

    // clang-format off
    json_switch(json{},
        on{"foo_request"} | [](auto&& j) { (void) j; },
        on{"bar_request"} | [](auto&& j) { (void) j; },
        on{"baz_request"} | [](auto&& j) { (void) j; }
    );
    // clang-format on

    constexpr auto res =
        lfold([](auto& acc, auto x) { acc += x; }, 1, 2, 3, 4, 5, 6);
    static_assert(res == 1 + 2 + 3 + 4 + 5 + 6);
}
