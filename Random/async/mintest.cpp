

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <mutex>
#include <thread>

#if defined(MINGW)
#include <mingw.condition_variable.h>
#include <mingw.mutex.h>
#include <mingw.thread.h>
#endif

#include <ecst/thread_pool.hpp>
#include <ecst/utils.hpp>
#include <experimental/tuple>
#include <functional>
#include <thread>
#include <tuple>

inline void sleep_ms(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

namespace orizzonte
{
    std::mutex m;

    template <typename, typename>
    class node_then;

    namespace continuation
    {
        template <typename TContinuable, typename... TConts>
        auto then(TContinuable&& c, TConts&&... conts);
    }

    template <typename TDerived>
    class continuable
    {
    public:
        using this_type = TDerived;

        auto& as_derived() & noexcept
        {
            return static_cast<TDerived&>(*this);
        }

        const auto& as_derived() const & noexcept
        {
            return static_cast<TDerived&>(*this);
        }

        auto as_derived() && noexcept
        {
            return std::move(static_cast<TDerived&>(*this));
        }

    public:
        template <typename... TConts>
        auto then(TConts&&... conts) &
        {
            return continuation::then(*this, FWD(conts)...);
        }

        template <typename... TConts>
        auto then(TConts&&... conts) &&
        {
            return continuation::then(std::move(*this), FWD(conts)...);
        }
    };

    template <typename TContext>
    class base_node
    {
    private:
        friend TContext;
        TContext& _ctx;

    protected:
        using context_type = TContext;

    public:
        base_node(TContext& ctx) noexcept : _ctx{ctx}
        {
        }

    protected:
        auto& ctx() & noexcept
        {
            return this->_ctx;
        }

        const auto& ctx() const & noexcept
        {
            return this->_ctx;
        }

        template <typename TF>
        void post(TF&& f)
        {
            this->ctx().post(FWD(f));
        }
    };

    template <typename TContext>
    class root : public base_node<TContext>
    {
    private:
        friend TContext;

    protected:
        using base_type = base_node<TContext>;
        using context_type = typename base_type::context_type;
        using base_type::base_type;

    public:
        template <typename TNode, typename... TNodes>
        void start(TNode& n, TNodes&... ns)
        {
            n.execute(ns...);
        }
    };

    template <typename TParent>
    class child_of : protected TParent
    {
    protected:
        template <typename TParentFwd>
        child_of(TParentFwd&& p) : TParent{FWD(p)}
        {
        }

        auto& parent() & noexcept
        {
            return static_cast<TParent&>(*this);
        }

        const auto& parent() const & noexcept
        {
            return static_cast<TParent&>(*this);
        }

        auto parent() && noexcept
        {
            return std::move(static_cast<TParent&>(*this));
        }
    };

    template <typename TParent, typename TF>
    class node_then : private TF,
                      protected child_of<TParent>,
                      public continuable<node_then<TParent, TF>>
    {
        template <typename>
        friend class root;

        template <typename, typename>
        friend class node_then;

        using this_type = node_then<TParent, TF>;
        using context_type = typename TParent::context_type;
        using continuable_type = continuable<node_then<TParent, TF>>;

        friend context_type;

    private:
        auto& as_f() & noexcept
        {
            return static_cast<TF&>(*this);
        }

        auto execute() &
        {
            this->post([this]() mutable { as_f()(); });
        }

        template <typename TNode, typename... TNodes>
        auto execute(TNode& n, TNodes&... ns) &
        {
            this->post([this, &n, &ns...]() mutable {
                as_f()();

                this->post([&n, &ns...]() mutable { n.execute(ns...); });
            });
        }

    public:
        template <typename TParentFwd, typename TFFwd>
        node_then(TParentFwd&& p, TFFwd&& f)
            : TF{FWD(f)}, child_of<TParent>{FWD(p)}
        {
        }

        using continuable_type::then;

        template <typename... TNodes>
        auto start(TNodes&... ns)
        {
            this->parent().start(*this, ns...);
        }
    };

    template <typename TContext, typename... TConts>
    auto build_root(TContext& ctx, TConts&&... conts)
    {
        using root_type = root<TContext>;
        return node_then<root_type, TConts...>(root_type{ctx}, FWD(conts)...);
    }

    namespace continuation
    {
        template <typename TContinuable, typename... TConts>
        auto then(TContinuable&& c, TConts&&... conts)
        {
            using c_type = typename TContinuable::this_type;
            return node_then<c_type, TConts...>{
                FWD(c).as_derived(), FWD(conts)...};
        }
    }

    template <typename TContext>
    class context_facade
    {
    public:
        template <typename... TConts>
        auto build(TConts&&... conts)
        {
            return ll::build_root(static_cast<TContext&>(*this), FWD(conts)...);
        }
    };
}

struct my_context : public ll::context_facade<my_context>
{
    ecst::thread_pool _p;

    template <typename TF>
    void post(TF&& f)
    {
        // f();
        // std::thread{std::move(f)}.detach();
        _p.post(std::move(f));
    }
};

int main()
{
    my_context ctx;

    for(int i = 0; i < 25; ++i)
    {
        ctx.post([&ctx, i] {
            std::atomic<bool> k{false};

            auto x = ctx.build([y = 1]{}).then([i, &k] {
                std::cout << i << "\n";
                k = true;
            });

            x.start();

            while(!k)
            {
                sleep_ms(5);
            }
        });
    }

    sleep_ms(300);
}
