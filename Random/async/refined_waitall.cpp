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
    using pool = ecst::thread_pool;

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
        std::experimental::apply(
            [&f](auto&&... xs) { (f(FWD(xs)), ...); }, FWD(t));
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
        base_node(context& ctx) noexcept : _ctx{ctx}
        {
        }
    };

    struct root : base_node
    {
        using base_node::base_node;

        template <typename TNode, typename... TNodes>
        void start(TNode& n, TNodes&... ns) &
        {
            n.execute(ns...);
        }

        auto& ctx() & noexcept
        {
            return this->_ctx;
        }

        const auto& ctx() const & noexcept
        {
            return this->_ctx;
        }
    };

    template <typename TParent>
    struct parent_holder
    {
        TParent _p;

        template <typename TParentFwd>
        parent_holder(TParentFwd&& p) : _p{FWD(p)}
        {
        }
    };

    template <typename TParent>
    struct child_of : parent_holder<TParent>
    {
        using parent_holder<TParent>::parent_holder;

        auto& parent() & noexcept
        {
            return this->_p;
        }

        const auto& parent() const & noexcept
        {
            return this->_p;
        }

        auto parent() && noexcept
        {
            return std::move(this->_p);
        }

        auto& ctx() & noexcept
        {
            return this->_p.ctx();
        }

        const auto& ctx() const & noexcept
        {
            return this->_p.ctx();
        }
    };



    template <typename TParent, typename TF>
    struct node_then : child_of<TParent>, TF
    {
        using this_type = node_then<TParent, TF>;

        auto& as_f() noexcept
        {
            return static_cast<TF&>(*this);
        }

        template <typename TParentFwd, typename TFFwd>
        node_then(TParentFwd&& p, TFFwd&& f)
            : child_of<TParent>{FWD(p)}, TF{FWD(f)}
        {
        }

        auto execute() &
        {
            this->ctx()._p.post(as_f());
        }

        template <typename TNode, typename... TNodes>
        auto execute(TNode& n, TNodes&... ns) &
        {
            this->ctx()._p.post([&] {
                as_f()();
                this->ctx()._p.post([&] { n.execute(ns...); });
            });
        }

        template <typename TCont>
        auto then(TCont&& cont) &&
        {
            return node_then<this_type, TCont>{std::move(*this), FWD(cont)};
        }

        template <typename... TConts>
        auto wait_all(TConts&&... cont) &&;

        template <typename... TNodes>
        auto start(TNodes&... ns) &
        {
            this->parent().start(*this, ns...);
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
    struct node_wait_all : child_of<TParent>, TFs...
    {
        using this_type = node_wait_all<TParent, TFs...>;

        movable_atomic<int> _ctr{sizeof...(TFs)};

        template <typename TParentFwd, typename... TFFwds>
        node_wait_all(TParentFwd&& p, TFFwds&&... fs)
            : child_of<TParent>{FWD(p)}, TFs{FWD(fs)}...
        {
        }

        auto execute() &
        {
            (this->ctx()._p.post([&] { static_cast<TFs&> (*this)(); }), ...);
        }

        template <typename TNode, typename... TNodes>
        auto execute(TNode& n, TNodes&... ns) &
        {
            auto exec = [this, &n, &ns...](auto& f) {
                this->ctx()._p.post([&] {
                    f();
                    if(--_ctr == 0)
                    {
                        n.execute(ns...);
                    }
                });
            };

            (exec(static_cast<TFs&>(*this)), ...);
        }

        template <typename TCont>
        auto then(TCont&& cont) &
        {
            return node_then<this_type&, TCont>{*this, FWD(cont)};
        }

        template <typename TCont>
        auto then(TCont&& cont) &&
        {
            return node_then<this_type, TCont>{std::move(*this), FWD(cont)};
        }

        template <typename... TNodes>
        auto start(TNodes&... ns) &
        {
            this->parent().start(*this, ns...);
        }
    };

    template <typename TParent, typename TF>
    template <typename... TConts>
    auto node_then<TParent, TF>::wait_all(TConts&&... conts) &&
    {
        return node_wait_all<this_type, TConts...>{
            std::move(*this), FWD(conts)...};
    }

    template <typename TF>
    auto context::build(TF&& f)
    {
        return node_then<root, TF>(root{*this}, FWD(f));
    }
}

template <typename T>
void execute_after_move(T x)
{
    x.start();
    ll::sleep_ms(1200);
}

int main()
{
    ll::pool p;
    ll::context ctx{p};

    auto lvalue_comp = ctx.build([] { ll::print_sleep_ms(150, "A"); })
                           .then([] { ll::print_sleep_ms(150, "B"); })
                           .wait_all([] { ll::print_sleep_ms(150, "C0"); },
                               [] { ll::print_sleep_ms(150, "C1"); },
                               [] { ll::print_sleep_ms(150, "C2"); });

std::printf("%lu\n", sizeof(lvalue_comp));

    auto computation = lvalue_comp.then([] { ll::print_sleep_ms(150, "D"); })
                           .wait_all([] { ll::print_sleep_ms(150, "E0"); },
                               [] { ll::print_sleep_ms(150, "E1"); },
                               [] { ll::print_sleep_ms(150, "E2"); })
                           .then([] { ll::print_sleep_ms(150, "F"); });

std::printf("%lu\n", sizeof(computation));
    execute_after_move(std::move(computation));
}
