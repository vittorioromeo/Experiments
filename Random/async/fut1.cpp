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
                struct base_t
                {
                };

                struct automatic_t : base_t, boost::hana::int_<0>
                {
                };

                struct always_t : base_t, boost::hana::int_<1>
                {
                };

                struct never_t : base_t, boost::hana::int_<2>
                {
                };

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
        void wait(TContext& ctx, TChain&& chain);
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
            // TODO: as derived?
            return continuation::then(*this, FWD(xs)...);
        }

        template <typename... Ts>
        auto then(Ts&&... xs) &&
        {
            // TODO: as derived?
            return continuation::then(std::move(*this), FWD(xs)...);
        }

        void wait() &
        {
            continuation::wait(this->as_derived().ctx(), this->as_derived());
        }

        void wait() &&
        {
            auto& ctx = this->as_derived().ctx();
            continuation::wait(ctx, std::move(this->as_derived()));
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

    class debug_node_info
    {
    private:
#ifndef NDEBUG
        bool _executed{false};
#endif

    public:
        void set_executed() noexcept
        {
#ifndef NDEBUG
            _executed = true;
#endif
        }

        void assert_not_executed() noexcept
        {
#ifndef NDEBUG
            assert(!_executed);
#endif
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
                    result::t<decltype(res_value)> res(FWD(res_value));
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

    template <typename TPolicyPost, typename TParent, typename... TFs>
    class node_wait_all
        : private TFs...,
          protected child_of<TParent>,
          public continuable<node_wait_all<TPolicyPost, TParent, TFs...>>
    {
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

                        // If this is the last `TFs...` function to end, trigger
                        // the
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
        using continuable_type::wait;

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

                return node_type{FWD(c).as_derived(), FWD(x), FWD(xs)...};
            }
        }

        template <typename TContext, typename TChain>
        void wait(TContext& ctx, TChain&& chain)
        {
            ecst::latch l{1};

            auto c = FWD(chain).then(
                [&l]() { l.decrement_and_notify_all(); });
            l.execute_and_wait_until_zero(
                [&ctx, &c, &l]() mutable { c.start(); });
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


volatile unsigned int state = 0;
unsigned int get_value()
{
    return state;
}

int main()
{   
    pool p;
    my_context ctx{p};

    unsigned int temp;
    ctx.build([&temp]{ temp = get_value(); }).wait();
    return temp;
}