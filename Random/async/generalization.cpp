#include "nothing.hpp"
#include "perfect_capture.hpp"
//#include "threading.hpp"
#include "utility.hpp"

#include <ecst/thread_pool.hpp>
#include <ecst/utils.hpp>
#include <experimental/tuple>
#include <functional>
#include <tuple>

namespace orizzonte
{
    namespace policy
    {
        namespace post
        {
            namespace impl
            {
                // clang-format off
                struct base_t { };
                struct automatic_t  : base_t, boost::hana::int_<0> { };
                struct always_t     : base_t, boost::hana::int_<1> { };
                struct never_t      : base_t, boost::hana::int_<2> { };
                // clang-format on

                template <typename T>
                constexpr auto is = std::is_base_of<base_t, std::decay_t<T>>{};
            }

            constexpr impl::always_t always{};
            constexpr impl::never_t never{};
            constexpr impl::automatic_t automatic{};
        }
    }

    namespace result
    {
        template <typename... Ts>
        using t = std::tuple<void_to_nothing_t<Ts>...>;

        using none_t = t<>;

        template <typename... Ts>
        using of_apply = decltype(apply_ignore_nothing(std::declval<Ts>()...));
    }

    template <typename, typename, typename>
    class node_then;

    template <typename, typename, typename...>
    class node_wait_all;

    // Continuations between nodes are implemented as free functions in order to
    // avoid code repetition.
    // Node types will provide ref-qualified member functions that call the free
    // functions.
    namespace continuation
    {
        template <typename TContinuable, typename T, typename... Ts>
        auto then(TContinuable&& c, T&& x, Ts&&... xs);

        template <typename TContext, typename TChain>
        auto wait(TContext& ctx, TChain&& chain);
    }

    template <typename TDerived>
    struct ctrp
    {
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
            return static_cast<TDerived&&>(*this);
        }
    };

    // CRTP base class for all nodes that provide continuation member functions.
    template <typename TDerived>
    class continuable : public ctrp<TDerived>
    {
        // Clang bug prevents this:
        /*
        template <typename TContinuable, typename... TConts>
        friend auto continuation::then(TContinuable&& c, TConts&&... conts);

    protected:
        */
    public:
        using this_type = TDerived;

    public:
        template <typename... Ts>
        auto then(Ts&&... xs) &
        {
            return continuation::then(this->as_derived(), FWD(xs)...);
        }

        template <typename... Ts>
        auto then(Ts&&... xs) &&
        {
            return continuation::then(
                std::move(this->as_derived()), FWD(xs)...);
        }

        decltype(auto) wait() &
        {
            return continuation::wait(
                this->as_derived().ctx(), this->as_derived());
        }

        decltype(auto) wait() &&
        {
            auto& ctx = this->as_derived().ctx();
            return continuation::wait(ctx, std::move(this->as_derived()));
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
    class root : public base_node<TContext>, public ctrp<root<TContext>>
    {
    private:
        friend TContext;

    public:
        using this_type = root<TContext>;

    protected:
        using base_type = base_node<TContext>;
        using context_type = typename base_type::context_type;
        using return_type = result::none_t;
        using base_type::base_type;

    public:
        // TODO: should be `protected`?
        template <typename TNode, typename... TNodes>
        void start(TNode& n, TNodes&... ns) &
        {
            n.execute(result::none_t{}, ns...);
        }
    };

    // TODO: should this perfect-capture parent?
    // probably not
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

        static constexpr auto parent_is_root() noexcept
        {
            return vrm::core::is_specialization_of<TParent, root>{};
        }

        template <typename TPolicyPost, typename TF>
        void post_if_first(TPolicyPost pp, TF&& f)
        {
            static_assert(policy::post::impl::is<TPolicyPost>);

            IF_CONSTEXPR(pp == policy::post::automatic)
            {
                IF_CONSTEXPR(parent_is_root())
                {
                    this->post(FWD(f));
                }
                else
                {
                    FWD(f)();
                }
            }
            else IF_CONSTEXPR(pp == policy::post::always)
            {
                this->post(FWD(f));
            }
            else IF_CONSTEXPR(pp == policy::post::never)
            {
                FWD(f)();
            }
        }

        template <typename TPolicyPost, typename TF>
        void post_unless_never(TPolicyPost pp, TF&& f)
        {
            static_assert(policy::post::impl::is<TPolicyPost>);

            IF_CONSTEXPR(pp == policy::post::never)
            {
                FWD(f)();
            }
            else
            {
                this->post(FWD(f));
            }
        }
    };

    template <typename TPolicyPost, typename TParent, typename TF>
    class node_then : private TF,
                      protected child_of<TParent>,
                      public continuable<node_then<TPolicyPost, TParent, TF>>
    {
    public:
        template <typename>
        friend class root;

        template <typename, typename, typename>
        friend class node_then;

        template <typename, typename, typename...>
        friend class node_wait_all;

        using this_type = node_then<TPolicyPost, TParent, TF>;
        using context_type = typename TParent::context_type;
        using continuable_type = continuable<this_type>;
        using policy_post_type = TPolicyPost;

        friend context_type;
        friend continuable_type;

    public:
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
            this->post_if_first(
                policy_post_type{}, [ this, x = FWD_CAPTURE(x) ]() mutable {
                    apply_ignore_nothing(as_f(), forward_like<T>(x.get()));
                });
        }

        template <typename T, typename TNode, typename... TNodes>
        auto execute(T&& x, TNode& n, TNodes&... ns) &
        {
            // `fwd_capture` is used to preserve "lvalue references".
            this->post_if_first(policy_post_type{},
                [ this, x = FWD_CAPTURE(x), &n, &ns... ]() mutable {
                    // Take the result value of this function...
                    decltype(auto) res_value =
                        apply_ignore_nothing(as_f(), forward_like<T>(x.get()));

                    // ...and wrap it into a tuple. Preserve "lvalue
                    // references".
                    // (I think it's safe to move here.)
                    result::t<decltype(res_value)> res(std::move(res_value));
                    n.execute(std::move(res), ns...);
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
        using continuable_type::wait;
        using continuable_type::as_derived;

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

        movable_atomic(movable_atomic&& r) noexcept : base_type{r.load()}
        {
        }

        movable_atomic& operator=(movable_atomic&& r) noexcept
        {
            this->store(r.load());
            return *this;
        }
    };

    template <std::size_t TAlign, typename T>
    class aligned_wrapper
    {
    private:
        alignas(TAlign) T _x;

    public:
        aligned_wrapper() = default;

        template <typename TFwd>
        aligned_wrapper(TFwd&& x) : _x(FWD(x))
        {
        }

        auto& get() & noexcept
        {
            return _x;
        }

        const auto& get() const & noexcept
        {
            return _x;
        }

        auto get() &&
        {
            return std::move(_x);
        }

        operator T&() &
        {
            return _x;
        }

        operator const T&() const &
        {
            return _x;
        }

        operator T &&() &&
        {
            return std::move(_x);
        }
    };

    template <std::size_t TAlign, typename... Ts>
    class aligned_tuple : public std::tuple<aligned_wrapper<TAlign, Ts>...>
    {
    private:
        using base_type = std::tuple<aligned_wrapper<TAlign, Ts>...>;

    public:
        using base_type::base_type;
    };

    template <typename TPolicyPost, typename TParent, typename... TFs>
    class node_wait_all
        : private TFs...,
          protected child_of<TParent>,
          public continuable<node_wait_all<TPolicyPost, TParent, TFs...>>
    {
    public:
        template <typename>
        friend class root;

        template <typename, typename, typename>
        friend class node_then;

        template <typename, typename, typename...>
        friend class node_wait_all;

        using this_type = node_wait_all<TPolicyPost, TParent, TFs...>;
        using context_type = typename TParent::context_type;
        using continuable_type = continuable<this_type>;
        using policy_post_type = TPolicyPost;

        friend context_type;
        friend continuable_type;

    public:
        using input_type = typename child_of<TParent>::input_type;

        // TODO: make this work
        /*
        using return_type = result::t<
            aligned_wrapper<result::of_apply<TFs, input_type>, 64>...>;
        */

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
                this->post_unless_never(policy_post_type{},
                    [ this, x = std::move(x), &f ]() mutable {

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
                [ this, x = FWD_COPY_CAPTURE(x), &n, &ns... ](auto idx, auto& f)
            {
                // `x` is now a `perfect_capture` wrapper, so it can be moved.
                // The original `x` has already been copied.
                this->post_unless_never(policy_post_type{},
                    [ this, x = std::move(x), &n, &ns..., &f ] {
                        // Should either propagate an "lvalue reference" to the
                        // original `x` or move the copy of `x` that was made in
                        // `exec`.

                        /* TODO: conditionally unpack aligned tuple
                        decltype(auto) res = apply_ignore_nothing(
                            [&f](auto&&... vs) -> decltype(
                                auto) { return f(vs.get()...); },
                            forward_like<T>(x.get()));
                        */

                        decltype(auto) res =
                            apply_ignore_nothing(f, forward_like<T>(x.get()));

                        // Don't set results for `void`-returning functions.
                        IF_CONSTEXPR(!is_nothing(res))
                        {
                            // Should either propagate an "lvalue reference" to
                            // the original `x` or move the copy of `x` that was
                            // made in `exec`.

                            std::get<decltype(idx){}>(_results) =
                                forward_like<T>(res);

                            // TODO: race cond?
                            /*
                            auto& datum = std::get<decltype(idx){}>(_results);
                            datum = forward_like<T>(res);
                            */
                        }

                        // If this is the last `TFs...` function to end, trigger
                        // the next continuation.
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
        using continuable_type::wait;
        using continuable_type::as_derived;

        template <typename... TNodes>
        auto start(TNodes&... ns) &
        {
            this->parent().start(*this, ns...);
        }
    };


    // "compose" assumes that all inputs are existing continuables
    template <typename TContext, typename... TConts>
    auto compose(TContext&& ctx, TConts&&... conts)
    {
        static_assert(sizeof...(conts) > 0,
            "can't compose a chain of empty continuables");

        auto wrap_start = [](auto& c) { return [&c] { c.start(); }; };
        return ctx.build(wrap_start(FWD(conts))...);
    }

    template <std::size_t>
    struct node_type_dispatch
    {
        template <typename... Ts>
        using type = node_wait_all<Ts...>;
    };

    template <>
    struct node_type_dispatch<1>
    {
        template <typename... Ts>
        using type = node_then<Ts...>;
    };

    template <std::size_t TN, typename... Ts>
    using node_type_dispatch_t =
        typename node_type_dispatch<TN>::template type<Ts...>;


    namespace continuation
    {
        template <typename TContinuable, typename T, typename... Ts>
        auto then(TContinuable&& c, T&& x, Ts&&... xs)
        {
            using c_type = typename std::decay_t<TContinuable>::this_type;

            IF_CONSTEXPR(policy::post::impl::is<T>)
            {
                using policy_type = std::decay_t<T>;
                using node_type = node_type_dispatch_t<sizeof...(xs),
                    policy_type, c_type, Ts...>;

                IF_CONSTEXPR(sizeof...(xs) == 0)
                {
                    struct cant_build_chain_of_zero_continuables;
                    cant_build_chain_of_zero_continuables{};
                }
                else
                {
                    return node_type{FWD(c).as_derived(), FWD(xs)...};
                }
            }
            else
            {
                using policy_type = policy::post::impl::automatic_t;
                using node_type = node_type_dispatch_t<1 + sizeof...(xs),
                    policy_type, c_type, T, Ts...>;

                return node_type{
                    forward_like<TContinuable>(FWD(c).as_derived()), FWD(x),
                    FWD(xs)...};
            }
        }

        template <typename TContext, typename TChain>
        auto wait(TContext& ctx, TChain&& chain)
        {
            ecst::latch l{1};

            using c_type = std::decay_t<TChain>;
            using return_type = typename c_type::return_type;

            return_type res;

            // Consumes `chain`.
            auto c = then(std::move(chain), [&l, &res](auto&&... xs) {
                res = return_type(FWD(xs)...);
                l.decrement_and_notify_all();
            });

            l.execute_and_wait_until_zero(
                [&ctx, &c, &l, &res]() mutable { c.start(); });

            return res;
        }
    }


    template <typename TContext, typename... Ts>
    auto build_root(TContext& ctx, Ts&&... xs)
    {
        using root_type = root<TContext>;
        return continuation::then(root_type{ctx}, FWD(xs)...);
    }


    template <typename TContext>
    class context_facade
    {
    public:
        template <typename... Ts>
        auto build(Ts&&... xs)
        {
            return ll::build_root(static_cast<TContext&>(*this), FWD(xs)...);
        }
    };
}

template <typename T>
void execute_after_move(T x)
{
    x.start();
    ll::sleep_ms(200);
}

/*
template <typename TContext, typename... TChains>
void wait_until_complete(TContext& ctx, TChains&&... chains)
{
    ecst::latch l{1};

    auto c = std::experimental::apply(
        [&ctx](auto&&... xs) { return compose(ctx, FWD(xs)...); },
        chains).then([&l] { l.decrement_and_notify_all(); });

    l.execute_and_wait_until_zero(
        [&ctx, &c, &l, chains = FWD_CAPTURE_AS_TUPLE(chains) ]() mutable {

            c.start();
        });
}
*/

namespace orizzonte
{
    template <typename TContext, typename TChain>
    void wait_until_complete(TContext& ctx, TChain&& chain)
    {
        ecst::latch l{1};

        auto c = FWD(chain).then([&l] { l.decrement_and_notify_all(); });
        l.execute_and_wait_until_zero([&ctx, &c, &l]() mutable { c.start(); });
    }
}

struct nocopy
{
    nocopy() = default;

    nocopy(const nocopy&) = delete;
    nocopy& operator=(const nocopy&) = delete;

    nocopy(nocopy&&) = default;
    nocopy& operator=(nocopy&&) = default;
};


#if 0
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

#if 1
struct pool
{
    template <typename TF>
    void post(TF&& f)
    {
        // Required because `std::thread` always copies its callable argument.
        /*auto jptr =
            std::make_shared<ecst::fixed_function<void()>>([f = std::move(
                                                                f)]() mutable {
                f();
            });
        std::thread{[jptr]{ (*jptr)(); }}.detach();
        */
        std::thread{std::move(f)}.detach();
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
        .wait();
}
#endif

#if 0
int main()
{
    pool p;
    my_context ctx{p};

    for(int i = 0; i < 25; ++i)
    {
        ctx.post([&ctx, i] {
            std::cout << "start " << i << std::endl;
            std::atomic<bool> k{false};

            auto x = ctx.build(orizzonte::policy::post::never, [y = 1]{})
                         .then([i, &k] {
                             std::cout << i << "\n";
                             k = true;
                         });

            x.start();

            while(!k)
            {
                ll::sleep_ms(5);
            }
            std::cout << "end   " << i << std::endl;
        });

        // ll::sleep_ms(2);
    }

    ll::sleep_ms(500);
}
#endif

#if 0
int main()
{
    pool pl;
    my_context ctx{pl};

    // TODO: fix!!!
    auto a = ctx.build([ i = 0, x = nocopy{} ]() mutable {
        ++i;
        return i;
    });

    // TODO: automatically unpack tuple if there's a single arg
    auto b = ctx.build([&a] { return a.wait(); }).then([](std::tuple<int> t) {
        assert(std::get<0>(t) == 1);
    });

    b.wait();
}
#endif

#if 0
int main()
{
    pool pl;
    my_context ctx{pl};

    std::vector<int> v{5, 1, 67, 31, 7, 3, 1, 8, 56, 9, 2, 36, 1, 7, 48, 9, 3,
        47, 2, 1, 8, 65, 9, 4, 2, 347, 1, 2, 7, 458, 3};

    auto qs_step = boost::hana::fix([&ctx](
        auto self, auto b, auto e, auto f_then) {

        using std::swap;
        auto size = std::distance(b, e);
        if(size <= 1) return;

        auto p = std::prev(e);
        swap(*std::next(b, size / 2), *p);
        auto q = std::partition(b, p, [p](decltype(*p) x) { return x < *p; });
        swap(*q, *p);

        auto c = ctx.build([self, b, q, f_then] { self(b, q, f_then); },
            [self, q, e, f_then] { self(std::next(q), e, f_then); });

        auto test = std::make_shared<std::decay_t<decltype(c)>>(std::move(c));

        auto z = ctx.build([test]() mutable { test->wait(); });
        z.start();
    });

    qs_step(v.begin(), v.end(), [&v] {

    });

    ll::sleep_ms(250);
    for(const auto& x : v) std::cout << x << " ";
    ll::sleep_ms(250);
}
#endif

#if 1
int main()
{
    pool p;
    my_context ctx{p};

    ctx.build([] { return 10; })
        .then([](int x) { return x + 1; })
        .then([](int x) { return x + 2; }, [](int x) { return x + 3; })
        .then([](int a, int b) { return a + b; })
        .wait();
}
#endif

#if 0
int main()
{
    pool p;
    my_context ctx{p};

    auto c = ctx.build([] { return 10; })
                 .then([](int x) { return std::to_string(x); })
                 .then([](std::string x) { return "num: " + x; })
                 .then([](std::string x) { std::printf("%s\n", x.c_str()); });

    c.start();

    // with moveonly
    auto c2 = ctx.build([] { return nocopy{}; }).then([](nocopy) {});
    c2.start();

    sleep_ms(200);
}
#endif

#if 0
int main()
{
    pool p;
    my_context ctx{p};

    for(int i = 0; i < 25; ++i)
    {
        p.post([&ctx, i] {
            std::atomic<bool> k{false};

            auto x = ctx.build([i] { return i; }).then([i, &k](int z) {
                std::cout << i << z << "\n";
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
#endif

#if 0
int main()
{
    // TODO: gcc segfault on mingw when set to true.
    constexpr bool run_when_all_tests{true};

    pool p;
    my_context ctx{p};

    // auto computation =               // .
    //     ctx.build([] { return 10; }) // .
    //         .then([](int x) { return std::to_string(x); })
    //         .then([](std::string x) { return "num: " + x; })
    //         .then([](std::string x) { std::printf("%s\n", x.c_str()); });

    for(volatile int i = 0; i < 20; ++i)
    {
        ctx.post([i, &ctx] {
            ctx.build([i]() -> int { return i; })
                .then([](int x) { return std::to_string(x); })
                .then([](std::string x) { return "inum: " + x; })
                .then([](std::string x) { std::printf("%s\n", x.c_str()); })
                .wait();

            // with moveonly
            // auto c2 = ctx.build([] { return nocopy{}; }).then([](nocopy) {});
            // c2.start();

            // ll::wait_until_complete(ctx, c);
            // sleep_ms(100);
        });
    }

    {
        // with lvalue
        int aaa = 10;
        auto c =
            ctx.build([&aaa]() -> int& { return aaa; })
                .then([](int& x) { return std::to_string(x); })
                .then([](std::string x) { return "num: " + x; })
                .then([](std::string x) { std::printf("%s\n", x.c_str()); });
        c.wait();
    }

    // when_all
    // IF_CONSTEXPR(run_when_all_tests)
    {
        auto c = ctx.build([] { return 5; })
                     .then([](int y) { std::printf(">>%d\n", y); },
                         [](int y) { std::printf(">>%d\n", y); });
        c.wait();
    }

    // when_all with lvalue
    // IF_CONSTEXPR(run_when_all_tests)
    {
        int aaa2 = 10;
        auto c = ctx.build([&aaa2]() -> int& { return aaa2; })
                     .then([](int& y) { std::printf(">>%d\n", y); },
                         [](int& y) { std::printf(">>%d\n", y); });
        c.wait();
    }

    // when_all with atomic lvalue
    IF_CONSTEXPR(run_when_all_tests)
    {
        std::atomic<int> aint{0};
        auto c = ctx.build([&aint]() -> std::atomic<int>& { return aint; })
                     .then([](auto& y) { y += 5; }, [](auto& y) { y += 5; })
                     .then([&aint] {
                         std::printf("AINT: %d\n", aint.load());
                         assert(aint == 10);
                     });
        c.wait();
    }

    // when_all returns
    IF_CONSTEXPR(run_when_all_tests)
    {
        auto c =
            ctx.build([] { return 5; })
                .then([](int y) { return y + 1; }, [](int y) { return y + 2; })
                .then([](int z0, int z1) {
                    assert(z0 == 6);
                    assert(z1 == 7);
                });
        c.wait();
    }

    // when_all returns lvalues
    IF_CONSTEXPR(run_when_all_tests)
    {
        int lv0 = 0;
        int lv1 = 0;
        auto c =
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
                });
        c.wait();
    }

    // execute_after_move(std::move(computation));
    // wait_until_complete(ctx, std::move(computation));
    // computation.start();
    ll::sleep_ms(400);
}
#endif
