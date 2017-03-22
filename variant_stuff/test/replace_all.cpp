#include "../replace_all.hpp"
#include "./test_utils.hpp"

struct A
{
};

struct B
{
};

template <typename...>
struct bag
{
};

TEST_MAIN()
{
    // Sanity checks.
    SA_SAME((A), (A));
    SA_SAME((B), (B));
    SA_SAME((bag<>), (bag<>));

    SA_SAME((vr::replace_all<A, B, bag<>>), (bag<>));
    SA_SAME((vr::replace_all<A, B, bag<A>>), (bag<B>));
    SA_SAME((vr::replace_all<A, B, bag<B>>), (bag<B>));

    SA_SAME((vr::replace_all<A, B, bag<A, A>>), (bag<B, B>));
    SA_SAME((vr::replace_all<A, B, bag<B, A>>), (bag<B, B>));

    SA_SAME((vr::replace_all<A, B, bag<A, bag<A>>>), (bag<B, bag<B>>));
    SA_SAME((vr::replace_all<A, B, bag<A, bag<A, bag<A>>>>),
        (bag<B, bag<B, bag<B>>>));
}