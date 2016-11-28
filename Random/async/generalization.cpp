

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
        auto& as_f() noexcept
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
        template <typename>
        friend class root;

        template <typename, typename>
        friend class node_then;

        template <typename, typename...>
        friend class node_wait_all;

        using this_type = node_wait_all<TParent, TFs...>;
        using continuable_type = continuable<node_wait_all<TParent, TFs...>>;
        using context_type = typename TParent::context_type;
        friend context_type;

    protected:
        using input_type = typename child_of<TParent>::input_type;
        using return_type = result::t<result::of_apply<TFs, input_type>...>;

    private:
        movable_atomic<int> _ctr{sizeof...(TFs)};

        // TODO: should use EBO
        // TODO: should add padding between members to avoid false sharing
        // TODO: deal with lvalue references. Tuple is non-default-ctor for refs
        return_type _results;

        template <typename T>
        auto execute(T&& x) &
        {
            auto exec =
                // `fwd_capture` the argument, forcing a copy instead of a move.
                [ this, x = FWD_COPY_CAPTURE(x) ](auto, auto& f) mutable
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
            auto exec =
                // `fwd_capture` the argument, forcing a copy instead of a move.
                [ this, x = FWD_COPY_CAPTURE(x), &n, &ns... ](
                    auto idx, auto& f)
            {
                // `x` is now a `perfect_capture` wrapper, so it can be moved.
                // The original `x` has already been copied.
                this->post([ this, x = std::move(x), &n, &ns..., &f ] {
                    // Should either propagate an "lvalue reference" to the
                    // original `x` or move the copy of `x` that was made in
                    // `exec`.
                    decltype(auto) res =
                        apply_ignore_nothing(f, forward_like<T>(x.get()));

                    using decay_res_type = std::decay_t<decltype(res)>;

                    // Don't set results for `void`-returning functions.
                    IF_CONSTEXPR(!std::is_same<decay_res_type, nothing_t>{})
                    {
                        // Should either propagate an "lvalue reference" to
                        // the original `x` or move the copy of `x` that was
                        // made in `exec`.

                        // TODO: race cond?
                        std::get<decltype(idx){}>(_results) =
                            forward_like<T>(res);
                    }

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

    template <typename TContext, typename... TConts>
    auto build_root(TContext& ctx, TConts&&... conts)
    {
        static_assert(sizeof...(conts) > 0, "can't build a chain of empty continuables");

        using root_type = root<TContext>;

        if constexpr(sizeof...(conts) == 1)
        {
            return node_then<root_type, TConts...>(root_type{ctx}, FWD(conts)...);
        }
        else
        {
            return node_wait_all<root_type, TConts...>(root_type{ctx}, FWD(conts)...);
        }
    }


    // "compose" assumes that all inputs are existing continuables
    template <typename TContext, typename ...TConts>
    auto compose(TContext&& ctx, TConts&&... conts)
    {
        static_assert(sizeof...(conts) > 0, "can't compose a chain of empty continuables");

        auto wrap_start = [](auto& c){ return [&c]{ c.start(); }; };
        return ctx.build(wrap_start(conts)...);
    }

    namespace continuation
    {
        template <typename TContinuable, typename... TConts>
        auto then(TContinuable&& c, TConts&&... conts)
        {
            static_assert(sizeof...(conts) > 0, "can't build a chain of empty continuables");

            using c_type = typename TContinuable::this_type;

            IF_CONSTEXPR(sizeof...(conts) == 1)
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

template <typename T>
void execute_after_move(T x)
{
    x.start();
    sleep_ms(200);
}

template<typename TContext, typename ...TChains>
void wait_until_complete(TContext& ctx, TChains&&... chains)
{
    ecst::latch l{1};
    l.execute_and_wait_until_zero([&ctx, &l, chains = FWD_CAPTURE_AS_TUPLE(chains)]() mutable {
        apply_fwd_capture([&ctx](auto&&... xs){ return compose(ctx, FWD(xs)...); }, chains)
            .then([&l]{ l.decrement_and_notify_all(); } )
            .start();
    });
}

struct nocopy
{
    nocopy() = default;

    nocopy(const nocopy&) = delete;
    nocopy& operator=(const nocopy&) = delete;

    nocopy(nocopy&&) = default;
    nocopy& operator=(nocopy&&) = default;
};


#if 1
// TODO: debug gcc asan error with real pool
using pool = ecst::thread_pool;
#endif

#if 0
struct pool
{
    template <typename TF>
    void post(TF&& f)
    {
        f();
    }
};
#endif

#if 0
struct pool
{
    template <typename TF>
    void post(TF&& f)
    {
        // Required because `std::thread` always copies its callable argument.
        auto jptr = std::make_shared<ecst::fixed_function<void()>>([f = std::move(f)]() mutable { f(); });
        std::thread{[jptr]() mutable { (*jptr)(); }}.detach();
    }
};
#endif

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

#if 0
int main()
{
    pool p;
    my_context ctx{p};

    ctx.build([] { return 10; })
        .then([](int x) { return std::to_string(x); })
        .then([](std::string x) { return "num: " + x; })
        .then([](std::string x) { std::printf("%s\n", x.c_str()); })
        .start();

    // with moveonly
    ctx.build([] { return nocopy{}; }).then([](nocopy) {}).start();

    sleep_ms(200);
}
#else
int main()
{
    // TODO: gcc segfault on mingw when set to true.
    constexpr bool run_when_all_tests{false};

    pool p;
    my_context ctx{p};

    auto computation =               // .
        ctx.build([] { return 10; }) // .
            .then([](int x) { return std::to_string(x); })
            .then([](std::string x) { return "num: " + x; })
            .then([](std::string x) { std::printf("%s\n", x.c_str()); });

    for(volatile int i = 0; i < 20; ++i)
    {
        ctx.build([i] { return i; })
            .then([](int x) { return std::to_string(x); })
            .then([](std::string x) { return "inum: " + x; })
            .then([](std::string x) { std::printf("%s\n", x.c_str()); })
            .start();

        // with moveonly
        ctx.build([] { return nocopy{}; }).then([](nocopy) {}).start();
    }

    // with lvalue
    int aaa = 10;
    ctx.build([&aaa]() -> int& { return aaa; })
        .then([](int& x) { return std::to_string(x); })
        .then([](std::string x) { return "num: " + x; })
        .then([](std::string x) { std::printf("%s\n", x.c_str()); })
        .start();

    // when_all
    if constexpr(run_when_all_tests)
    {
    ctx.build([] { return 5; })
        .then([](int y) { std::printf(">>%d\n", y); },
            [](int y) { std::printf(">>%d\n", y); })
        .start();
    }

    // when_all with lvalue
    if constexpr(run_when_all_tests)
    {
    int aaa2 = 10;
    ctx.build([&aaa2]() -> int& { return aaa2; })
        .then([](int& y) { std::printf(">>%d\n", y); },
            [](int& y) { std::printf(">>%d\n", y); })
        .start();
    }

    // when_all with atomic lvalue
    if constexpr(run_when_all_tests)
    {
    std::atomic<int> aint{0};
    ctx.build([&aint]() -> std::atomic<int>& { return aint; })
        .then([](auto& y) { y += 5; }, [](auto& y) { y += 5; })
        .then([&aint] {
            std::printf("AINT: %d\n", aint.load());
            assert(aint == 10);
        })
        .start();
    }

    // when_all returns
    if constexpr(run_when_all_tests)
    {
    ctx.build([] { return 5; })
        .then([](int y) { return y + 1; }, [](int y) { return y + 2; })
        .then([](int z0, int z1) {
            assert(z0 == 6);
            assert(z1 == 7);
        })
        .start();
    }

    // when_all returns lvalues
    if constexpr(run_when_all_tests)
    {
        int lv0 = 0;
        int lv1 = 0;
        ctx.build([&lv0, &lv1]() -> std::tuple<int&, int&> {
               return {lv0, lv1};
           })
            .then(
                [](auto y) -> int { // TODO: can't return reference, fix
                    std::get<0>(y) += 1;
                    return std::get<0>(y);
                },
                [](auto y) -> int { // TODO: can't return reference, fix
                    std::get<1>(y) += 2;
                    return std::get<1>(y);
                })
            .then([&lv0, &lv1](int z0, int z1) {
                assert(z0 == 1);
                assert(z1 == 2);
                assert(lv0 == 1);
                assert(lv1 == 2);
            })
            .start();
    }

    // execute_after_move(std::move(computation));
    // wait_until_complete(ctx, std::move(computation));
    computation.start();
    sleep_ms(200);
}
#endif
