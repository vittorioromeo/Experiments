#include <iostream>
#include <type_traits>
#include <utility>
#include <cassert>
#include <vrm/core/static_if.hpp>

auto zeroary_fn = []
{
    return 0;
};

auto unary_fn = [](auto)
{
    return zeroary_fn;
};

auto binary_fn = [](auto)
{
    return unary_fn;
};


int uncurried_sum(int x, int y, int z)
{
    return x + y + z;
};

auto curried_sum = [](auto x)
{
    return [x](auto y)
    {
        return [x, y](auto z)
        {
            return x + y + z;
        };
    };
};

auto curry_1 = [](auto f)
{
    return [=](auto x)
    {
        return [=](auto... ys)
        {
            return f(x, ys...);
        };
    };
};

template <typename T>
struct is_callable_impl
{
private:
    typedef char(&yes)[1];
    typedef char(&no)[2];

    struct Fallback
    {
        void operator()();
    };
    struct Derived : T, Fallback
    {
    };

    template <typename U, U>
    struct Check;

    template <typename>
    static yes test(...);

    template <typename C>
    static no test(Check<void (Fallback::*)(), &C::operator()>*);

public:
    static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
};
template <typename T>
using is_callable = std::integral_constant<bool, is_callable_impl<T>::value>;

static_assert(is_callable<decltype(curry_1)>{}, "");
static_assert(is_callable<decltype(uncurried_sum)>{}, "");



using namespace vrm::core;

using namespace std;

struct yes
{
};

template <typename FUNC>
struct is_callable_with
{
    template <typename... PARAMS,
        typename = decltype(std::declval<FUNC>()(std::declval<PARAMS>()...))>
    yes test(int, PARAMS...) const;

    void test(...) const;

    template <typename... PARAMS>
    auto operator()(PARAMS...) const
        -> decltype(test(0, std::declval<PARAMS>()...));
};

template <typename FUNC, typename CAPTURE>
using is_callable_by_capture = std::is_same<yes,
    decltype(std::declval<CAPTURE>()(std::declval<is_callable_with<FUNC>>()))>;

// case for when FUNC can be called with the current CAPTURE
template <typename FUNC, typename CAPTURE>
auto curry_impl(FUNC f, CAPTURE c, ...)
{
    return c(f);
}

// case for when FUNC cannot be called with current CAPTURE
// and we assume this is because an additional entry in the CAPTURE is needed
template <typename FUNC, typename CAPTURE,
    typename = std::enable_if_t<!is_callable_by_capture<FUNC, CAPTURE>::value>>
auto curry_impl(FUNC f, CAPTURE c, int)
{
    return [f, c](auto new_param)
    {
        auto make_new_capture = [new_param](auto... params)
        {
            return [new_param, params...](auto callee)
            {
                return callee(params..., new_param);
            };
        };

        return curry_impl(f, c(make_new_capture), 0);
    };
}

template <typename T>
auto curry(T func)
{
    return curry_impl(func,
        [](auto f)
        {
            return f();
        },
        0);
}

template <typename TF>
auto lif(TF f)
{
    return [=](auto x)
    {
        return [=](auto... ys)
        {
            f(x, ys...);
        };
    };
}

template <typename TF>
auto my_curry(TF f)
{
    return [=](auto x)
    {
        return [=](auto y)
        {
            return [=](auto z)
            {
                return f(x, y, z);
            };
        };
    };
}

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

auto testx = []()
{
};
auto testy = [](auto)
{
};
// auto testz = [](auto...){};
static_assert(callable_zero<decltype(testx)>::value, "");
static_assert(!callable_zero<decltype(testy)>::value, "");
// static_assert(callable_zero<decltype(testz)>::value, "");
static_assert(!callable_zero<decltype(uncurried_sum)>::value, "");

template <typename TF, int TArity>
struct curry_helper
{
    static auto exec(TF x)
    {
        return [=](auto k)
        {
            auto y = [=](auto... ks)
            {
                return x(k, ks...);
            };

            return curry_helper<decltype(y), TArity - 1>::exec(y);
        };
    }
};

template <typename TF>
struct curry_helper<TF, 0>
{
    static auto exec(TF x)
    {
        return x();
    }
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

            return curry_helper2<decltype(y),
                !callable_zero<decltype(y())>{}>::exec(y);
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


template <int TArity, typename TF>
auto my_curry_2(TF f)
{
    return curry_helper<TF, TArity>::exec(f);
    /*
    return static_if(bool_v<(TArity == 0)>)
        .then([=](auto x)
            {
                static_assert(callable_zero<decltype(x)>{}, "");
                return x();
            })
        .else_([=](auto x)
            {
                return [=](auto k)
                {
                    return my_curry_2<TArity - 1>([=](auto... ks)
                        {
                            return x(k, ks...);
                        });
                };
            })(f);
*/
}

template <typename TF>
auto my_curry_3(TF f)
{
    return curry_helper2<TF, callable_zero<decltype(f)>{}>::exec(f);
}

int main()
{
    assert(curry(uncurried_sum)(1)(2)(3) == 1 + 2 + 3);
    assert(my_curry(uncurried_sum)(1)(2)(3) == 1 + 2 + 3);

    assert(my_curry_2<3>(uncurried_sum)(1)(2)(3) == 1 + 2 + 3);
    assert(my_curry_3(uncurried_sum)(1)(2)(3) == 1 + 2 + 3);
}
