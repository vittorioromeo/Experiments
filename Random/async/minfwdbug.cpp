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
            // TODO: as derived?
            return continuation::then(*this, FWD(xs)...);
        }

        template <typename... Ts>
        auto then(Ts&&... xs) &&
        {
            // TODO: as derived?
            return continuation::then(std::move(*this), FWD(xs)...);
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

        template <typename TParentFwd, typename TFFwd>
        node_then(TParentFwd&& p, TFFwd&& f)
            : TF{FWD(f)}, child_of<TParent>{FWD(p)}
        {
        }
    };


    namespace continuation
    {
        template <typename TContinuable, typename T, typename... Ts>
        auto then(TContinuable&& c, T&& x, Ts&&... xs)
        {
            using c_type = typename std::decay_t<TContinuable>::this_type;

            using policy_type = policy::post::impl::automatic_t;
            using node_type = node_then<policy_type, c_type, T>;

            // TODO: ?
            static_assert(std::is_rvalue_reference<decltype(FWD(c))>{});
            static_assert(std::is_rvalue_reference<decltype(
                    forward_like<TContinuable>(FWD(c).as_derived()))>{});

            return node_type{FWD(c).as_derived(), FWD(x), FWD(xs)...};
        }
    }

    template <typename TContext, typename... Ts>
    auto build_root(TContext& ctx, Ts&&... xs)
    {
        using root_type = root<TContext>;
        return continuation::then(root_type{ctx}, FWD(xs)...);
    }

    template <typename TContext>
    struct context_facade
    {
        template <typename... Ts>
        auto build(Ts&&... xs)
        {
            return ll::build_root(static_cast<TContext&>(*this), FWD(xs)...);
        }
    };
}

struct nocopy
{
    nocopy() = default;

    nocopy(const nocopy&) = delete;
    nocopy& operator=(const nocopy&) = delete;

    nocopy(nocopy&&) = default;
    nocopy& operator=(nocopy&&) = default;
};

struct my_context : public ll::context_facade<my_context>
{
    template <typename TF>
    void post(TF&& f)
    {
        f();
    }
};

int main()
{
    my_context ctx;

    // TODO: fix!!!
    auto a = ctx.build([ i = 0, x = nocopy{} ]() mutable {
        ++i;
        return i;
    });
}
