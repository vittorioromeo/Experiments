#include "./utils.hpp"
#include <variant>
#include <boost/variant.hpp>
#include <eggs/variant.hpp>

template <typename>
struct parent;

template <template <typename...> class TVariant, typename... Ts>
struct parent<TVariant<Ts...>>
{
    template <template <template <typename> class> class TX>
    using apply = TX<TVariant>;
};

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
