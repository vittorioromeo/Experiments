#include "../probably_callable.hpp"
#include "./test_utils.hpp"

void f0(int);
void f1(int, int);

inline auto l0 = [](int) {};
inline auto l1 = [](int, int) {};

TEST_MAIN()
{
    using namespace vr;

    static_assert(is_probably_unary_callable<decltype(&f0)>{});
    static_assert(!is_probably_binary_callable<decltype(&f0)>{});

    static_assert(!is_probably_binary_callable<decltype(&f0)>{});
    static_assert(is_probably_binary_callable<decltype(&f1)>{});

    static_assert(is_probably_unary_callable<decltype(l0)>{});
    static_assert(!is_probably_unary_callable<decltype(l1)>{});

    static_assert(!is_probably_binary_callable<decltype(l0)>{});
    static_assert(is_probably_binary_callable<decltype(l1)>{});

    // Overloaded function objects:
    auto al0 = [](auto) {};
    auto al1 = [](auto, auto) {};
    auto alv = [](auto...) {};

    static_assert(is_probably_unary_callable<decltype(al0)>{});
    static_assert(!is_probably_unary_callable<decltype(al1)>{});
    static_assert(!is_probably_binary_callable<decltype(al0)>{});
    static_assert(is_probably_binary_callable<decltype(al1)>{});

    static_assert(is_probably_unary_callable<decltype(alv)>{});
    static_assert(is_probably_binary_callable<decltype(alv)>{});

    // Mandatory helper:
    auto ah0 = [](auto x) -> decltype(x.foo()) { return x.foo(); };
    static_assert(!is_probably_unary_callable<decltype(ah0)>{});

    struct Foo
    {
        void foo();
    };
    static_assert(is_probably_unary_callable<decltype(ah0), Foo>{});
}