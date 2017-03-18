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
                auto v = make(1);
                vr::visit_in_place(v)( // .
                    [](int x) { TEST_ASSERT_OP(x, ==, 1); },
                    [](float x) { TEST_ASSERT(false); },
                    [](char x) { TEST_ASSERT(false); });
            }

            {
                auto v = make(2.f);
                vr::visit_in_place(v)( // .
                    [](int x) { TEST_ASSERT(false); },
                    [](float x) { TEST_ASSERT_OP(x, ==, 2.f); },
                    [](char x) { TEST_ASSERT(false); });
            }

            {
                auto v = make('c');
                vr::visit_in_place(v)( // .
                    [](int x) { TEST_ASSERT(false); },
                    [](float x) { TEST_ASSERT(false); },
                    [](char x) { TEST_ASSERT_OP(x, ==, 'c'); });
            }
        });
}