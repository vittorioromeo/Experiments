#include <functional>
#include <iostream>

std::function<std::string()> printer;

struct lhs_catcher
{
};

template <typename T>
const T& operator*(lhs_catcher&&, const T& v)
{
    printer = [&]
    {
        return std::to_string(v);
    };
    return v;
}


#define MY_ASSERT(...)               \
    if(!(__VA_ARGS__))               \
    {                                \
        lhs_catcher{} * __VA_ARGS__; \
        std::cout << printer();      \
    }


int main()
{
    int f = 10; 

    MY_ASSERT(f < 5);

    return 0;
}