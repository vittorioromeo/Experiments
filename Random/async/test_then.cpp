#include  "shared.hpp"

namespace ll
{
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
       .then([] { ll::print_sleep_ms(250, "C"); })
       .then([] { ll::print_sleep_ms(250, "D"); })
       .then([] { ll::print_sleep_ms(250, "E"); })
       .start();

    int a;
    std::cin >> a;
}
