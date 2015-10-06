#include <SSVUtils/Core/Core.hpp>

struct S0
{
    template <typename T>
    decltype(auto) fn(int mA, T&& mB) const
    {
        ssvu::lo() << mA << "\n";
        return ssvu::fwd<T>(mB);
    }
};


int main()
{
    int i0 = 1;
    int& i1 = i0;
    const int i2 = 2;
    const int& i3 = 3;

    S0 s;
    s.fn(0, i0);
    s.fn(1, i1);
    s.fn(2, i2);
    s.fn(3, i3);

    return 0;
}