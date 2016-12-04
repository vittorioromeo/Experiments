#include "shared.hpp"

namespace ll
{
    template <typename TTuple, typename TF>
    void for_tuple(TTuple& t, TF&& f)
    {
        std::experimental::apply(
            [&f](auto&&... xs) { (f(std::forward<decltype(xs)>(xs)), ...); },
            t);
    }

    struct context
    {
        pool& _p;
        context(pool& p) : _p{p}
        {
        }

        template <typename TF>
        auto build(TF&& f);
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
            : base_node{ctx}, _p{std::move(p)}, _f{std::move(f)}
        {
        }

        auto execute()
        {
            _f();
        }

        template <typename TNode, typename... TNodes>
        auto execute(TNode& n, TNodes&... ns)
        {
            _ctx._p.post([&] {
                _f();
                n.execute(ns...);
            });
        }

        template <typename TCont>
        auto then(TCont&& cont)
        {
            auto& ctxref = this->_ctx;
            return node_then<this_type, TCont>{ctxref, std::move(*this),
                                               std::move(cont)};
        }

        template <typename... TConts>
        auto wait_all(TConts&&... cont);

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
            : base_node{ctx}, _p{std::move(p)}, _fs{std::move(fs)...}
        {
        }

        template <typename TNode, typename... TNodes>
        auto execute(TNode& n, TNodes&... ns)
        {
  /*          auto f = [&](auto x) {
                _ctx._p.post([&] {
                    std::get<decltype(x){}>(_fs)();
                    // std::cout << _ctr << std::endl;
                    if(--_ctr == 0)
                    {
                        n.execute(ns...);
                    }
                });
            };

            f(std::integral_constant<int, 0>{});

*/

_ctx._p.post([&] {
                            std::get<0>(_fs)();
                            std::cout << _ctr << std::endl;
                            if(--_ctr == 0)
                            {
                                n.execute(ns...);
                            }
                        });

            // f(std::integral_constant<int, 1>{});

            // for_tuple(_fs, f);
            /*
                        _ctx._p.post([&] {
                            std::get<0>(_fs)();
                            std::cout << _ctr << std::endl;
                            if(--_ctr == 0)
                            {
                                n.execute(ns...);
                            }
                        });

                        _ctx._p.post([&] {
                            std::get<1>(_fs)();
                            std::cout << _ctr << std::endl;
                            if(--_ctr == 0)
                            {
                                n.execute(ns...);
                            }
                        });*/
        }

        template <typename TCont>
        auto then(TCont&& cont)
        {
            auto& ctxref = this->_ctx;
            return node_then<this_type, TCont>{ctxref, std::move(*this),
                                               std::move(cont)};
        }

        template <typename... TNodes>
        auto start(TNodes&... ns)
        {
            _p.start(*this, ns...);
        }
    };

    template <typename TParent, typename TF>
    template <typename... TConts>
    auto node_then<TParent, TF>::wait_all(TConts&&... conts)
    {
        auto& ctxref = this->_ctx;
        return node_wait_all<this_type, TConts...>{ctxref, std::move(*this),
                                                   std::move(conts)...};
    }

    template <typename TF>
    auto context::build(TF&& f)
    {
        return node_then<root, TF>(*this, root{*this}, std::move(f));
    }
}

int main()
{
    ll::pool p;
    ll::context ctx{p};

    ctx.build([] { ll::print_sleep_ms(25, "A"); })
        .wait_all([] { ll::print_sleep_ms(25, "C0"); })
        .then([] { ll::print_sleep_ms(25, "B"); })
        .wait_all([] { ll::print_sleep_ms(25, "C0"); })
        .then([] { ll::print_sleep_ms(25, "B"); })
        .start();
    /*
        ctx.build([] { ll::print_sleep_ms(25, "A"); })
            .then([] { ll::print_sleep_ms(25, "B"); })
            .wait_all([] { ll::print_sleep_ms(25, "C0"); },
                      [] { ll::print_sleep_ms(25, "C1"); })
            .then([] { ll::print_sleep_ms(25, "D"); })
            .wait_all([] { ll::print_sleep_ms(25, "E0"); },
                      [] { ll::print_sleep_ms(25, "E1"); })
            .then([] { ll::print_sleep_ms(25, "F"); })
            .wait_all([] { ll::print_sleep_ms(25, "G0"); },
                      [] { ll::print_sleep_ms(25, "G1"); })
            .then([] { ll::print_sleep_ms(25, "H"); })
            .start();
    */
    int a;
    std::cin >> a;
}
