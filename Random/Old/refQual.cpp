#include <iostream>

struct S0
{
    auto get() & { return 0; }
    auto get() const & { return 1; }
    auto get() && { return 2; }
};

int main()
{
    S0 lv;
    const S0& clv{S0{}};

    std::cout << lv.get() << "\n";
    std::cout << clv.get() << "\n";
    std::cout << S0{}.get() << "\n";

    return 0;
}