#include <SSVUtils/SSVUtils.hpp>

volatile int state{0};
template <typename T>
struct OV : public T
{
    bool alive{true};
};

void doBench()
{
    using namespace ssvu;

    struct OSmall
    {
        char k[16];
        int myState;
        void a()
        {
            ++state;
            ++myState;
        }
        OSmall() { ++state; }
        ~OSmall() { ++state; }
    };
    struct OBig
    {
        char k[128];
        int myState;
        void a()
        {
            ++state;
            ++myState;
        }
        OBig() { ++state; }
        ~OBig() { ++state; }
    };

    constexpr std::size_t s(10000000);

    ssvu::lo() << "starting" << std::endl;
    while(true)
    {
        MonoManager<OSmall> storage;
        storage.reserve(s + 10000);

        for(auto i(0u); i < s; ++i) storage.create();


        storage.refresh();



        for(auto& i : storage) i->a();


        {
            int k{0};
            for(auto& i : storage)
            {
                if(k++ % 3 == 0) storage.del(*i);
            }
        }



        storage.refresh();



        for(auto k(0u); k < 3; ++k)
        {
            for(auto j(0u); j < 10000; ++j)
            {
                auto& h(storage.create());
                storage.del(h);
            }

            storage.refresh();
        }


        Benchmark::start("forauto");
        for(auto& i : storage) i->a();
        Benchmark::endLo();

        Benchmark::start("foridx");
        for(auto i(0u); i < storage.size(); ++i) storage.getAtTmp(i)->a();
        Benchmark::endLo();

        ssvu::lo().flush();
    }
}

int main()
{
    // SSVUT_RUN();
    doBench();
    return 0;
}