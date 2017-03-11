#include "../callable_utils.hpp"
#include "../any_type.hpp"
#include "./test_utils.hpp"

void f0(int);
void f1(int, int);

inline auto l0 = [](int) {};
inline auto l1 = [](int, int) {};

TEST_MAIN()
{
    using namespace vr;

    static_assert(is_unary_callable_with_any<decltype(&f0), int>{});
    static_assert(!is_unary_callable_with_any<decltype(&f1), int>{});

    static_assert(!is_binary_callable_with_any<decltype(&f0), int>{});
    static_assert(is_binary_callable_with_any<decltype(&f1), int>{});

    static_assert(is_unary_callable_with_any<decltype(l0), int>{});
    static_assert(!is_unary_callable_with_any<decltype(l1), int>{});

    static_assert(!is_binary_callable_with_any<decltype(l0), int>{});
    static_assert(is_binary_callable_with_any<decltype(l1), int>{});

    // Implicit conversions:
    static_assert(is_unary_callable_with_any<decltype(&f0), float>{});
    static_assert(!is_unary_callable_with_any<decltype(&f1), float>{});

    static_assert(!is_binary_callable_with_any<decltype(&f0), float>{});
    static_assert(is_binary_callable_with_any<decltype(&f1), float>{});

    static_assert(is_unary_callable_with_any<decltype(l0), float>{});
    static_assert(!is_unary_callable_with_any<decltype(l1), float>{});

    static_assert(!is_binary_callable_with_any<decltype(l0), float>{});
    static_assert(is_binary_callable_with_any<decltype(l1), float>{});

    // `any_type`:
    static_assert(is_unary_callable_with_any<decltype(&f0), any_type>{});
    static_assert(!is_unary_callable_with_any<decltype(&f1), any_type>{});

    static_assert(!is_binary_callable_with_any<decltype(&f0), any_type>{});
    static_assert(is_binary_callable_with_any<decltype(&f1), any_type>{});

    static_assert(is_unary_callable_with_any<decltype(l0), any_type>{});
    static_assert(!is_unary_callable_with_any<decltype(l1), any_type>{});

    static_assert(!is_binary_callable_with_any<decltype(l0), any_type>{});
    static_assert(is_binary_callable_with_any<decltype(l1), any_type>{});

    // Overloaded function objects:
    auto al0 = [](auto) {};
    auto al1 = [](auto, auto) {};
    auto alv = [](auto...) {};

    static_assert(is_unary_callable_with_any<decltype(al0), int>{});
    static_assert(!is_unary_callable_with_any<decltype(al1), int>{});
    static_assert(!is_binary_callable_with_any<decltype(al0), int>{});
    static_assert(is_binary_callable_with_any<decltype(al1), int>{});

    static_assert(is_unary_callable_with_any<decltype(alv), int>{});
    static_assert(is_binary_callable_with_any<decltype(alv), int>{});

    // Mandatory helper:
    auto ah0 = [](auto x) -> decltype(x.foo()) { return x.foo(); };
    static_assert(!is_unary_callable_with_any<decltype(ah0)>{});

    struct Foo
    {
        void foo();
    };
    static_assert(is_unary_callable_with_any<decltype(ah0), Foo>{});

    // SFINAE:
    static_assert(is_unary_callable_with_any<decltype(ah0), int, Foo>{});
}