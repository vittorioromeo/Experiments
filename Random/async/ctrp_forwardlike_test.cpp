#include <vrm/core/type_traits.hpp>

template <typename TDerived>
struct crtp
{
    auto& as_derived() &
    {
        return static_cast<TDerived&>(*this);
    }

    const auto& as_derived() const &
    {
        return static_cast<const TDerived&>(*this);
    }

    auto as_derived() &&
    {
        return static_cast<TDerived&&>(*this);
    }
};

template <typename T>
struct holder : crtp<holder<T>>
{
    T _x;
};

template <int>
struct nocopy
{
    nocopy() = default;
    nocopy(const nocopy&) = delete;
    nocopy(nocopy&&) = default;
};

using vrm::core::forward_like;

template <typename T>
void g(T)
{
}

template <typename T0, typename T1>
void f(T0&& x0, T1&& x1)
{
    g(std::forward<T0>(x0));
    g(std::forward<T1>(x1));
    g(forward_like<T0>(x1));
    g(forward_like<T1>(x0));
}

int main()
{
    f(holder<nocopy<0>>{}, holder<nocopy<1>>{});
    f([a = holder<nocopy<0>>{}]{}, [b = holder<nocopy<1>>{}]{});
}