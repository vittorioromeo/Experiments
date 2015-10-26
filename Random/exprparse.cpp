#include <functional>
#include <iostream>

// highest precedence and left-to-right
// *   /   %

// lowest precendence and right-to-left
// &=   ^=   |=

std::function<std::string()> printer_lhs, printer_rhs;


struct lhs_catcher
{
};

struct rhs_catcher 
{};

template <typename T>
const T& operator*(lhs_catcher&&, const T& v)
{
    printer_lhs = [&]
    {
        return std::to_string(v);
    };
    return v;
}

template <typename T>
auto operator*(const T& v, rhs_catcher&& c) 
{
    printer_rhs = [&]
    {
        return std::to_string(v);
    };
    return c;
}

template <typename T>
const T& operator-(const T& v, rhs_catcher&&)
{
std::terminate();}   


#define MY_ASSERT(...)               \
    if(!(__VA_ARGS__))               \
    {                                \
        lhs_catcher{} * __VA_ARGS__ * rhs_catcher{}; \
        std::cout << #__VA_ARGS__ << "\n"; \
        std::cout << "lhs: " << printer_lhs() << "\n";      \
        std::cout << "rhs: " << printer_rhs() << "\n\n";      \
    }


int main()
{
    int f = 10; 

    MY_ASSERT(f + 1 < (f - 1));

    return 0;
}