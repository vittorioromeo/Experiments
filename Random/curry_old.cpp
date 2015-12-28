#include <type_traits>
#include <utility>
#include <cassert>

template <typename...>
using void_t = void;

template <typename, typename = void>
class is_zero_callable : public std::false_type
{
};

template <typename T>
class is_zero_callable<T, void_t<decltype(std::declval<T>()())>>
    : public std::true_type
{
};

template <typename TF, bool TLastStep>
struct curry_impl
{
    static auto exec(TF f)
    {
        // Bind `x` to subsequent calls.
        return [=](auto x)
        {
            auto bound_f = [=](auto... xs)
            {
                return f(x, xs...);
            };

            using last_step = std::integral_constant<bool, /* ??? */>;

            // Curry recursively.
            return curry_impl<decltype(bound_f), last_step>::exec(bound_f);
        };
    }
};

template <typename TF>
struct curry_impl<TF, true>
{
    static auto exec(TF f)
    {
        return f();
    }
};

template <typename TF>
auto curry(TF f)
{
    return curry_impl<TF, is_zero_callable<decltype(f)>{}>::exec(f);
}

auto sum3 = [](int x, int y, int z)
{
    return x + y + z;
};

int main()
{
    assert(curry(sum3)(1)(1)(1) == 3);

    auto plus2 = curry(sum3)(1)(1);
    assert(plus2(1) == 3);
    assert(plus2(3) == 5);
}