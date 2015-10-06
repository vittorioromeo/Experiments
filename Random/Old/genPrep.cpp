#include <SSVUtils/Core/Core.hpp>

using namespace std;
using namespace ssvu;

constexpr SizeT rcount{100};

int main()
{
    std::string output;

    for(auto i(0u); i < rcount; ++i)
    {
        output += "#define SSVPP_IMPL_FOREACH_";
        output += toStr(i + 1);
        output += "(mAction, mData, mA0, mA1, ...)		";
        output += "mAction(" + toStr(i) + ", mData, mA0) SSVPP_IMPL_FOREACH_" +
                  toStr(i) + "(mAction, mData, mA1, __VA_ARGS__)";
        output += "\n";
    }

    std::cout << output << std::endl;

    return 0;
}