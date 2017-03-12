#include "../overload.hpp"
#include "../utils.hpp"
#include "../visit.hpp"
#include "../visit_helper.hpp"
#include "./test_utils.hpp"

namespace test
{
    namespace impl
    {
        template <typename T>
        struct type_w_t
        {
            using type = T;
        };
    }

    template <typename... Ts>
    constexpr impl::type_w_t<Ts...> type_w{};

    namespace impl
    {
        template <typename... Ts>
        struct alternatives_t
        {
        };

        template <typename>
        struct unpack_alternatives;

        template <typename... Ts>
        struct unpack_alternatives<alternatives_t<Ts...>>
        {
            template <template <typename...> class T>
            using apply = T<Ts...>;
        };

        template <typename T>
        struct maker_t
        {
            template <typename... Ts>
            auto operator()(Ts&&... xs) const
            {
                return T(FWD(xs)...);
            }
        };
    }

    template <typename... Ts>
    constexpr impl::alternatives_t<Ts...> alternatives{};

    template <typename TAlternatives, typename TF>
    void with_all_variant_implementations(TAlternatives, TF&& f)
    {
        using helper = impl::unpack_alternatives<TAlternatives>;

        using v_std = typename helper::template apply<std::variant>;
        using v_boost = typename helper::template apply<boost::variant>;
        using v_eggs = typename helper::template apply<eggs::variant>;
        using v_typesafe = typename helper::template apply<type_safe::variant>;

        f(impl::maker_t<v_std>{});
        f(impl::maker_t<v_boost>{});
        f(impl::maker_t<v_eggs>{});
        f(impl::maker_t<v_typesafe>{});
    }
}