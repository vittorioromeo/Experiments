

#include <atomic>
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

        using return_type = TReturn;
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

        using this_type = node_then<TParent, TF>;
        using continuable_type = continuable<node_then<TParent, TF>>;

        // TODO: might be useful?
        /*
        using input_type = void_to_nothing_t<typename TParent::return_type>;
        using return_type = decltype(call_ignoring_nothing(
            std::declval<TF>(), std::declval<input_type>()));
        */

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



        using this_type = node_wait_all<TParent, TFs...>;
        using continuable_type = continuable<node_wait_all<TParent, TFs...>>;

        movable_atomic<int> _ctr{sizeof...(TFs)};



        template <typename T>
        auto execute(T&& x) &
        {
            // TODO: figure out lifetime of `x`!
            auto exec = [this, x](auto& f) {
                this->post([this, x, &f] {

                    // TODO: un-hardcode
                    // TODO: never move? always copy? always ref?
                    f(std::get<0>(x));
                });
            };

            // TODO: gcc bug?
            (exec(static_cast<TFs&>(*this)), ...);
        }

        template <typename T, typename TNode, typename... TNodes>
        auto execute(T&& x, TNode& n, TNodes&... ns) &
        {
            // TODO: figure out lifetime of `x`!
            auto exec = [this, x, &n, &ns...](auto& f) {
                this->post([this, x, &n, &ns..., &f] {

                    // TODO: un-hardcode
                    // TODO: never move? always copy? always ref?
                    f(std::get<0>(x));
                    if(--_ctr == 0)
                    {
                        n.execute(ns...);
                    }
                });
            };

            // TODO: gcc bug?
            (exec(static_cast<TFs&>(*this)), ...);
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
    /*
        template <typename TParent, typename TReturn, typename TF>
        template <typename... TConts>
        auto node_then<TParent, TReturn, TF>::wait_all(TConts&&... conts) &&
        {
            return node_wait_all<this_type, TConts...>{
                std::move(*this), FWD(conts)...};
        }
    */
    template <typename TF>
    auto context::build(TF&& f)
    {
        using return_type = void; // decltype(f());
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
    ll::sleep_ms(1200);
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

    // with lvalue
    int aaa = 10;
    ctx.build([&aaa]() -> int& { return aaa; })
        .then([](int& x) { return std::to_string(x); })
        .then([](std::string x) { return "num: " + x; })
        .then([](std::string x) { std::printf("%s\n", x.c_str()); })
        .start();

    // with moveonly
    ctx.build([] { return nocopy{}; }).then([](nocopy) {}).start();

    // when_all
    ctx.build([] { return 5; })
        .then([](int y) { std::printf(">>%d\n", y); },
            [](int y) { std::printf(">>%d\n", y); })
        .start();

    // when_all with lvalue
    ctx.build([&aaa]() -> int& { return aaa; })
        .then([](int& y) { std::printf(">>%d\n", y); },
            [](int& y) { std::printf(">>%d\n", y); })
        .start();

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
