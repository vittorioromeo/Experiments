#include <SSVUtils/SSVUtils.hpp>

struct Base
{
    bool dead{false};
    volatile int k;
    virtual volatile void call() {}
    virtual ~Base() {}
};

struct Der1 : public Base
{
    char s[10];
    volatile void call() override { k += 1; }
};
struct Der2 : public Base
{
    char b[100];
    volatile void call() override { k += 2; }
};

void doBench()
{
    using namespace ssvu;
    constexpr std::size_t s(1000000);
    constexpr int jj{10};

    Benchmark::start("VecUPtr");
    {
        VecUPtr<Base> v;
        v.reserve(s);

        for(int j{0}; j < jj; ++j)
        {
            for(int i{0}; i < s; ++i)
            {
                if(i % 2 == 0)
                    v.emplace_back(makeUPtr<Der1>());
                else
                    v.emplace_back(makeUPtr<Der2>());
            }

            int i{0};
            for(auto& r : v)
                if(i++ % 3 == 0) r->dead = true;

            eraseRemoveIf(v, [](const UPtr<Base>& mP)
                {
                    return mP->dead;
                });
            v.clear();
        }
    }
    Benchmark::endLo();

    Benchmark::start("VecUPtrRec");
    {
        VecUPtrRec<Base> v;
        v.reserve(s);

        for(int j{0}; j < jj; ++j)
        {
            for(int i{0}; i < s; ++i)
            {
                if(i % 2 == 0)
                    v.emplace_back(makeUPtrRecPoly<Der1, Base>());
                else
                    v.emplace_back(makeUPtrRecPoly<Der2, Base>());
            }

            int i{0};
            for(auto& r : v)
                if(i++ % 3 == 0) r->dead = true;

            eraseRemoveIf(v, [](const UPtrRec<Base>& mP)
                {
                    return mP->dead;
                });
            v.clear();
        }
    }
    Benchmark::endLo();

    Benchmark::start("PolyManager");
    {
        PolyManager<Base> v;
        v.reserve(s);

        for(int j{0}; j < jj; ++j)
        {
            for(int i{0}; i < s; ++i)
            {
                if(i % 2 == 0)
                    v.create<Der1>();
                else
                    v.create<Der2>();
            }

            v.refresh();

            int i{0};
            for(auto& r : v)
                if(i++ % 3 == 0) v.del(*r);

            v.refresh();
            v.clear();
        }
    }
    Benchmark::endLo();
}

int main()
{
    ssvu::lo("N") << sizeof(ssvu::UPtr<Base>) << std::endl;
    ssvu::lo("R") << sizeof(ssvu::UPtrRec<Base>) << std::endl;
    SSVU_ASSERT(false);
    for(int n{0}; n < 3; ++n) doBench();
}