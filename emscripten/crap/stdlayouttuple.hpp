#include <cstddef>
#include <type_traits>
#include <utility>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

namespace vrm
{
    namespace sdl
    {
        template <typename... Ts>
        struct sl_tuple;

        template <typename T, typename... Ts>
        struct sl_tuple<T, Ts...>
        {
            using this_type = sl_tuple<T, Ts...>;
            using rest_type = sl_tuple<Ts...>;

            T first;
            rest_type rest;

            sl_tuple() noexcept
            {
                static_assert(::std::is_standard_layout<this_type>{}, "");
            }

            template <typename U, typename... Us,
                typename = ::std::enable_if_t<
                    !::std::is_base_of<sl_tuple, ::std::decay_t<U>>{}>>
            sl_tuple(U&& u, Us&&... tail) noexcept : first(FWD(u)),
                                                     rest(FWD(tail)...)
            {
                static_assert(::std::is_standard_layout<this_type>{}, "");
            }

            template <typename X>
            static constexpr ::std::size_t offset_of{
                std::is_same<T, X>{}
                    ? 0
                    : sizeof(T) + rest_type::template offset_of<X>};

            template <::std::size_t N>
            static constexpr ::std::size_t offset_of_nth{
                N == 0 ? 0
                       : sizeof(T) + rest_type::template offset_of_nth<N - 1>};
        };

        template <typename T>
        struct sl_tuple<T>
        {
            using this_type = sl_tuple<T>;

            T first;

            sl_tuple() noexcept
            {
                static_assert(::std::is_standard_layout<this_type>{}, "");
            }

            template <typename U,
                typename = ::std::enable_if_t<
                    !::std::is_base_of<sl_tuple, ::std::decay_t<U>>{}>>
            sl_tuple(U&& u) noexcept : first(FWD(u))
            {
                static_assert(::std::is_standard_layout<this_type>{}, "");
            }

            template <typename>
            static constexpr ::std::size_t offset_of{0};

            template <::std::size_t>
            static constexpr ::std::size_t offset_of_nth{0};
        };

        template <>
        struct sl_tuple<>
        {
        };

        namespace detail
        {
            template <::std::size_t i, typename T>
            struct sl_tuple_element;

            template <::std::size_t i, typename T, typename... Ts>
            struct sl_tuple_element<i, sl_tuple<T, Ts...>>
                : sl_tuple_element<i - 1, sl_tuple<Ts...>>
            {
            };

            template <typename T, typename... Ts>
            struct sl_tuple_element<0, sl_tuple<T, Ts...>>
            {
                using type = T;
            };

            template <::std::size_t i>
            struct sl_tuple_accessor
            {
                template <typename... Ts>
                static auto& get(sl_tuple<Ts...>& t) noexcept
                {
                    return sl_tuple_accessor<i - 1>::get(t.rest);
                }

                template <typename... Ts>
                static const auto& get(const sl_tuple<Ts...>& t) noexcept
                {
                    return sl_tuple_accessor<i - 1>::get(t.rest);
                }
            };

            template <>
            struct sl_tuple_accessor<0>
            {
                template <typename... Ts>
                static auto& get(sl_tuple<Ts...>& t) noexcept
                {
                    return t.first;
                }

                template <typename... Ts>
                static const auto& get(const sl_tuple<Ts...>& t) noexcept
                {
                    return t.first;
                }
            };
        }

        template <typename... Ts>
        auto make_sl_tuple(Ts&&... x) noexcept
        {
            return sl_tuple<::std::decay_t<Ts>...>(FWD(x)...);
        }

        template <::std::size_t i, typename... Ts>
        auto& get(sl_tuple<Ts...>& t) noexcept
        {
            return detail::sl_tuple_accessor<i>::get(t);
        }

        template <::std::size_t i, typename... Ts>
        const auto& get(const sl_tuple<Ts...>& t) noexcept
        {
            return detail::sl_tuple_accessor<i>::get(t);
        }

        template <typename T, typename... Ts>
        constexpr auto offset_of(const sl_tuple<Ts...>& t) noexcept
        {
            return decltype(t)::template offset_of<T>;
        }

        template <::std::size_t N, typename... Ts>
        constexpr auto offset_of_nth(const sl_tuple<Ts...>& t) noexcept
        {
            return decltype(t)::template offset_of_nth<N>;
        }

        static_assert(::std::is_standard_layout<sl_tuple<bool, int, float, char,
                          double, sl_tuple<int, char>>>{},
            "Compiler is stupid");

        static_assert(sl_tuple<char>::offset_of_nth<0> == 0, "");
        static_assert(sl_tuple<int>::offset_of_nth<0> == 0, "");

        static_assert(sl_tuple<char>::offset_of<char> == 0, "");
        static_assert(sl_tuple<int>::offset_of<int> == 0, "");

        static_assert(sl_tuple<char, int, float>::offset_of_nth<0> == 0, "");
        static_assert(
            sl_tuple<char, int, float>::offset_of_nth<1> == sizeof(char), "");
        static_assert(sl_tuple<char, int, float>::offset_of_nth<2> ==
                          sizeof(char) + sizeof(int),
            "");

        static_assert(sl_tuple<char, int, float>::offset_of<char> == 0, "");
        static_assert(
            sl_tuple<char, int, float>::offset_of<int> == sizeof(char), "");
        static_assert(sl_tuple<char, int, float>::offset_of<float> ==
                          sizeof(char) + sizeof(int),
            "");
    }
}

struct vert
{
    vrm::sdl::sl_tuple<int, int> _vals;

    template <typename... Ts>
    vert(Ts&&... xs)
        : _vals(FWD(xs)...)
    {
    }

    auto& x() { return vrm::sdl::get<0>(_vals); }
    auto& y() { return vrm::sdl::get<1>(_vals); }
};

static_assert(::std::is_standard_layout<vert>{}, "");

int main()
{
    using namespace vrm::sdl;

    auto vert(make_sl_tuple(0, 0, 0));



    return 0;
}