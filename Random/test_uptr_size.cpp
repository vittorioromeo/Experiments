#include <memory>
#include <iostream>

int main()
{
    struct del { long long x; float sx, y, z;};

    #define CSIZE(...) std::cout << #__VA_ARGS__ << ": " << sizeof(__VA_ARGS__) << "\n";

    CSIZE(std::unique_ptr<int>);
    CSIZE(std::unique_ptr<int[]>);

    CSIZE(std::unique_ptr<int, del>);
    CSIZE(std::unique_ptr<int[], del>);

    return 0;
}