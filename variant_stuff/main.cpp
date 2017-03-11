// Copyright(c) 2016-2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./overload.hpp"
#include "./visit.hpp"

struct vis
{
    auto operator()(int)
    {
        std::cout << "int\n";
    }
    auto operator()(float)
    {
        std::cout << "float\n";
    }
};

struct vis2
{
    auto operator()(int, int)
    {
        std::cout << "ii\n";
    }
    auto operator()(float, int)
    {
        std::cout << "fi\n";
    }
    auto operator()(int, float)
    {
        std::cout << "if\n";
    }
    auto operator()(float, float)
    {
        std::cout << "ff\n";
    }
};

int main()
{
    vis x;

    boost::variant<int, float> v0;
    eggs::variant<int, float> v1;
    std::variant<int, float> v2;

    v0 = 1;
    v1 = 1;
    v2 = 1;

    vr::visit(x, v0);
    vr::visit(x, v1);
    vr::visit(x, v2);

    v0 = 1.f;
    v1 = 1.f;
    v2 = 1.f;

    vr::visit(x, v0);
    vr::visit(x, v1);
    vr::visit(x, v2);

    vis2 x2;
    vr::visit(x2, v0, v0);
    vr::visit(x2, v1, v1);
    vr::visit(x2, v2, v2);

    auto x3 = vr::overload(               // .
        [](int) { std::cout << "i\n"; },  // .
        [](float) { std::cout << "f\n"; } // .
        );

    vr::visit(x3, v0);
    vr::visit(x3, v1);
    vr::visit(x3, v2);

    // clang-format off
    vr::visit_in_place(
        [](int) { std::cout << "i\n"; }, 
        [](float) { std::cout << "f\n"; })(v0);

    vr::visit_in_place(
        [](int) { std::cout << "i\n"; }, 
        [](float) { std::cout << "f\n"; })(v1);
        
    vr::visit_in_place(
        [](int) { std::cout << "i\n"; }, 
        [](float) { std::cout << "f\n"; })(v2);
    // clang-format on
}