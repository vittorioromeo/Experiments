#include "./test_utils.hpp"
#include "./variant_test_utils.hpp"

TEST_MAIN()
{
    using namespace test;

    with_all_variant_implementations(   // .
        alternatives<int, float, char>, // .
        [](auto make)                   // .
        {
            {
                auto f = vr::overload( // .
                    [](int x) { TEST_ASSERT_OP(x, ==, 1); },
                    [](float x) { TEST_ASSERT(false); },
                    [](char x) { TEST_ASSERT(false); });

                auto v = make(1);
                vr::visit(f, v);
            }

            {
                auto f = vr::overload( // .
                    [](int x) { TEST_ASSERT(false); },
                    [](float x) { TEST_ASSERT_OP(x, ==, 2.f); },
                    [](char x) { TEST_ASSERT(false); });

                auto v = make(2.f);
                vr::visit(f, v);
            }

            {
                auto f = vr::overload( // .
                    [](int x) { TEST_ASSERT(false); },
                    [](float x) { TEST_ASSERT(false); },
                    [](char x) { TEST_ASSERT_OP(x, ==, 'c'); });

                auto v = make('c');
                vr::visit(f, v);
            }
        });
}