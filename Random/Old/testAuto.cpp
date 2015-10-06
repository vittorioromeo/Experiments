#include <typeinfo>
#include <iostream>
#include <initializer_list>

int main()
{
    auto i{0};
    auto u{0u};

    std::cout << typeid(decltype(i)).name() << "\n"
              << typeid(decltype(u)).name() << std::endl;
}