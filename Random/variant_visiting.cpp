#include <experimental/type_traits>
#include <utility>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

template <typename>
struct parent;

template <template <typename...> class TVariant, typename... Ts>
struct parent<TVariant<Ts...>>
{
    template <template <template <typename> class> class TX>
    using apply = TX<TVariant>;
};

template <template <typename> class TVariant>
struct visit_helper;

template <typename>
struct visit_helper_finder;

template <template <typename...> class TVariant, typename... Ts>
struct visit_helper_finder<TVariant<Ts...>>
{
    using type = visit_helper<TVariant>;
};

namespace vr
{
    template <typename TVisitor>
    constexpr decltype(auto) visit(TVisitor&& visitor)
    {
        return FWD(visitor)();
    }

    template <typename TVisitor, typename TVariant, typename... TVariants>
    constexpr decltype(auto) visit(
        TVisitor&& visitor, TVariant&& variant, TVariants&&... variants)
    {
        // using helper = typename parent<std::decay_t<TVariant>>::template
        // apply<visit_helper>;

        using helper =
            typename visit_helper_finder<std::decay_t<TVariant>>::type;

        return helper{}(FWD(visitor), FWD(variant), FWD(variants)...);
    }
}

#include <boost/variant.hpp>
#include <eggs/variant.hpp>
#include <iostream>
#include <variant>

#define DEFINE_VISIT_HELPER(type, function)                   \
    template <>                                               \
    struct visit_helper<type>                                 \
    {                                                         \
        template <typename... Ts>                             \
        constexpr decltype(auto) operator()(Ts&&... xs) const \
            noexcept(noexcept(function(FWD(xs)...)))          \
        {                                                     \
            return function(FWD(xs)...);                      \
        }                                                     \
    }

/*
template <>
struct visit_helper<std::variant>
{
    template <typename... Ts>
    decltype(auto) operator()(Ts&&... xs) const
    {
        return std::visit(FWD(xs)...);
    }
};

template <>
struct visit_helper<boost::variant>
{
    template <typename... Ts>
    decltype(auto) operator()(Ts&&... xs) const
    {
        return boost::apply_visitor(FWD(xs)...);
    }
};

template <>
struct visit_helper<eggs::variant>
{
    template <typename... Ts>
    decltype(auto) operator()(Ts&&... xs) const
    {
        return eggs::variants::apply(FWD(xs)...);
    }
};
*/

DEFINE_VISIT_HELPER(::std::variant, ::std::visit);
DEFINE_VISIT_HELPER(::boost::variant, ::boost::apply_visitor);
DEFINE_VISIT_HELPER(::eggs::variant, ::eggs::variants::apply);

struct vis
{
    auto operator()(int)
    {
        std::cout << "int\n";
    }
    auto operator()(float)
    {
        std::cout << "float\n";
    }
};

struct vis2
{
    auto operator()(int, int)
    {
        std::cout << "ii\n";
    }
    auto operator()(float, int)
    {
        std::cout << "fi\n";
    }
    auto operator()(int, float)
    {
        std::cout << "if\n";
    }
    auto operator()(float, float)
    {
        std::cout << "ff\n";
    }
};

int main()
{
    vis x;

    boost::variant<int, float> v0;
    eggs::variant<int, float> v1;
    std::variant<int, float> v2;

    v0 = 1;
    v1 = 1;
    v2 = 1;

    vr::visit(x, v0);
    vr::visit(x, v1);
    vr::visit(x, v2);

    v0 = 1.f;
    v1 = 1.f;
    v2 = 1.f;

    vr::visit(x, v0);
    vr::visit(x, v1);
    vr::visit(x, v2);

    vis2 x2;
    vr::visit(x2, v0, v0);
    vr::visit(x2, v1, v1);
    vr::visit(x2, v2, v2);
}