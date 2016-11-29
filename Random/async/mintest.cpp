

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

#include "nothing.hpp"
#include "perfect_capture.hpp"
#include <ecst/thread_pool.hpp>
#include <ecst/utils.hpp>
#include <experimental/tuple>
#include <functional>
#include <thread>
#include <tuple>

#define IF_CONSTEXPR \
    if               \
    constexpr

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

namespace ll
{
    using vrm::core::forward_like;
    using vrm::core::copy_if_rvalue;
    using vrm::core::for_args;
    using vrm::core::for_args_data;
    using vrm::core::int_v;

    namespace result
    {
        template <typename... Ts>
        using t = std::tuple<void_to_nothing_t<Ts>...>;

        using none_t = t<>;

        constexpr none_t none{};

        template <typename... Ts>
        using of_apply = decltype(apply_ignore_nothing(std::declval<Ts>()...));
    }

    template <typename, typename>
    class node_then;

    template <typename, typename...>
    class node_wait_all;

    // Continuations between nodes are implemented as free functions in order to
    // avoid code repetition.
    // Node types will provide ref-qualified member functions that call the free
    // functions.
    namespace continuation
    {
        template <typename TContinuable, typename... TConts>
        auto then(TContinuable&& c, TConts&&... conts);
    }

    // CRTP base class for all nodes that provide continuation member functions.
    template <typename TDerived>
    class continuable
    {
        // Clang bug prevents this:
        /*
        template <typename TContinuable, typename... TConts>
        friend auto continuation::then(TContinuable&& c, TConts&&... conts);

    protected:
        */
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

        // TODO:
        /*
            auto then_with_context(...);
        */
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
        // TODO: should be protected, friend `build_root` ?
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
        using return_type = result::none_t;
        using base_type::base_type;

    public:
        // TODO: should be `protected`?
        template <typename TNode, typename... TNodes>
        void start(TNode& n, TNodes&... ns)
        {
            n.execute(result::none, ns...);
        }
    };

    template <typename TParent>
    class child_of : protected TParent
    {
    protected:
        using input_type = typename TParent::return_type;

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

        template <typename, typename...>
        friend class node_wait_all;

        using this_type = node_then<TParent, TF>;
        using context_type = typename TParent::context_type;
        using continuable_type = continuable<node_then<TParent, TF>>;

        friend context_type;

    protected:
        using input_type = typename child_of<TParent>::input_type;
        using return_type = result::t<result::of_apply<TF, input_type>>;

    private:
        auto& as_f() & noexcept
        {
            return static_cast<TF&>(*this);
        }

        template <typename T>
        auto execute(T&& x) &
        {
            // `fwd_capture` is used to preserve "lvalue references".
            // TODO: is this post required/beneficial?
            this->post([ this, x = FWD_CAPTURE(x) ]() mutable {
                apply_ignore_nothing(as_f(), forward_like<T>(x.get()));
            });
        }

        template <typename T, typename TNode, typename... TNodes>
        auto execute(T&& x, TNode& n, TNodes&... ns) &
        {
            // `fwd_capture` is used to preserve "lvalue references".
            this->post([ this, x = FWD_CAPTURE(x), &n, &ns... ]() mutable {
                // Take the result value of this function...
                decltype(auto) res_value =
                    apply_ignore_nothing(as_f(), forward_like<T>(x.get()));

                // ...and wrap it into a tuple. Preserve "lvalue references".
                result::t<decltype(res_value)> res(FWD(res_value));

                // TODO: is this post required/beneficial?
                // this->post([ res = std::move(res), &n, &ns... ]() mutable {
                n.execute(std::move(res), ns...);
                // });
            });
        }

    public:
        template <typename TParentFwd, typename TFFwd>
        node_then(TParentFwd&& p, TFFwd&& f)
            : TF{FWD(f)}, child_of<TParent>{FWD(p)}
        {
        }

        // Disambiguate with parent nodes' `then` member functions.
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
        static_assert(
            sizeof...(conts) > 0, "can't build a chain of empty continuables");

        using root_type = root<TContext>;

        return node_then<root_type, TConts...>(root_type{ctx}, FWD(conts)...);
    }

    namespace continuation
    {
        template <typename TContinuable, typename... TConts>
        auto then(TContinuable&& c, TConts&&... conts)
        {
            static_assert(sizeof...(conts) > 0,
                "can't build a chain of empty continuables");

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

using pool = ecst::thread_pool;

class my_context : public ll::context_facade<my_context>
{
private:
    pool& _p;

public:
    my_context(pool& p) : _p{p}
    {
    }

    template <typename TF>
    void post(TF&& f)
    {
        _p.post(std::move(f));
    }
};


int main()
{
    pool p;
    my_context ctx{p};

    for(int i = 0; i < 10; ++i)
    {
        ctx.post([i, &ctx] {
            ctx.build([i]() -> int { return i; })
                .then([](int x) { std::printf("%d\n", x); })
                .start();

            sleep_ms(5);
        });
    }

    sleep_ms(200);
}
