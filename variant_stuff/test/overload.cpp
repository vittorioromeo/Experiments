#include "../overload.hpp"
#include "./test_utils.hpp"

TEST_MAIN()
{
    // nothing
    {
        auto f = vr::overload();
        (void)f;
    }

    // lvalue_0
    {
        auto l = [](int x) { return x + 1; };
        auto f = vr::overload(l);

        TEST_ASSERT_OP(f(1), ==, 2);
    }

    // lvalue_1
    {
        auto l0 = [](int x) { return x + 1; };
        auto l1 = [](char x) { return x; };
        auto f = vr::overload(l0, l1);

        TEST_ASSERT_OP(f(1), ==, 2);
        TEST_ASSERT_OP(f('a'), ==, 'a');
    }

    // lvalue_makes_copy
    {
        auto l = [k = 0]() mutable
        {
            ++k;
            return k;
        };
        auto f = vr::overload(l);

        TEST_ASSERT_OP(f(), ==, 1);
        TEST_ASSERT_OP(f(), ==, 2);
        TEST_ASSERT_OP(f(), ==, 3);

        TEST_ASSERT_OP(l(), ==, 1);
    }

    // rvalue_0
    {
        auto f = vr::overload([](int x) { return x + 1; });

        TEST_ASSERT_OP(f(1), ==, 2);
    }

    // rvalue_1
    {
        auto f =
            vr::overload([](int x) { return x + 1; }, [](char x) { return x; });

        TEST_ASSERT_OP(f(1), ==, 2);
        TEST_ASSERT_OP(f('a'), ==, 'a');
    }
}