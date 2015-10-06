#include "NewHV.hpp"

struct ST0
{
    char data[10];
    ST0() { ssvu::lo("ST0 CTOR") << " \n"; }
    ~ST0() { ssvu::lo("ST0 DTOR") << " \n"; }
};

struct ST1
{
    char data[20];
    ST1() { ssvu::lo("ST1 CTOR") << " \n"; }
    ~ST1() { ssvu::lo("ST1 DTOR") << " \n"; }
};


int main()
{
    using namespace ssvu;

    {
        HVSingle<std::string> test;
        test.refresh();

        auto h0 = test.create();
        auto h1 = test.create();
        auto h2 = test.create();
        test.refresh();

        h0.get() = "h0 str";
        h1.get() = "h1 str";
        h2.get() = "h2 str";

        ssvu::lo("h0 str") << h0.get() << "\n";
        ssvu::lo("h1 str") << h1.get() << "\n";
        ssvu::lo("h2 str") << h2.get() << "\n";

        *h0 = "h0 str new";
        ssvu::lo("h0 str") << *h0 << "\n";

        h0->clear();
        ssvu::lo("h0 str") << *h0 << "\n";

        h2.setDead();

        for(auto& s : test) ssvu::lo() << s << ", " << std::endl;
        for(auto& s : makeRange(test.beginIdx(), test.endIdx()))
            ssvu::lo() << s << ", " << std::endl;

        for(const auto& s : test) ssvu::lo() << s << ", " << std::endl;
        for(const auto& s : makeRange(test.beginIdx(), test.endIdx()))
            ssvu::lo() << s << ", " << std::endl;

        const auto& ctest(test);


        for(const auto& s : ctest) ssvu::lo() << s << ", " << std::endl;
        for(const auto& s : makeRange(ctest.beginIdx(), ctest.endIdx()))
            ssvu::lo() << s << ", " << std::endl;
    }

    {
        HVSingle<UPtr<std::string>> test;
        test.refresh();

        auto h0 = test.create();
        auto h1 = test.create();
        auto h2 = test.create();
        test.refresh();

        h0.get() = makeUPtr<std::string>("h0 test");
        h1.get() = makeUPtr<std::string>("h1 test");
        h2.get() = makeUPtr<std::string>("h2 test");

        ssvu::lo("h0 str") << *(h0.get()) << "\n";
        ssvu::lo("h1 str") << *(h1.get()) << "\n";
        ssvu::lo("h2 str") << *(h2.get()) << "\n";

        **h0 = "h0 str new";
        ssvu::lo("h0 str") << **h0 << "\n";

        (*h0)->clear();
        ssvu::lo("h0 str") << **h0 << "\n";

        h2.setDead();
    }


    {
        HVMulti<ST0, ST1, std::string> test;
        test.refresh();

        auto h0 = test.create();
        auto h1 = test.create();
        auto h2 = test.create();
        test.refresh();

        h0.get<std::string>() = "h0 str";
        h1.get<std::string>() = "h1 str";
        h2.get<std::string>() = "h2 str";

        ssvu::lo("h0 str") << h0.get<std::string>() << "\n";
        ssvu::lo("h1 str") << h1.get<std::string>() << "\n";
        ssvu::lo("h2 str") << h2.get<std::string>() << "\n";

        std::get<2>(*h0) = "h0 str new";
        ssvu::lo("h0 str") << std::get<2>(*h0) << "\n";

        for(auto x : makeRange(
                test.beginSingle<std::string>(), test.endSingle<std::string>()))
            ssvu::lo("aaa") << x << "\n";

        for(auto x : makeRange(test.beginSingleIdx<std::string>(),
                test.endSingleIdx<std::string>()))
            ssvu::lo("aaa") << x << "\n";
    }


    // h1.destroy();
    // test.refresh();

    // ssvu::lo("h0 str") << h0.get<std::string>() << "\n";
    // ssvu::lo("h1 str") << h1.get<std::string>() << "\n";
    // ssvu::lo("h2 str") << h2.get<std::string>() << "\n";

    return 0;
}