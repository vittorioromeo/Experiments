

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

namespace ll
{
    using vrm::core::forward_like;
    using vrm::core::copy_if_rvalue;
    using vrm::core::for_args;
    using vrm::core::for_args_data;
    using vrm::core::int_v;

#if 0
    using pool = ecst::thread_pool;
#else
    struct pool
    {
        template <typename TF>
        void post(TF&& f)
        {
            f();
        }
    };
#endif

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
            // `*this` is always an "lvalue expression" - the code below does
            // not introduce infinite recursion.
            return std::move(this->as_derived());
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

    class context
    {
    public:
        pool& _p;

        context(pool& p) : _p{p}
        {
        }

        template <typename TF>
        auto build(TF&& f);
    };

    class base_node
    {
    private:
        friend class context;
        context& _ctx;

    protected:
        base_node(context& ctx) noexcept : _ctx{ctx}
        {
        }

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
            this->ctx()._p.post(FWD(f));
        }
    };

    template <typename TReturn>
    class root : protected base_node
    {
    private:
        friend class context;

    protected:
        using return_type = std::tuple<>;
        using base_node::base_node;

    public:
        // TODO: should be `protected`?
        template <typename TNode, typename... TNodes>
        void start(TNode& n, TNodes&... ns)
        {
            n.execute(std::make_tuple(nothing), ns...);
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
        friend class context;

        template <typename>
        friend class root;

        template <typename, typename>
        friend class node_then;

        template <typename, typename...>
        friend class node_wait_all;

        using this_type = node_then<TParent, TF>;
        using continuable_type = continuable<node_then<TParent, TF>>;

    protected:
        // TODO: pls cleanup
        using input_type = typename child_of<TParent>::input_type;
        using return_type = std::tuple<decltype(apply_ignore_nothing(
            std::declval<TF>(), std::declval<input_type>()))>;

        // TODO: might be useful?
        // using input_type = void_to_nothing_t<typename TParent::return_type>;
        // using return_type =
        //    std::tuple<void_to_nothing_t<decltype(apply_ignore_nothing(
        //        std::declval<TF>(), std::declval<input_type>()))>>;


    private:
        auto& as_f() noexcept
        {
            return static_cast<TF&>(*this);
        }

        template <typename T>
        auto execute(T&& x) &
        {
            // `fwd_capture` is used to preserve "lvalue references".
            this->post([ this, x = fwd_capture(FWD(x)) ]() mutable {
                apply_ignore_nothing(as_f(), forward_like<T>(x.get()));
            });
        }

        template <typename T, typename TNode, typename... TNodes>
        auto execute(T&& x, TNode& n, TNodes&... ns) &
        {
            // `fwd_capture` is used to preserve "lvalue references".
            this->post([ this, x = fwd_capture(FWD(x)), &n, &ns... ]() mutable {
                // Take the result value of this function...
                decltype(auto) res_value =
                    apply_ignore_nothing(as_f(), forward_like<T>(x.get()));

                // ...and wrap it into a tuple. Preserve "lvalue references".
                std::tuple<decltype(res_value)> res(FWD(res_value));

                this->post([ res = std::move(res), &n, &ns... ]() mutable {
                    n.execute(std::move(res), ns...);
                });
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

    template <typename T>
    struct movable_atomic : std::atomic<T>
    {
        using base_type = std::atomic<T>;
        using base_type::base_type;

        movable_atomic(movable_atomic&& r) noexcept : base_type{r.load()}
        {
        }

        movable_atomic& operator=(movable_atomic&& r) noexcept
        {
            this->store(r.load());
            return *this;
        }
    };

    template <typename TParent, typename... TFs>
    class node_wait_all : private TFs...,
                          protected child_of<TParent>,
                          public continuable<node_wait_all<TParent, TFs...>>

    {
        friend class context;

        template <typename>
        friend class root;

        template <typename, typename>
        friend class node_then;

        template <typename, typename...>
        friend class node_wait_all;

        // using input_type = void_to_nothing_t<typename TParent::return_type>;
        // using return_type =
        //    std::tuple<void_to_nothing_t<decltype(apply_ignore_nothing(
        //        std::declval<TFs>(), std::declval<input_type>()))...>>;


        using this_type = node_wait_all<TParent, TFs...>;
        using continuable_type = continuable<node_wait_all<TParent, TFs...>>;

    protected:
        // TODO: pls cleanup
        // TODO: not sure why remove_const is needed, was getting `const
        // nothing_t`
        using input_type = typename child_of<TParent>::input_type;
        using return_type =
            std::tuple<std::remove_const_t<decltype(apply_ignore_nothing(
                std::declval<TFs>(), std::declval<input_type>()))>...>;


    private:
        movable_atomic<int> _ctr{sizeof...(TFs)};

        // TODO: should use EBO
        // TODO: should add padding between members to avoid false sharing
        return_type _results;


        template <typename T>
        auto execute(T&& x) &
        {
            auto exec =
                // `fwd_capture` the argument, forcing a copy instead of a move.
                [ this, x = fwd_copy_capture(FWD(x)) ](auto, auto& f) mutable
            {
                // `x` is now a `perfect_capture` wrapper, so it can be moved.
                // The original `x` has already been copied.
                this->post([ this, x = std::move(x), &f ]() mutable {

                    // Should either propagate an "lvalue reference" to the
                    // original `x` or move the copy of `x` that was made in
                    // `exec`.
                    apply_ignore_nothing(f, forward_like<T>(x.get()));
                });
            };

            for_args_data([&exec](auto _,
                              auto& f) { exec(int_v<decltype(_)::index>, f); },
                static_cast<TFs&>(*this)...);
        }

        template <typename T, typename TNode, typename... TNodes>
        auto execute(T&& x, TNode& n, TNodes&... ns) &
        {
            // TODO: figure out lifetime of `x`!
            auto exec =
                // `fwd_capture` the argument, forcing a copy instead of a move.
                [ this, x = fwd_copy_capture(FWD(x)), &n, &ns... ](
                    auto idx, auto& f)
            {
                // `x` is now a `perfect_capture` wrapper, so it can be moved.
                // The original `x` has already been copied.
                this->post([ this, x = std::move(x), &n, &ns..., &f ] {

                    // Should either propagate an "lvalue reference" to the
                    // original `x` or move the copy of `x` that was made in
                    // `exec`.
                    std::get<decltype(idx){}>(_results) =
                        apply_ignore_nothing(f, forward_like<T>(x.get()));

                    // If this is the last `TFs...` function to end, trigger the
                    // next continuation.
                    if(--_ctr == 0)
                    {
                        n.execute(std::move(_results), ns...);
                    }
                });
            };

            for_args_data([&exec](auto _,
                              auto& f) { exec(int_v<decltype(_)::index>, f); },
                static_cast<TFs&>(*this)...);
        }

    public:
        template <typename TParentFwd, typename... TFFwds>
        node_wait_all(TParentFwd&& p, TFFwds&&... fs)
            : TFs{FWD(fs)}..., child_of<TParent>{FWD(p)}
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

    template <typename TF>
    auto context::build(TF&& f)
    {
        using return_type = decltype(f());
        using root_type = root<return_type>;
        return node_then<root_type, TF>(root_type{*this}, FWD(f));
    }

    namespace continuation
    {
        template <typename TContinuable, typename... TConts>
        auto then(TContinuable&& c, TConts&&... conts)
        {
            using c_type = typename TContinuable::this_type;

            IF_CONSTEXPR(sizeof...(conts) == 0)
            {
                static_assert("u wot m8");
            }
            else IF_CONSTEXPR(sizeof...(conts) == 1)
            {
                return node_then<c_type, TConts...>{
                    FWD(c).as_derived(), FWD(conts)...};
            }
            else
            {
                return node_wait_all<c_type, TConts...>{
                    FWD(c).as_derived(), FWD(conts)...};
            }
        }
    }
}

template <typename T>
void execute_after_move(T x)
{
    x.start();
    ll::sleep_ms(200);
}

struct nocopy
{
    nocopy() = default;

    nocopy(const nocopy&) = delete;
    nocopy& operator=(const nocopy&) = delete;

    nocopy(nocopy&&) = default;
    nocopy& operator=(nocopy&&) = default;
};

int main()
{
    ll::pool p;
    ll::context ctx{p};

    auto computation =               // .
        ctx.build([] { return 10; }) // .
            .then([](int x) { return std::to_string(x); })
            .then([](std::string x) { return "num: " + x; })
            .then([](std::string x) { std::printf("%s\n", x.c_str()); });

    ctx.build([] { return 10; })
        .then([](int x) { return std::to_string(x); })
        .then([](std::string x) { return "num: " + x; })
        .then([](std::string x) { std::printf("%s\n", x.c_str()); })
        .start();

    // with moveonly
    ctx.build([] { return nocopy{}; }).then([](nocopy) {}).start();

    // with lvalue
    int aaa = 10;
    ctx.build([&aaa]() -> int& { return aaa; })
        .then([](int& x) { return std::to_string(x); })
        .then([](std::string x) { return "num: " + x; })
        .then([](std::string x) { std::printf("%s\n", x.c_str()); })
        .start();

    // when_all
    ctx.build([] { return 5; })
        .then([](int y) { std::printf(">>%d\n", y); },
            [](int y) { std::printf(">>%d\n", y); })
        .start();

    // when_all with lvalue
    int aaa2 = 10;
    ctx.build([&aaa2]() -> int& { return aaa2; })
        .then([](int& y) { std::printf(">>%d\n", y); },
            [](int& y) { std::printf(">>%d\n", y); })
        .start();

    // when_all with atomic lvalue
    std::atomic<int> aint{0};
    ctx.build([&aint]() -> std::atomic<int>& { return aint; })
        .then([](auto& y) { y += 5; }, [](auto& y) { y += 5; })
        .then([&aint] {
            std::printf("AINT: %d\n", aint.load());
            assert(aint == 10);
        })
        .start();

    // when_all returns
    ctx.build([] { return 5; })
        .then([](int y) { return y + 1; }, [](int y) { return y + 2; })
        .then([](int z0, int z1) {
            assert(z0 == 6);
            assert(z1 == 7);
        })
        .start();

    // when_all returns lvalues
    {
        int lv0 = 0;
        int lv1 = 0;
        ctx.build([&lv0, &lv1]() -> std::tuple<int&, int&> {
               return {lv0, lv1};
           })
            .then(
                [](auto y) -> int& {
                    std::get<0>(y) += 1;
                    return std::get<0>(y);
                },
                [](auto y) -> int& {
                    std::get<1>(y) += 2;
                    return std::get<1>(y);
                })
            .then([&lv0, &lv1](int& z0, int& z1) {
                assert(z0 == 1);
                assert(z1 == 2);
                assert(lv0 == 1);
                assert(lv1 == 2);
            })
            .start();
    }

    /* TODO:
        ctx.build<int>([] { return 10; })
            .when_all<int, float>
            (
                // TODO: int? or const int&?
                // *(should the arg be copied across every task?)*
                [](int x) { return x + 1; },
                [](int x) { return x + 1.f; }
            )
            .then<void>([](std::tuple<int, float> x) { std::printf("%s\n",
       x.c_str()); })
            .start();
    */

    /* TODO:
        ctx.build<int>([] { return 10; })
            .when_any<int, float>
            (
                [](int x) { return x + 1; },
                [](int x) { return x + 1.f; }
            )
            .then<void>([](std::variant<int, float> x) { std::printf("%s\n",
       x.c_str()); })
            .start();
    */

    /* TODO:
        ctx.build<int>([] { return 10; })
            .then<int>([](int x) { return std::to_string(x); })
            .then_timeout<std::string>(200ms, [](std::string x) { return "num: "
       + x; })
            .then<void>([](std::optional<std::string> x) { std::printf("%s\n",
       x.c_str()); })
            .start();
    */

    // TODO: deduce return type when possible!
    // (generalize stuff from variant articles)
    // (generic lambdas are probably not deducible)
    /*
        if arity is deducible
            if arity is 0
                DEDUCE void
            else if arity is 1
                DEDUCE with function traits
    */

    execute_after_move(std::move(computation));
    ll::sleep_ms(200);
}
