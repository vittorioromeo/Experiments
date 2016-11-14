#include <atomic>
#include <chrono>
#include <cstdio>
#include <ecst/thread_pool.hpp>
#include <ecst/utils.hpp>
#include <experimental/tuple>
#include <functional>
#include <thread>
#include <tuple>

namespace ll
{
    struct pool
    {
        template <typename TF>
        void post(TF&& f)
        {
            f();
        }
    };

    inline void sleep_ms(int ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    template <typename T>
    inline void print_sleep_ms(int ms, const T& x)
    {
        std::puts(x);
        sleep_ms(ms);
    }

    template <typename TTuple, typename TF>
    void for_tuple(TTuple&& t, TF&& f)
    {
        std::experimental::apply([&f](auto&&... xs) { (f(xs), ...); }, t);
    }

    struct context
    {
        pool& _p;
        context(pool& p) : _p{p}
        {
        }

        template <typename TF>
        auto build(TF f);
    };

    struct base_node
    {
        context& _ctx;
        base_node(context& ctx) : _ctx{ctx}
        {
        }
    };

    struct root : base_node
    {
        using base_node::base_node;

        template <typename TNode, typename... TNodes>
        void start(TNode& n, TNodes&... ns)
        {
            n.execute(ns...);
        }
    };

    template <typename TParent, typename TF>
    struct node_then : base_node
    {
        using this_type = node_then<TParent, TF>;

        TParent _p;
        TF _f;

        node_then(context& ctx, TParent&& p, TF&& f)
            : base_node{ctx}, _p{std::move(p)}, _f{f}
        {
        }

        auto execute()
        {
            _ctx._p.post(_f);
        }

        template <typename TNode, typename... TNodes>
        auto execute(TNode& n, TNodes&... ns)
        {
            _ctx._p.post([&] {
                _f();
                _ctx._p.post([&] { n.execute(ns...); });
            });
        }

        template <typename TCont>
        auto then(TCont cont)
        {
            return node_then<this_type, TCont>{
                this->_ctx, std::move(*this), std::move(cont)};
        }

        template <typename... TConts>
        auto wait_all(TConts... cont);

        template <typename... TNodes>
        auto start(TNodes&... ns)
        {
            _p.start(*this, ns...);
        }
    };

    template <typename T>
    struct movable_atomic : std::atomic<T>
    {
        using base_type = std::atomic<T>;
        using base_type::base_type;

        movable_atomic(movable_atomic&& r) : base_type{r.load()}
        {
        }

        movable_atomic& operator=(movable_atomic&& r)
        {
            static_cast<base_type&>(*this).store(r.load());
            return *this;
        }
    };

    template <typename TParent, typename... TFs>
    struct node_wait_all : base_node
    {
        using this_type = node_wait_all<TParent, TFs...>;

        TParent _p;
        std::tuple<TFs...> _fs;
        movable_atomic<int> _ctr{sizeof...(TFs)};

        node_wait_all(context& ctx, TParent&& p, TFs&&... fs)
            : base_node{ctx}, _p{std::move(p)}, _fs{fs...}
        {
        }

        auto execute()
        {
            for_tuple(_fs, [&](auto&& f) {
                _ctx._p.post([&] {
                    f();
                    --_ctr;
                });
            });
        }

        template <typename TNode, typename... TNodes>
        auto execute(TNode& n, TNodes&... ns)
        {
            for_tuple(_fs, [&](auto&& f) {
                _ctx._p.post([&] {
                    f();
                    if(--_ctr == 0)
                    {
                        n.execute(ns...);
                    }
                });
            });
        }

        template <typename TCont>
        auto then(TCont cont)
        {
            return node_then<this_type, TCont>{
                this->_ctx, std::move(*this), std::move(cont)};
        }

        template <typename... TNodes>
        auto start(TNodes&... ns)
        {
            _p.start(*this, ns...);
        }
    };

    template <typename TParent, typename TF>
    template <typename... TConts>
    auto node_then<TParent, TF>::wait_all(TConts... conts)
    {
        return node_wait_all<this_type, TConts...>{
            this->_ctx, std::move(*this), std::move(conts)...};
    }

    template <typename TF>
    auto context::build(TF f)
    {
        return node_then<root, TF>(*this, root{*this}, std::move(f));
    }
}
int main()
{
    ll::pool p;
    ll::context ctx{p};

    ctx.build([] { ll::print_sleep_ms(250, "A"); })
        .then([] { ll::print_sleep_ms(250, "B"); })
        .wait_all([] { ll::print_sleep_ms(250, "C0"); },
            [] { ll::print_sleep_ms(250, "C1"); },
            [] { ll::print_sleep_ms(250, "C2"); })
        .then([] { ll::print_sleep_ms(250, "D"); })
        .start();

    ll::sleep_ms(1000);
}
