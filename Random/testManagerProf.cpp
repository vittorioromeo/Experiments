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

    if(true)
    {
        ssvu::lo() << "starting" << std::endl;
        while(true)
        {
            HandleVector<OSmall> storage;
            storage.reserve(s + 10000);

            for(auto i(0u); i < s; ++i) storage.create();

            storage.refresh();

            storage.forEach([](OSmall& mO)
                {
                    mO.a();
                });

            {
                int k{0};
                storage.forEachAtom([k](decltype(storage)::Atom& mO) mutable
                    {
                        if(k % 3 == 0) mO.setDead();
                        ++k;
                    });
            }

            storage.refresh();

            for(auto k(0u); k < 3; ++k)
            {
                for(auto j(0u); j < 10000; ++j)
                {
                    auto h(storage.create());
                    h.destroy();
                }

                storage.refresh();
            }

            storage.forEach([](OSmall& mO)
                {
                    mO.a();
                });
        }
    }

    if(false)
    {
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


            for(auto& i : storage) i->a();
        }
    }
}

int main()
{
    // SSVUT_RUN();
    doBench();
    return 0;
}