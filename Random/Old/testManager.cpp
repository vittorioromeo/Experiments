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

    {
        Benchmark::start("VecUPtr - small");
        {
            Benchmark::start("creation");
            VecUPtr<OV<OSmall>> storage;
            storage.reserve(s + 10000);
            Benchmark::endLo();

            Benchmark::start("filling");
            for(auto i(0u); i < s; ++i) storage.emplace_back(new OV<OSmall>);
            Benchmark::endLo();

            Benchmark::start("iteration");
            for(auto& o : storage) o->a();
            Benchmark::endLo();

            Benchmark::start("set dead");
            {
                int k{0};
                for(auto i(0u); i < s; ++i)
                {
                    if(k % 3 == 0) storage[i]->alive = false;
                    ++k;
                }
            }
            Benchmark::endLo();

            Benchmark::start("refresh");
            eraseRemoveIf(storage, [](const UPtr<OV<OSmall>>& mO)
                {
                    return !mO->alive;
                });
            Benchmark::endLo();

            Benchmark::start("add/rem");
            for(auto k(0u); k < 3; ++k)
            {
                for(auto j(0u); j < 10000; ++j)
                {
                    auto& u(getEmplaceUPtr<OV<OSmall>>(storage));
                    u.alive = false;
                }

                eraseRemoveIf(storage, [](const UPtr<OV<OSmall>>& mO)
                    {
                        return !mO->alive;
                    });
            }
            Benchmark::endLo();

            Benchmark::start("iteration");
            for(auto& o : storage) o->a();
            Benchmark::endLo();

            Benchmark::start("cleanup");
        }

        Benchmark::endLo();
    }
    Benchmark::endLo();

    ssvu::lo() << "" << std::endl;

    {
        Benchmark::start("AtomM - small");
        {
            Benchmark::start("creation");
            HandleVector<OSmall> storage;
            storage.reserve(s + 10000);
            Benchmark::endLo();

            Benchmark::start("filling");
            for(auto i(0u); i < s; ++i) storage.create();
            Benchmark::endLo();

            Benchmark::start("refresh");
            storage.refresh();
            Benchmark::endLo();

            Benchmark::start("iteration");
            storage.forEach([](OSmall& mO)
                {
                    mO.a();
                });
            Benchmark::endLo();

            Benchmark::start("set dead");
            {
                int k{0};
                storage.forEachAtom([k](decltype(storage)::Atom& mO) mutable
                    {
                        if(k % 3 == 0) mO.setDead();
                        ++k;
                    });
            }
            Benchmark::endLo();

            Benchmark::start("refresh");
            storage.refresh();
            Benchmark::endLo();

            Benchmark::start("add/rem");
            for(auto k(0u); k < 3; ++k)
            {
                for(auto j(0u); j < 10000; ++j)
                {
                    auto h(storage.create());
                    h.destroy();
                }

                storage.refresh();
            }
            Benchmark::endLo();

            Benchmark::start("iteration");
            storage.forEach([](OSmall& mO)
                {
                    mO.a();
                });
            Benchmark::endLo();

            Benchmark::start("cleanup");
        }

        Benchmark::endLo();
    }
    Benchmark::endLo();

    ssvu::lo() << "" << std::endl;

    {
        Benchmark::start("MonoManager - small");
        {
            Benchmark::start("creation");
            MonoManager<OSmall> storage;
            storage.reserve(s + 10000);
            Benchmark::endLo();

            Benchmark::start("filling");
            for(auto i(0u); i < s; ++i) storage.create();
            Benchmark::endLo();

            Benchmark::start("refresh");
            storage.refresh();
            Benchmark::endLo();

            Benchmark::start("iteration");
            for(auto& i : storage) i->a();
            Benchmark::endLo();

            Benchmark::start("set dead");
            {
                int k{0};
                for(auto& i : storage)
                {
                    if(k++ % 3 == 0) storage.del(*i);
                }
            }
            Benchmark::endLo();

            Benchmark::start("refresh");
            storage.refresh();
            Benchmark::endLo();

            Benchmark::start("add/rem");
            for(auto k(0u); k < 3; ++k)
            {
                for(auto j(0u); j < 10000; ++j)
                {
                    auto& h(storage.create());
                    storage.del(h);
                }

                storage.refresh();
            }
            Benchmark::endLo();

            Benchmark::start("iteration");
            for(auto& i : storage) i->a();
            Benchmark::endLo();

            Benchmark::start("cleanup");
        }

        Benchmark::endLo();
    }
    Benchmark::endLo();

    ssvu::lo() << "" << std::endl;

    {
        Benchmark::start("VecUPtr - big");
        {
            Benchmark::start("creation");
            VecUPtr<OV<OBig>> storage;
            storage.reserve(s + 10000);
            Benchmark::endLo();

            Benchmark::start("filling");
            for(auto i(0u); i < s; ++i) storage.emplace_back(new OV<OBig>);
            Benchmark::endLo();

            Benchmark::start("iteration");
            for(auto& o : storage) o->a();
            Benchmark::endLo();

            Benchmark::start("set dead");
            {
                int k{0};
                for(auto i(0u); i < s; ++i)
                {
                    if(k % 3 == 0) storage[i]->alive = false;
                    ++k;
                }
            }
            Benchmark::endLo();

            Benchmark::start("refresh");
            eraseRemoveIf(storage, [](const UPtr<OV<OBig>>& mO)
                {
                    return !mO->alive;
                });
            Benchmark::endLo();

            Benchmark::start("add/rem");
            for(auto k(0u); k < 3; ++k)
            {
                for(auto j(0u); j < 10000; ++j)
                {
                    auto& u(getEmplaceUPtr<OV<OBig>>(storage));
                    u.alive = false;
                }

                eraseRemoveIf(storage, [](const UPtr<OV<OBig>>& mO)
                    {
                        return !mO->alive;
                    });
            }
            Benchmark::endLo();

            Benchmark::start("cleanup");
        }

        Benchmark::endLo();
    }
    Benchmark::endLo();

    ssvu::lo() << "" << std::endl;

    {
        Benchmark::start("AtomM - big");
        {
            Benchmark::start("creation");
            HandleVector<OBig> storage;
            storage.reserve(s + 10000);
            Benchmark::endLo();

            Benchmark::start("filling");
            for(auto i(0u); i < s; ++i) storage.create();
            Benchmark::endLo();

            Benchmark::start("refresh");
            storage.refresh();
            Benchmark::endLo();

            Benchmark::start("iteration");
            storage.forEach([](OBig& mO)
                {
                    mO.a();
                });
            Benchmark::endLo();

            Benchmark::start("set dead");
            {
                int k{0};
                storage.forEachAtom([k](decltype(storage)::Atom& mO) mutable
                    {
                        if(k % 3 == 0) mO.setDead();
                        ++k;
                    });
            }
            Benchmark::endLo();

            Benchmark::start("refresh");
            storage.refresh();
            Benchmark::endLo();

            Benchmark::start("add/rem");
            for(auto k(0u); k < 3; ++k)
            {
                for(auto j(0u); j < 10000; ++j)
                {
                    auto h(storage.create());
                    h.destroy();
                }

                storage.refresh();
            }
            Benchmark::endLo();

            Benchmark::start("cleanup");
        }

        Benchmark::endLo();
    }
    Benchmark::endLo();

    ssvu::lo() << "" << std::endl;

    {
        Benchmark::start("MonoManager - big");
        {
            Benchmark::start("creation");
            MonoManager<OBig> storage;
            storage.reserve(s + 10000);
            Benchmark::endLo();

            Benchmark::start("filling");
            for(auto i(0u); i < s; ++i) storage.create();
            Benchmark::endLo();

            Benchmark::start("refresh");
            storage.refresh();
            Benchmark::endLo();

            Benchmark::start("iteration");
            for(auto& i : storage) i->a();
            Benchmark::endLo();

            Benchmark::start("set dead");
            {
                int k{0};
                for(auto& i : storage)
                {
                    if(k++ % 3 == 0) storage.del(*i);
                }
            }
            Benchmark::endLo();

            Benchmark::start("refresh");
            storage.refresh();
            Benchmark::endLo();

            Benchmark::start("add/rem");
            for(auto k(0u); k < 3; ++k)
            {
                for(auto j(0u); j < 10000; ++j)
                {
                    auto& h(storage.create());
                    storage.del(h);
                }

                storage.refresh();
            }
            Benchmark::endLo();

            Benchmark::start("iteration");
            for(auto& i : storage) i->a();
            Benchmark::endLo();

            Benchmark::start("cleanup");
        }

        Benchmark::endLo();
    }
    Benchmark::endLo();

    ssvu::lo() << "" << std::endl;
}

int main()
{
    SSVUT_RUN();
    doBench();
    return 0;
}