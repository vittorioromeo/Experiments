#include <SSVUtils/Core/Core.hpp>
/*
#define REPEAT(count, macro, ...) \
    SSVPP_IF(count, SSVPP_EXPAND, SSVPP_EAT) \
    ( \
        SSVPP_OBSTRUCT(REPEAT_INDIRECT)()(SSVPP_DECREMENT(count), macro,
__VA_ARGS__) \
        SSVPP_OBSTRUCT(macro)(SSVPP_DECREMENT(count), __VA_ARGS__) \
    )

#define REPEAT_INDIRECT() REPEAT

//An example of using this macro
#define M(s, i) s
SSVPP_EVAL(REPEAT(8, M)) // 0 1 2 3 4 5 6 7

SSVPP_REPEAT_INC(8, M, ())
*/
int main()
{
    int x = 5;
    int y = 6;
    SSVU_ASSERT_OP(x, ==, y);
    SSVU_ASSERT_OP_MSG(x, ==, y, "x not equal to y");
}