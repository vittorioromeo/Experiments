#include <atomic>
#include <chrono>
#include <cstdio>
#include <thread>
#include <mutex>
#include <mingw.thread.h>
#include <mingw.mutex.h>
#include <mingw.condition_variable.h>
#include <ecst/thread_pool.hpp>
#include <ecst/utils.hpp>
#include <experimental/tuple>
#include <functional>
#include <thread>
#include <tuple>
#include "perfect_capture.hpp"
#include "nothing.hpp"

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
    struct node_then;

    template <typename, typename...>
    struct node_wait_all;

    namespace continuation
    {
        template <typename TContinuable, typename... TConts>
        auto then(TContinuable&& c, TConts&&... conts);
    }

    template <typename TDerived>
    struct continuable
    {
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
            return std::move(as_derived());
        }

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

    template <typename TReturn>
    struct root : base_node
    {
        using return_type = TReturn;
        using base_node::base_node;

        template <typename TNode, typename... TNodes>
        void start(TNode& n, TNodes&... ns)
        {
            n.execute(std::make_tuple(nothing), ns...);
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
    struct child_of
    {
        TParent _p;

        template <typename TParentFwd>
        child_of(TParentFwd&& p) : _p{FWD(p)}
        {
        }

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
            return parent().ctx();
        }

        const auto& ctx() const & noexcept
        {
            return parent().ctx();
        }

        template <typename TF>
        void post(TF&& f)
        {
            this->ctx()._p.post(FWD(f));
        }
    };

    template <typename TParent, typename TF>
    struct node_then : child_of<TParent>,
                       continuable<node_then<TParent, TF>>,
                       TF
    {
        using this_type = node_then<TParent, TF>;

        // TODO: might be useful?
        /*
        using input_type = void_to_nothing_t<typename TParent::return_type>;
        using return_type = decltype(call_ignoring_nothing(
            std::declval<TF>(), std::declval<input_type>()));
        */

        auto& as_f() noexcept
        {
            return static_cast<TF&>(*this);
        }

        template <typename TParentFwd, typename TFFwd>
        node_then(TParentFwd&& p, TFFwd&& f)
            : child_of<TParent>{FWD(p)}, TF{FWD(f)}
        {
        }

        template <typename T>
        auto execute(T&& x) &
        {
            // TODO: fwd_capture?
            /*this->ctx()._p.post([ this, x = FWD(x) ]() mutable {
                with_void_to_nothing(as_f(), FWD(x));
            });*/

            this->post([this, x = fwd_capture(FWD(x))]() mutable {
                apply_ignore_nothing(as_f(), forward_like<T>(x.get()));
            });
        }

        template <typename T, typename TNode, typename... TNodes>
        auto execute(T&& x, TNode& n, TNodes&... ns) &
        {
            // TODO: fwd_capture?
            /*this->ctx()._p.post([ this, x = FWD(x), &n, &ns... ]() mutable {
                decltype(auto) res = with_void_to_nothing(as_f(), FWD(x));

                this->ctx()._p.post(
                    [ res = std::move(res), &n, &ns... ]() mutable {
                        n.execute(std::move(res), ns...);
                    });
            });*/
            this->post([ this, x = fwd_capture(FWD(x)), &n, &ns... ]() mutable {

                decltype(auto) res_value = apply_ignore_nothing(as_f(), forward_like<T>(x.get()));
                std::tuple<decltype(res_value)> res(FWD(res_value));

                this->post(
                    [ res = std::move(res), &n, &ns... ]() mutable {
                        n.execute(std::move(res), ns...);
                    });
            });
        }

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
    struct node_wait_all : child_of<TParent>,
                           continuable<node_wait_all<TParent, TFs...>>,
                           TFs...
    {
        using this_type = node_wait_all<TParent, TFs...>;

        movable_atomic<int> _ctr{sizeof...(TFs)};

        template <typename TParentFwd, typename... TFFwds>
        node_wait_all(TParentFwd&& p, TFFwds&&... fs)
            : child_of<TParent>{FWD(p)}, TFs{FWD(fs)}...
        {
        }

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

// TODO: test on linux with sanitizer
#if 0
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
    ctx.build([] { return nocopy{}; })
        .then([](nocopy) {})
        .start();
#endif

    // when_all
    ctx.build([] { return 5; })
        .then([](int y) { std::printf(">>%d\n", y); }, [](int y) { std::printf(">>%d\n", y); })
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
