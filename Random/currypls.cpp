#include <iostream>
#include <type_traits>
#include <utility>
#include <cassert>

int uncurried_sum(int x, int y, int z)
{
    return x + y + z;
};

template <typename...>
using void_t = void;

template <typename, typename = void>
class callable_zero : public std::false_type
{
};

template <typename T>
class callable_zero<T, void_t<decltype(std::declval<T>()())>>
    : public std::true_type
{
};


template <typename TF, bool TZeroCallable>
struct curry_helper2
{
    static auto exec(TF x)
    {
        return [=](auto k)
        {
            auto y = [=](auto... ks)
            {
                return x(k, ks...);
            };

            return curry_helper2<decltype(y), false>::exec(y);
        };
    }
};

template <typename TF>
struct curry_helper2<TF, true>
{
    static auto exec(TF x)
    {
        return x();
    }
};

template <typename TF>
auto my_curry_3(TF f)
{
    return curry_helper2<TF, callable_zero<decltype(f)>{}>::exec(f);
}

int main()
{
    assert(my_curry_3(uncurried_sum)(1)(2)(3) == 1 + 2 + 3);
}
