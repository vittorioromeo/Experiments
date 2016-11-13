#include <atomic>
#include <ecst/thread_pool.hpp>
#include <ecst/utils.hpp>
#include <functional>

using pool = ecst::thread_pool;

/*template <typename TF>
auto synchronize(TF f)
{
}


template <typename TF>
struct async_latch_and_continuation
{
    std::atomic<std::size_t> _counter;
    TF _continuation;
};*/

struct async_control
{
    pool& _p;

    async_control(pool& p) : _p{p}
    {
    }
};

template <typename TParent>
struct async_node
{
    async_control& _ac;
    TParent _parent;

    template <typename TFwdParent>
    async_node(async_control& ac, TFwdParent&& parent)
        : _ac{ac}, _parent(FWD(parent))
    {
    }
};

namespace asyncbb
{
    template <typename...>
    using int_t = int;

    struct root
    {
        template <typename T, typename... Ts>
        void start(T& x, Ts&... xs)
        {
            x.build_cont(xs...)();

            // vrm::core::static_if(sizeof...(xs) > 0)
            //  .then([](auto) {})(int_t<T>{0});
        }
    };

    struct ctr : std::atomic<int>
    {
        using bt = std::atomic<int>;
        ctr(int x) : bt{x}
        {
        }

        ctr(const ctr&) = delete;
        ctr& operator=(const ctr&) = delete;

        ctr(ctr&& rhs) : bt{int(rhs)}
        {
        }

        ctr& operator=(ctr&& rhs)
        {
            static_cast<bt&>(*this) = int(rhs);
            return *this;
        }
    };

    template <typename TParent, typename... TFs>
    struct x_when_all : async_node<TParent>
    {
        ctr _ctr{sizeof...(TFs)};

        x_when_all(const x_when_all&) = delete;
        x_when_all(x_when_all&&) = default;

        template <typename... Ts>
        void start(Ts&... xs)
        {
            this->_parent.start(*this, xs...);
        }

        template <typename T, typename... Ts>
        auto build_cont(T&& cx, Ts&&... cxs)
        {
            return [&] {
                vrm::core::for_tuple(
                    [&](auto&& x) {
                        this->_ac._p.post([&] {
                            x();
                            --_ctr;
                            if(_ctr == 0)
                            {
                                cx.build_cont(cxs...)();
                            }
                        });
                    },
                    _fs);
            };
        }

        using this_type = x_when_all<TParent, TFs...>;

        std::tuple<TFs...> _fs;

        template <typename TFwdParent, typename... TFFwds>
        x_when_all(async_control& ac, TFwdParent&& parent, TFFwds&&... fs);

        template <typename TFCont>
        auto then(TFCont&& f_cont);
    };

    template <typename TParent, typename TF>
    struct x_then : async_node<TParent>
    {
        x_then(const x_then&) = delete;
        x_then(x_then&&) = default;

        template <typename... Ts>
        void start(Ts&... xs)
        {
            this->_parent.start(*this, xs...);
        }

        auto build_cont()
        {
            return [&] { this->_ac._p.post(_f); };
        }

        template <typename T, typename... Ts>
        auto build_cont(T&& x, Ts&&... xs)
        {
            return [&] {
                this->_ac._p.post([&] {
                    _f();
                    x.build_cont(FWD(xs)...)();
                });

            };
        }

        using this_type = x_then<TParent, TF>;

        TF _f;

        template <typename TFwdParent, typename TFFwd>
        x_then(async_control& ac, TFwdParent&& parent, TFFwd&& f)
            : async_node<TParent>{ac, FWD(parent)}, _f{FWD(f)}
        {
        }

        template <typename TFCont>
        auto then(TFCont&& f_cont)
        {
            return x_then<this_type, std::decay_t<TFCont>>{
                this->_ac, std::move(*this), FWD(f_cont)};
        }

        template <typename... TFConts>
        auto when_all(TFConts&&... f_conts)
        {
            return x_when_all<this_type, std::decay_t<TFConts>...>{
                this->_ac, *this, FWD(f_conts)...};
        }
    };

    template <typename TParent, typename... TFs>
    template <typename TFwdParent, typename... TFFwds>
    x_when_all<TParent, TFs...>::x_when_all(
        async_control& ac, TFwdParent&& parent, TFFwds&&... fs)
        : async_node<TParent>{ac, FWD(parent)}, _fs{FWD(fs)...}
    {
    }

    template <typename TParent, typename... TFs>
    template <typename TFCont>
    auto x_when_all<TParent, TFs...>::then(TFCont&& f_cont)
    {
        return x_then<this_type, std::decay_t<TFCont>>{
            this->_ac, std::move(*this), FWD(f_cont)};
    }
}

template <typename TParent>
struct async_builder : async_node<TParent>
{
    using async_node<TParent>::async_node;

    template <typename... TFs>
    auto when_all(TFs&&... fs)
    {
        return asyncbb::x_when_all<TParent, std::decay_t<TFs>...>{
            this->_ac, this->_parent, FWD(fs)...};
    }

    template <typename TF>
    auto then(TF&& f)
    {
        return asyncbb::x_then<TParent, std::decay_t<TF>>{
            this->_ac, this->_parent, FWD(f)};
    }
};

class async_context
{
private:
    async_control _ac;

public:
    async_context(pool& p) : _ac{p}
    {
    }

    auto prepare()
    {
        return async_builder<asyncbb::root>{_ac, asyncbb::root{}};
    }
};

int main()
{
    using namespace std::literals;

    pool p;
    async_context ac{p};

    auto whenalltest = [](auto x) {
        return [x] {
            std::cout << x << " start\n";
            std::this_thread::sleep_for(500ms);
            std::cout << x << " end\n";
        };
    };

    auto computation =
        ac.prepare()
            .when_all(whenalltest("WA"), whenalltest("WB"), whenalltest("WC"))
            .then([] { std::cout << "A\n"; })
            .then([] { std::cout << "B\n"; });
    //            .then([] { std::cout << "B\n"; })
    //            .then([] { std::cout << "C\n"; });

    computation.start();

    std::this_thread::sleep_for(1000ms);
}
