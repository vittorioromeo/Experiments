#include "shared.hpp"
#include <type_traits>

namespace ll
{
    struct context
    {
        ll::pool& _p;
        context(ll::pool& p) noexcept : _p{p}
        {
        }

        template <typename TF>
        auto build(TF&& f);
    };


    template <typename TF>
    struct node_then : TF
    {
        ll::context& _ctx;

        template <typename TFFwd>
        node_then(TFFwd&& f, ll::context& ctx)
            : TF(std::forward<TFFwd>(f)), _ctx{ctx}
        {
        }

        template <typename TCont>
        auto then(TCont&& cont)
        {
            auto new_cont = // .
                [ this, cont = std::forward<TCont>(cont) ]() mutable
            {
                (*this)();
                _ctx._p.post(std::move(cont));
            };

            return node_then<std::decay_t<decltype(new_cont)>>{
                std::move(new_cont), _ctx};
        }
    };

    template <typename TF>
    auto context::build(TF&& f)
    {
        auto cont = [ f = std::forward<TF>(f), this ]() mutable
        {
            _p.post(std::move(f));
        };

        return node_then<std::decay_t<decltype(cont)>>{std::move(cont), *this};
    }
}

int main()
{
    ll::pool p;
    ll::context ctx{p};

    ctx.build([] { ll::print_sleep_ms(250, "A"); })
        .then([] { ll::print_sleep_ms(250, "B"); })
        .then([] { ll::print_sleep_ms(250, "C"); })();

    ll::sleep_ms(1000);
}
